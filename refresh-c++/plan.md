### 환경에 대해 간단한 소개
CS를 직접 구현해보면서 좀 더 깊게 공부하기 위한 프로젝트
가장 익숙한 환경으로 프로젝트 구축했음
    - 의존성은 cmake로, 공부할 타겟 하나 잡고 클래스 만든 뒤에 같은 폴더에 테스트도 추가하는 방식으로 진행하고 있음
    - 아직 타겟들이 다 작고 소스끼리 의존성이 없어서 전부 프리펑션으로 작성했음
        - 추후에 필요하다 싶으면 만들 예정
    
---

#### 목표 및 일지
완료: 프로젝트 설정과 간단한 실험들 및 shared ptr, RAII 등 기본적인 내용 골라서 진행했음
이제 본격적으로 기본 CS 지식 공부

목표: 자료구조 구현 설계 및 테스트
    - 선형 자료구조
    - 스택/큐/덱
    - 힙/우선순위 큐
    - 해시맵/세트
    - 트리
    - Union-Find
    - 세그먼트 트리 / 펜윅트리
    - LRU 캐시

---

### DONE

<!-- 목표를 좀 더 장기적으로 세분화 하여 나누기로 결정
앞으로 이번주 4일동안 할 것들 아래와 같음
3달 정도 계획: C++ 심화, CS 기초, 시스템, 성능 기초
1. RAII & 예외 안전성
    - 파일/소켓/mutex RAII 래퍼 구현
2. 스마트 포인터 심화
    - unique/shared/weak 내부 구조 분석
3. Move/Forwarding
    - move/emplace 실험 및 분석
4. 컨테이너 구조
    - vector/list/map 삽입/탐색 성능 측정

    1. 파일 RAII (UniqueFd) → 가장 단순, 패턴 확정
    2. 소켓 RAII (UniqueSocket) → 동일 패턴 확장 + 오류/타임아웃 처리
    3. mutex 사용 패턴 → std::lock_guard / unique_lock로 멀티스레드 안정화 테스트
    4. CountingAlloc(카운팅 할당자) → 컨테이너 할당 관찰 지표 확보
    5. 고정 풀 Allocator(간단 pool) → 실제 최적화 효과 확인 -->

---

<!-- <Week 1. C++ 핵심 리프레시 & GitHub 세팅>
Day 1
 스마트 포인터 & 순환 참조 방지
 unique/shared/weak + 순환 참조 예제 2종 (good/bad)
 leak counter & cycle break
 Day 2
 STL 컨테이너 성능 분석 + Move Semantics
 vector vs list 삽입/삭제 성능 비교
 push_back vs emplace_back + move ctor 성능 측정
 ~BSP·IoT 장비 최적화~ -> 장비가 없어서 패스
Day 3
 RAII & 예외 안전성
 파일/소켓/mutex RAII 래퍼 3종
 noexcept와 예외 안전 설계 장비 드라이버, 커널 리소스 관리
Day 4
 Memory Pool & Allocator
 고정 크기 메모리 풀 구현
 STL custom allocator 적용 임베디드 메모리 제한 환경 대응
Day 5
 커널 모듈·BSP 감각 익히기
 QEMU에 커널 모듈 빌드 & insmod/rmmod 실습
 Device Tree 수정 mini 실습 BSP 통합 역량
Day 6
 네트워크·IPC
 TCP/UDP echo server/client
 gdb attach로 네트워크 중 디버깅 IoT 장비 통신 디버깅
Day 7
 Logger + Memory Pool + 네트워크 통합 미니 프로젝트

#### 변경 포인트
 스마트 포인터/순환 참조 → Day 1에서 바로 깊게 (이미 했던 내용 포함)
 Move Semantics → Day 2에 STL 성능 비교랑 같이 묶어 바로 실습
 Memory Pool/Allocator → Qualcomm에서 메모리 제약 환경 경험 강조할 수 있게 Day 4에 당김
 BSP/커널 모듈 → Week 1에 미리 맛보고, Week 3~4에서 Yocto로 확장 -->
<!--
<Week 1. C++ 핵심 리프레시 & GitHub 세팅>
Day 1
 GitHub repo qualcomm_prep 생성 (README에 목표 작성)
 C++ 스마트 포인터(unique_ptr, shared_ptr) 기본 예제 3개 작성
 프로그래머스 Lv3 1문제 풀이 (간단한 STL 활용 문제)
 오늘 학습 노트 week1_cpp/day1.md 업로드
Day 2
 STL 컨테이너 vector vs list 삽입/삭제 성능 비교 코드 작성
 성능 측정(chrono) 결과를 표로 정리
 프로그래머스 Lv3 1문제 풀이
 오늘 성능 비교 결과 리포트 stl_benchmark.md 업로드
Day 3
 RAII(Resource Acquisition Is Initialization) 개념 학습
 파일 핸들 관리 클래스를 RAII로 구현
 Logger 클래스 기본 구조 설계 (std::ofstream 이용)
 Logger 기초 버전 코드 logger_basic.cpp 업로드
Day 4
 Move semantics와 rvalue reference 실습 (std::move 예제)
 push_back vs emplace_back 성능 비교
 프로그래머스 Lv3 1문제 풀이
 결과 리포트 move_semantics.md 업로드
Day 5
 Memory Pool 기본 개념 학습
 간단한 고정 크기 Memory Pool 클래스 구현
 Logger에 Memory Pool 적용해 메모리 할당 감소 테스트
 테스트 결과 리포트 memory_pool_test.md 업로드
Day 6
 예외 처리와 noexcept 사용 시 장단점 학습
 기존 Memory Pool에 예외 안전성 적용
 프로그래머스 Lv3 1문제 풀이
 수정된 코드와 리포트 exception_safety.md 업로드
Day 7
 이번 주 학습 내용 정리 (Markdown 1~2p)
 Logger + Memory Pool 통합 버전 완성
 모의 C++ 기초 면접 5문제 작성 및 답변 준비
 GitHub repo 정리/README 업데이트

<첫 주 목표>
GitHub repo에 최소 코드 6개 + 리포트 4개 업로드
C++ 기초 및 메모리 관리 → 실무형 예제까지 연결
8주 플랜을 위한 작동하는 워크플로 구축 -->