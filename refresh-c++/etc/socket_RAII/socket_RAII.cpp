#include "gtest/gtest.h"

#include <cstdio>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <utility>

namespace
{
using namespace std;
constexpr socket_t kInvalidVal = -1;

inline bool isSockeValid(socket_t socket_id) { return socket_id >= 0; }
void setSocketTimeout(socket_t socket_id, std::chrono::milliseconds ms)
{
  timeval tv{};
  tv.tv_sec = static_cast<long>(ms.count() / 1000);
  tv.tv_usec = static_cast<long>((ms.count() % 1000) * 1000);

  // SOLSOCKET: options fro socket level. Level number for (get/set)sockopt() to apply to socket itself
  // SO_RCVTIMEO: socket timeout
  if(::setsockopt(socket_id, SOL_SOCKET, SO_RCVTIMEO), &tv, sizeof(tv))
  {
    throw std::system_error(std::error_code(errno, std::generic_category()), where);
  }
}

} // namespace
class SocketRAIIWarpper
{
public:
  SocketRAIIWarpper(){}
  explicit SocketRAIIWarpper(socket_t socket_id)
    : socket_id_(socket_id)
  {
    if(!isSockeValid(socket_id))
    {
        // 테스트니까.. 일단 생성자에서 throw 하자.. ㅋㅋ
        throw std::runtime_error("Socket: invalid handle");
    }
  }
  ~SocketRAIIWarpper(){}

  SocketRAIIWarpper(const SocketRAIIWarpper &) = delete;
  SocketRAIIWarpper &operator=(const SocketRAIIWarpper &) = delete;
  SocketRAIIWarpper(SocketRAIIWarpper &&socket_to_move) noexcept
    : socket_id_(socket_to_move.getSocketId())
  {
    socket_to_move.resetSocketId();
  }
  SocketRAIIWarpper &operator=(SocketRAIIWarpper&& socket_to_move) {}

  size_t getSocketId() { return socket_id_; }
  void resetSocketId() { socket_id_ = kInvalidVal; }
  void closeSocket()
  {
    if(isSockeValid(socket_id_))
    {
      ::close(socket_id_);
    }
    resetSocketId();
  }

private:
  socket_t socket_id_;
}

class TESTFileRAII : public testing::Test
{
protected:
    void SetUp() override
    {
        string text = "I want to sleep now. It's 2:20 am";
        makeTestFile(text);
    }
};

TEST_F(TESTFileRAII, FileNotExist)
{
    FileDescriptorRAIIWrapper fd_wrapper = FileDescriptorRAIIWrapper::makeInstByReadOnly("no_such_file.txt");
    EXPECT_FALSE(fd_wrapper);
    EXPECT_NO_THROW(fd_wrapper.reset());
}

---
#pragma once
#include <stdexcept>
#include <string>
#include <system_error>
#include <chrono>
#include <cstring>
#include <vector>


#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
using socket_t = int;
inline bool socket_is_invalid(socket_t s){ return s<0; }
inline void socket_close(socket_t s){ if(!socket_is_invalid(s)) ::close(s); }
inline int last_error_code(){ return errno; }
inline int err_again(){ return EAGAIN; }
inline int err_inprogress(){ return EINPROGRESS; }
struct NetInit { NetInit(){} ~NetInit(){} }; // NOP on POSIX


inline std::system_error make_syserr(const char* where){
#if defined(_WIN32)
  return std::system_error(std::error_code(last_error_code(), std::system_category()), where);
#else
  return std::system_error(std::error_code(errno, std::generic_category()), where);
#endif
}

// --- Socket (low-level RAII) ---
class Socket {
public:
  Socket() noexcept : s_(invalid_value()) {}
  explicit Socket(socket_t s) : s_(s) {
    if (socket_is_invalid(s_)) throw std::runtime_error("Socket: invalid handle");
  }

  // factory: ::socket
  static Socket create(int domain, int type, int protocol=0){
    socket_t s = ::socket(domain, type, protocol);
    if (socket_is_invalid(s)) throw make_syserr("socket()");
    return Socket(s);
  }

