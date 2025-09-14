다음 알고리즘을 순서로 하나씩 구현해볼 예정

1. 알고리즘 설계 기법 (Paradigms)
- 브루트 포스 (Brute Force)
    - 모든 경우의 수를 전부 탐색 → 완전탐색
- 분할 정복 (Divide and Conquer)
    - 문제를 쪼개고 합치는 방식 (예: 퀵 정렬, 병합 정렬)
- 탐욕법 (Greedy Method)
    - 순간순간 최적 선택 (예: Kruskal, Prim)
- 동적 계획법 (Dynamic Programming, DP)
    - 부분 문제의 결과를 저장·재활용 (예: 피보나치, LCS)
- 백트래킹 (Backtracking)
    - 해 탐색 중 조건 불만족 시 되돌아감 (예: N-Queen)
- 분지 한정 (Branch and Bound)
    - 최적화 문제에서 탐색 공간을 줄임 (예: TSP 근사)
- 무작위화 기법 (Randomized Algorithms)
    - 알고리즘 수행에 확률적 요소 사용 (예: 랜덤 퀵정렬)

2. 기본 알고리즘 (Fundamental Algorithms)
- 정렬 (Sorting)
    - ✅ 단순 정렬: 삽입, 버블
    - 고급 정렬: 퀵, 병합, 힙 정렬
    - 특수 정렬: 기수 정렬, 계수 정렬
- 탐색 (Searching)
    - ✅ 선형 탐색
    - ✅ 이진 탐색

3. 그래프 알고리즘 (Graph Algorithms)
- 탐색
    - DFS, BFS
- 최단 경로
    - 다익스트라, 벨만-포드, 플로이드-워셜
- 최소 신장 트리
    - Kruskal, Prim
- 위상 정렬
    - DAG(Directed Acyclic Graph) 정렬
- 흐름 문제 (Network Flow)
    - Ford-Fulkerson, Edmonds-Karp, Dinic
- 강결합 컴포넌트 / 사이클 탐지
    - Kosaraju, Tarjan

4. 문자열 알고리즘 (String Algorithms)
- 패턴 매칭
    - 단순 패턴 매칭
    - KMP 알고리즘
    - Rabin-Karp (해시 기반)
- 접두사/접미사 기반
    - Z 알고리즘
    - 접미사 배열 (Suffix Array)
    - 접미사 자동자 (Suffix Automaton)
- 자료구조 활용
    - Trie
    - Aho–Corasick 자동자

5. 수학적 알고리즘 (Mathematical Algorithms)
- 정수론
    - 최대공약수/최소공배수 (유클리드 호제법)
    - 소수 판정, 소수 생성 (에라토스테네스의 체)
    - 모듈러 연산, 빠른 거듭제곱
- 조합론 / 확률
    - 이항 계수, 파스칼 삼각형
    - 확률적 시뮬레이션
- 수치 해석
    - 뉴턴-랩슨 방법
    - 선형 방정식 풀이 (가우스 소거법)
- 다항식 / 신호 처리
    - FFT (Fast Fourier Transform)
    - 다항식 곱셈

6. 최적화 알고리즘 (Optimization Algorithms)
- 이분 탐색 (Binary Search on Answer)
- 매칭/네트워크 최적화
    - 이분 매칭 (Hungarian Algorithm)
    - 최대 유량-최소 컷 정리
- 근사 알고리즘 (Approximation Algorithms)
    - NP-Hard 문제에 대해 근사해 제공
- 메타휴리스틱
    - 유전 알고리즘
    - 시뮬레이티드 어닐링

<!-- 7. 고급 알고리즘 (Advanced Topics)
- 현대 CS/AI/데이터 과학에서 자주 다룸.
- 데이터 압축
- 허프만 코딩
- LZ77, LZ78
- 암호학
- RSA, ECC
- 해시 함수
- 머신러닝 기초
- 선형 회귀, 로지스틱 회귀
- 결정 트리, 랜덤 포레스트
- 신경망, 강화학습
- 병렬/분산 알고리즘
- MapReduce
- 분산 합의 알고리즘 (Paxos, Raft) -->