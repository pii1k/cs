### unique pointer의 get()
- unique pointer가 소유한 객체의 원시 포인터를 돌려줌
- 소유권은 넘어가지 않음(읽기 전용)

### smart_pointer.cpp에 관하여...
해당 예제는 보편적인 unique_ptr의 사용법과 달리 C 리소스/OS를 핸들링하는 것이라 make_unique를 사용하면 안된다. 
make_unique는 new/delete용이고 FILE*과 같은 OS 리소스는 닫는 방법이 다르기 때문이다.

조금 더 자세히 설명하면 다음과 같다.
- make_unique<T>(args...)는 힙에 new T(args...)로 객체를 만든 뒤 그 포인터를 unique_ptr<T>로 감싸는 역할을 한다.
- 하지만 FILE*는 new FILE을 만드는 것이 아니라 fopen()으로 소유권을 얻고, 해제도 delete가 아닌 fclose()로 해야한다. 
- FILE*과 같은 C 리소스/OS 핸들에는 다음과 같은 종류가 있다.
    - FILE*, DIR*, CURL*, sqlite3* ...
    - 커스텀 deleter가 달린 unique_ptr로 감싸줘야 제대로 리소스 관리가 가능하다.

### unique_ptr로 C 리소스 관리하는 표준 패턴
1. 함수 포인터 deleter 사용 (간단, 약간 큰 사이즈)
```cpp
using file_ptr = std::unique_ptr<FILE, int(*)(FILE*)>;
file_ptr f(std::fopen("x.txt", "w", &std::fclose);
if(!f) throw std::runtime_error("open failed");

std::fputs("hello", f.get());
// 스코프 종료 시 자동으로 fclose 호출
```
- deleter 함수 포인터가 저장되어 unique_ptr 크기가 포인터 하나 만큼 커지는 단점이 있음

2. functor deleter 사용
```cpp
struct FileCloser
{
    void operator()(FILE* fp) const noexcept
    {
        if (fp) std::fclose(fp);
    }
}

using file_ptr = std::unique_ptr<FILE, FileCloser>; 
file_ptr f(std::fopen("x.txt", "w));
```
- FileCloser가 무상태 타입이면 EBO(Empty Base Optimization) 덕에 unique_ptr의 크기가 raw 포인터와 거의 동일 (컴파일러 최적화)

### 그래도 make_unique를 쓰고 싶다면.. 래퍼 클래스를 만들면 된다
```cpp
class FileWrapper {
public:
    FileWrapper(const char* path, const char* mode) 
    {
        fp_ = std::fopen(path, mode);
        if (!fp_) throw std::runtime_error("open failed");
    } 
    
    ~FileWrapper() 
    {
        if (fp_) std::fclose(fp_); 
    }

    FILE* native() const noexcept { return fp_; }

    // no copy
    FileWrapper(const FileWrapper&) = delete;
    FileWrapper& operator=(const FileWrapper&) = delete;
    
    // yes move
    FileWrapper(FileWrapper&& o) noexcept :
        fp_(std::exchange(o.fp_, nullptr)) {}
    FileWrapper& operator=(FileWrapper&& o) noexcept 
    {
        if (this != &o) 
        { 
            if (fp_) std::fclose(fp_); 
            fp_ = std::exchange(o.fp_, nullptr); 
        }
        return *this;
    }

private:
  FILE* fp_{nullptr};
};

auto f = std::make_unique<FileWrapper>("x.txt","w"); 
std::fputs("hi\n", f->native());
```

### shared_ptr 순환 참조와 weak_ptr 끊기
내부에서 무슨 일이 일어나는가
- shared_ptr<T>는 컨트롤 블럭을 가진다
    - string(use_count): 실제 소유자 개수
    - weak(weak_count): 비소유 참조 개수
- use_count가 0이 되면 T의 소멸자가 호출되고 메모리가 해제됨
- weak_count는 컨트롤 블록만 붙잡고 객체는 소유하지 않음

### weak_ptr는 언제 사용할까
부모–자식: 부모→자식 shared_ptr, 자식→부모 weak_ptr
캐시/옵저버 패턴: 강한 소유권은 다른 곳에 있고, 나는 살아있으면 쓰고 아니면 말아야 하는 참조가 필요할 때