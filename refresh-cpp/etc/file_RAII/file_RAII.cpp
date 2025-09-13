#include "gtest/gtest.h"

#include <cstdio>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <utility>

// 설계 및 가독성 편의상 unistd 관련 함수는 ::function으로 표기함

namespace
{
using namespace std;

const auto kTestFileName = "I'm tired today.txt";
void makeTestFile(const string &text)
{
    FILE *f = std::fopen(kTestFileName, "wb");
    if (f)
    {
        std::fwrite(text.data(), 1, text.size(), f);
        std::fclose(f);
    }
}

} // namespace

class FileDescriptorRAIIWrapper
{
public:
    FileDescriptorRAIIWrapper() noexcept = default;
    explicit FileDescriptorRAIIWrapper(int fd) noexcept : fd_(fd) {}

    ~FileDescriptorRAIIWrapper()
    {
        if (fd_ >= 0)
        {
            ::close(fd_);
        }
    }

    FileDescriptorRAIIWrapper(const FileDescriptorRAIIWrapper &) = delete;
    FileDescriptorRAIIWrapper &operator=(const FileDescriptorRAIIWrapper &) = delete;

    /* std::exchange - descriptions from cppReference
    - Replaces the value of obj with new_value and returns the old value of obj.
    - std::exchange can be used when implementing move constructors and, for the members that don't require special cleanup, move assignment operators:
    return an old value가 중요한듯. old value를 return 하기 때문에 생성자에 return std::exchange() 시 주의*/
    FileDescriptorRAIIWrapper(FileDescriptorRAIIWrapper &&other) noexcept : fd_(std::exchange(other.fd_, -1)) {}
    FileDescriptorRAIIWrapper &operator=(FileDescriptorRAIIWrapper &&other) noexcept
    {
        if (this != &other && fd_ >= 0)
        {
            ::close(fd_);
        }
        else
        {
            printf("You're trying to make a same instance.\n");
        }
        fd_ = std::exchange(other.fd_, -1);
        return *this;
    }

    int getFd() { return fd_; };
    explicit operator bool() const noexcept { return fd_ >= 0; };

    int release() noexcept { return std::exchange(fd_, -1); } // 소유권 반환
    void reset(int new_fd = -1) noexcept                      // 초기화
    {
        if (new_fd == fd_)
            return;

        if (fd_ >= 0)
            ::close(fd_);
        fd_ = new_fd;
    }

    static FileDescriptorRAIIWrapper makeInstByReadOnly(const string &path) noexcept
    {
        int fd = ::open(path.c_str(), O_RDONLY);
        return FileDescriptorRAIIWrapper(fd);
    }

    string readAll(FileDescriptorRAIIWrapper &fd_wrapper)
    {
        if (!fd_wrapper)
        {
            return {};
        }

        string ret{};
        ret.reserve(1024);

        char buf[4096];
        ssize_t read_bytes = ::read(fd_wrapper.getFd(), buf, sizeof(buf));
        if (read_bytes > 0)
        {
            ret.append(buf, buf + read_bytes);
        }
        return ret;
    }

private:
    int fd_ = -1;
};

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

TEST_F(TESTFileRAII, CheckMoveSementics)
{
    FileDescriptorRAIIWrapper fd_wrapper_1 = FileDescriptorRAIIWrapper::makeInstByReadOnly(kTestFileName);
    EXPECT_TRUE(fd_wrapper_1);

    FileDescriptorRAIIWrapper fd_wrapper_2 = std::move(fd_wrapper_1);
    EXPECT_FALSE(fd_wrapper_1);
    EXPECT_TRUE(fd_wrapper_2);

    int raw = fd_wrapper_2.release();
    EXPECT_TRUE(raw);
    ::close(raw);
}

TEST_F(TESTFileRAII, ExchangeUsingReset)
{
    FileDescriptorRAIIWrapper fd_wrapper = FileDescriptorRAIIWrapper::makeInstByReadOnly(kTestFileName);

    fd_wrapper.reset(fd_wrapper.release());
    EXPECT_TRUE(fd_wrapper);

    fd_wrapper.reset(FileDescriptorRAIIWrapper::makeInstByReadOnly("no_such_file.txt").release());
    EXPECT_FALSE(fd_wrapper);
}
