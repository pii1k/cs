### Day 3 – 파일 핸들 RAII + 예외 안전성
#### unistd.h에 관하여...
유닉스/리눅스 운영체제에서 프로세스/파일/입출력/시스템 호출을 제어하는 표준 인터페이스 제공
대표적인 함수
- File Descriptor(fd) I/O
    - read, write, close, lseek
- 프로세스 제어
    - fork(프로세스 생성), execv(다른 프로그램 실행), sleep
- 파일/디렉터리 접근
    - access(권한 확인), chdir(현재 작업 디렉토리 변경), getwd(경로 조회)
- 환경 정보
    - sysconf, getlogin, getpid, getppid

---

1. RAII의 근본 개념
RAII(Resource Acquisition Is Initialization)는
C++에서 리소스를 안전하게 관리하기 위한 핵심 설계 원칙

1.1 왜 필요한가?
OS 자원(파일 디스크립터, 소켓, 메모리, mutex 등)은 획득 후 반드시 해제해야 함
C 언어처럼 open / close를 직접 호출하는 방식은 함수 중간에서 예외 발생 시 close가 호출되지 않아 누수(leak) 발생 가능

2. RAII의 핵심 원리
생성자(Constructor) → 리소스 획득 (예: 파일 열기, 소켓 생성, 메모리 할당)
소멸자(Destructor) → 리소스 해제 (예: close, delete, unlock)
스코프(scope)를 벗어날 때 자동 호출 → 해제 코드 호출 누락 방지
예외가 발생해도 스택 언와인딩 과정에서 소멸자는 무조건 호출됨

3. 예외 안전성과 noexcept
3.1 왜 noexcept가 중요한가?
C++ 표준 라이브러리 컨테이너(vector, map 등)는 내부에서 객체를 재배치(reallocate)하거나 소멸할 때 소멸자가 예외를 던지면 프로그램이 std::terminate로 강제 종료
따라서 소멸자/이동 연산자는 반드시 noexcept로 선언해야 안전함

4. 복사 금지, 이동 허용 (Move-Only)
파일 디스크립터, 소켓은 유일한 소유권을 가져야 함
만약 복사가 가능하다면 동일한 핸들을 여러 객체가 닫으려 하면서 double-close 문제가 발생

해결:
복사 생성자/복사 대입 연산자 삭제 (delete)
이동 생성자/이동 대입 연산자는 구현 → 소유권 이전 (std::exchange 활용)

5. 에러 처리 방식
RAII 클래스의 생성자에서 실패할 가능성이 있다면 2가지 선택:
예외를 던진다 — 사용자가 try-catch로 잡아야 함
빈 객체(Invalid state)를 만든다 — operator bool() 등으로 확인

6. RAII의 확장 패턴
UniqueSocket: 소켓 생성/닫기 자동화
UniqueMutex: lock/unlock 자동화 (std::lock_guard 유사)
UniqueMapHandle: OS나 드라이버에서 받은 커널 핸들 해제 자동화
UniqueSharedMemory: mmap/munmap 자동화
-> 등에 적용 가능

7. 예외 안전성 보장 3단계
예외 안전성은 보장 수준에 따라 세 가지로 나뉜다

Basic guarantee
    - 예외가 발생해도 리소스 누수 없음, 프로그램이 유효한 상태 유지
Strong guarantee
    - 예외가 발생하면 연산 전 상태로 되돌아감(트랜잭션 롤백처럼)
No-throw guarantee
    - 예외가 전혀 발생하지 않음(noexcept)

RAII 클래스의 소멸자와 이동 연산자는 No-throw guarantee를 민족해야 함