  // move-only
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;
  Socket(Socket&& o) noexcept : s_(o.s_){ o.s_ = invalid_value(); }
  Socket& operator=(Socket&& o) noexcept {
    if (this!=&o){ close(); s_=o.s_; o.s_=invalid_value(); }
    return *this;
  }

  ~Socket(){ close(); }

  socket_t native_handle() const { return s_; }
  bool valid() const { return !socket_is_invalid(s_); }

  void close(){
    if (valid()){ socket_close(s_); s_=invalid_value(); }
  }

  void set_blocking(bool enable){
    int flags = ::fcntl(s_, F_GETFL, 0);
    if (flags<0) throw make_syserr("fcntl(F_GETFL)");
    if (enable) flags &= ~O_NONBLOCK; else flags |= O_NONBLOCK;
    if (::fcntl(s_, F_SETFL, flags)<0) throw make_syserr("fcntl(F_SETFL)");

  }

  // both send & recv timeouts
  void set_timeout(std::chrono::milliseconds ms){
    timeval tv{};
    tv.tv_sec  = static_cast<long>(ms.count()/1000);
    tv.tv_usec = static_cast<long>((ms.count()%1000)*1000);
    if (setsockopt(s_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))<0)
      throw make_syserr("setsockopt(SO_RCVTIMEO)");
    if (setsockopt(s_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv))<0)
      throw make_syserr("setsockopt(SO_SNDTIMEO)");
  }

  void set_nodelay(bool enable){
    int v = enable? 1:0;
    if (setsockopt(s_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&v), sizeof(v))!=0)
      throw make_syserr("setsockopt(TCP_NODELAY)");
  }

  void shutdown_send(){
#if defined(_WIN32)
    if (::shutdown(s_, SD_SEND)!=0) throw make_syserr("shutdown(SD_SEND)");
#else
    if (::shutdown(s_, SHUT_WR)!=0) throw make_syserr("shutdown(SHUT_WR)");
#endif
  }
};

// --- TcpStream: connected socket ---
class TcpStream {
public:
  TcpStream() = default;
  explicit TcpStream(Socket&& s) : sock_(std::move(s)) {}

  // connect(host, service) using getaddrinfo
  static TcpStream connect(const std::string& host, const std::string& service,
                           bool nodelay=true, std::chrono::milliseconds timeout=std::chrono::milliseconds{0})
  {
    addrinfo hints{};
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* res=nullptr;
    int rc = ::getaddrinfo(host.c_str(), service.c_str(), &hints, &res);
    if (rc!=0) throw std::runtime_error(std::string("getaddrinfo: ")+gai_strerror(rc));

    Socket s;
    for (addrinfo* p=res; p; p=p->ai_next){
      try {
        s = Socket::create(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (timeout.count()>0){
          // Best-effort: set non-blocking connect + poll by OS timeout (send/recv timeouts won’t affect connect)
          s.set_blocking(false);
          if (::connect(s.native_handle(), p->ai_addr, static_cast<int>(p->ai_addrlen))<0){
            int e = last_error_code();
#if defined(_WIN32)
            if (e!=WSAEWOULDBLOCK && e!=WSAEINPROGRESS) { s.close(); continue; }
#else
            if (e!=EINPROGRESS) { s.close(); continue; }
#endif
            // Wait for writable
            fd_set wfds; FD_ZERO(&wfds); FD_SET(s.native_handle(), &wfds);
            timeval tv{};
            tv.tv_sec = static_cast<long>(timeout.count()/1000);
            tv.tv_usec= static_cast<long>((timeout.count()%1000)*1000);
            int sel = ::select(static_cast<int>(s.native_handle()+1), nullptr, &wfds, nullptr, &tv);
            if (sel<=0){ s.close(); continue; }
            // check error
            int err=0; socklen_t len=sizeof(err);
            if (getsockopt(s.native_handle(), SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &len)!=0 || err!=0){
              s.close(); continue;
            }
            s.set_blocking(true);
          } else {
            // immediate connect
            s.set_blocking(true);
          }
        } else {
          if (::connect(s.native_handle(), p->ai_addr, static_cast<int>(p->ai_addrlen))<0){
            s.close(); continue;
          }
        }
        // success
        if (nodelay) s.set_nodelay(true);
        freeaddrinfo(res);
        return TcpStream(std::move(s));
      } catch(...){
        s.close();
        continue;
      }
    }
    freeaddrinfo(res);
    throw make_syserr("connect()");
  }

  // basic I/O
  // returns bytes sent/received; throws on hard errors; 0 on orderly shutdown from peer at recv
  size_t send(const void* buf, size_t len){
#if defined(_WIN32)
    int n = ::send(sock_.native_handle(), static_cast<const char*>(buf), static_cast<int>(len), 0);
#else
    ssize_t n = ::send(sock_.native_handle(), buf, len, 0);
#endif
    if (n<0) throw make_syserr("send()");
    return static_cast<size_t>(n);
  }

  size_t recv(void* buf, size_t len){
#if defined(_WIN32)
    int n = ::recv(sock_.native_handle(), static_cast<char*>(buf), static_cast<int>(len), 0);
#else
    ssize_t n = ::recv(sock_.native_handle(), buf, len, 0);
#endif
    if (n<0){
#if defined(_WIN32)
      int e = last_error_code();
      if (e==WSAEWOULDBLOCK) return 0; // non-blocking case compatibility
#else
      if (errno==EAGAIN || errno==EWOULDBLOCK) return 0;
#endif
      throw make_syserr("recv()");
    }
    return static_cast<size_t>(n);
  }

  // send all (blocking loop)
  void send_all(const void* buf, size_t len){
    const char* p = static_cast<const char*>(buf);
    size_t left = len;
    while (left>0){
      size_t n = send(p, left);
      left -= n; p += n;
    }
  }

  void set_timeout(std::chrono::milliseconds ms){ sock_.set_timeout(ms); }
  void set_blocking(bool b){ sock_.set_blocking(b); }
  void set_nodelay(bool b){ sock_.set_nodelay(b); }
  void shutdown_send(){ sock_.shutdown_send(); }

  socket_t native_handle() const { return sock_.native_handle(); }

private:
  Socket sock_;
};

// --- TcpListener: server (bind/listen/accept) ---
class TcpListener {
public:
  TcpListener() = default;

