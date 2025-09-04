RAII TCP stack
    NetInit: (Windows) WSAStartup/WSACleanup RAII
    SocketRAIIWrapper: 저수준 핸들 RAII (move-only)
    TcpListener: 서버 소켓 (bind/listen/accept)
    TcpStream: 연결 소켓 (connect/send/recv/shutdown)

저수준 핸들 RAII
- socket id(handle) 값을 직접 핸들링 해야함
예를들어
- move 시 소유권을 해제하는 객체의 handle id 넘겨줌 + 해당 객체의 핸들 값 초기화 및 소켈 종료

TcpStream
  - connect
  - send
  - receive
  - sendAll
  - setTimeout
  - setBlocking
  - setNoDelay
  - shutdownSend
  - nativeHandle

TcpListener
  - bind
  - accept
  - nativeHandle

=> 이걸로 소유권이 왔다 갔다 하는걸 어떻게 확인 한다는 것인가?

---
나중에 물어볼거
  Socket(Socket&& o) noexcept : s_(o.s_){ o.s_ = invalid_value(); }
    -> 얘는 close 안해줘도 되나?
  fcntl 이 뭔가
  tv.tv_usec = static_cast<long>((ms.count() % 1000) * 1000);
    -> 얘는 무슨 계산식?? 나머지 계산하고 다시 왜 천을 곱해주지?
  어 그것도 궁금함 보통 c memcpy 같은 곳에 size 넣는 곳에 sizeof() -> 이렇게 넣는데 이게 어떤 의미임?? 스택의 사이즈라는거였나? 포인트부터 저 사이즈 만큼 복사하려고 그랬던거 같은데 sizeof랑 요즘 c++에서 제공되는 .size() 함수들 또는 sizeof() * 어떤것을 하는 코드를 자주 봤었는데 이 어떤 것은 뭐야?? 동적할당 할때 인스턴스 갯수인가?