  // bind on host:service (host can be nullptr/empty for ANY)
  static TcpListener bind(const std::string& host, const std::string& service, int backlog=64, bool reuse=true){
    addrinfo hints{};
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE; // for bind

    addrinfo* res=nullptr;
    int rc = ::getaddrinfo(host.empty()? nullptr : host.c_str(), service.c_str(), &hints, &res);
    if (rc!=0) throw std::runtime_error(std::string("getaddrinfo: ")+gai_strerror(rc));

    Socket s;
    for (addrinfo* p=res; p; p=p->ai_next){
      try{
        s = Socket::create(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (reuse){
          int v=1;
          setsockopt(s.native_handle(), SOL_SOCKET, SO_REUSEADDR,
                     reinterpret_cast<const char*>(&v), sizeof(v));
        }
        if (::bind(s.native_handle(), p->ai_addr, static_cast<int>(p->ai_addrlen))<0){
          s.close(); continue;
        }
        if (::listen(s.native_handle(), backlog)<0){
          s.close(); continue;
        }
        freeaddrinfo(res);
        return TcpListener(std::move(s));
      }catch(...){
        s.close(); continue;
      }
    }
    freeaddrinfo(res);
    throw make_syserr("bind/listen");
  }

  TcpStream accept(bool nodelay=true){
    for(;;){
#if defined(_WIN32)
      SOCKET c = ::accept(sock_.native_handle(), nullptr, nullptr);
      if (c==INVALID_SOCKET){ int e=last_error_code(); if (e==WSAEINTR) continue; throw make_syserr("accept()"); }
#else
      int c = ::accept(sock_.native_handle(), nullptr, nullptr);
      if (c<0){ if (errno==EINTR) continue; throw make_syserr("accept()"); }
#endif
      Socket s(c);
      if (nodelay){ try { s.set_nodelay(true); } catch(...){} }
      return TcpStream(std::move(s));
    }
  }

  socket_t native_handle() const { return sock_.native_handle(); }

private:
  explicit TcpListener(Socket&& s) : sock_(std::move(s)) {}
  Socket sock_;
};
