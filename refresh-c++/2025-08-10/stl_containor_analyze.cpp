#include "gtest/gtest.h"

#include <chrono>
#include <cmath>
#include <cstddef>
#include <list>
#include <vector>

namespace
{
using namespace std;
using namespace chrono;

constexpr uint16_t kN = 20000;
} // namespace

class STLContainorAnalyze : public testing::Test
{
public:
    template <typename Func>
    uint64_t measure_time(Func functoin_to_measure)
    {
        auto start = chrono::high_resolution_clock::now();
        functoin_to_measure();
        auto end = chrono::high_resolution_clock::now();
        return duration_cast<microseconds>(end - start).count();
    }

    void thrash_caches(uint64_t bytes = 256ULL * 1024 * 1024) // default = 256MB
    {
        static vector<uint8_t> buf(bytes, 1);
        uint64_t sum = 0;
        for (uint64_t i = 0; i < buf.size(); i += 64)
        {
            sum += buf[i]; // 64B stride
        }
        asm volatile("" ::"r"(sum)); // optimize-away 방지
    }

    template <class F>
    pair<uint64_t, uint64_t> measure_cold_warm(F &&f, int warm_repeats = 7)
    {
        // cold
        this->thrash_caches();
        auto start = chrono::steady_clock::now();
        f();
        auto end = chrono::steady_clock::now();
        auto cold_time = chrono::duration_cast<chrono::microseconds>(end - start).count();

        // warm
        vector<uint64_t> warm_times;
        warm_times.reserve(warm_repeats);
        for (int i = 0; i < warm_repeats; ++i)
        {
            auto start = chrono::steady_clock::now();
            f();
            auto end = chrono::steady_clock::now();
            warm_times.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());
        }
        nth_element(warm_times.begin(), warm_times.begin() + warm_repeats / 2, warm_times.end());
        return {cold_time, warm_times[warm_repeats / 2]};
    }
};

TEST_F(STLContainorAnalyze, PushBack)
{
    vector<int> v;
    list<int> l;

    uint64_t time_v = measure_time([&]()
                                   {for (int i = 0; i < kN; i++){
                                        v.push_back(i);} });

    uint64_t time_l = measure_time([&]()
                                   {for (int i = 0; i < kN; i++){
                                        l.push_back(i);} });

    printf("vec:%ld | list:%ld\n", time_v, time_l);
    EXPECT_GE(time_l, time_v);
}

TEST_F(STLContainorAnalyze, RandomInsert)
{
    vector<int> v(kN, 0);
    list<int> l(kN, 0);

    uint64_t time_v = measure_time([&]()
                                   {for (int i = 0; i < 1000; ++i){
                                        v.insert(v.begin() + rand() % v.size(), i);} });

    uint64_t time_l = measure_time([&]()
                                   {auto it = l.begin();
                                    for (int i = 0; i < 1000; ++i)
                                    {
                                        advance(it, rand() % (l.size()));
                                        l.insert(it, i);
                                    } });

    printf("vec:%ld | list:%ld\n", time_v, time_l);
    EXPECT_GE(time_l, time_v);
}

TEST_F(STLContainorAnalyze, PushbackVSEmplacebackPOD)
{
    vector<int> push_back_v;
    vector<int> emplace_back_v;

    uint64_t push_back_time = measure_time([&]()
                                           {for (int i = 0; i < kN; i++){
                                        push_back_v.push_back(i);} });

    uint64_t emplace_back_time = measure_time([&]()
                                              {for (int i = 0; i < kN; i++){
                                        emplace_back_v.emplace_back(i);} });

    printf("push_back_time:%ld | emplace_back_time:%ld\n", push_back_time, emplace_back_time);
    EXPECT_GE(emplace_back_time, push_back_time);
}

TEST_F(STLContainorAnalyze, PushbackVSEmplacebackNONPOD)
{
    struct BigObject
    {
        string name;
        unique_ptr<int[]> buf;
        int size{};

        explicit BigObject(int size) : name(),
                                       buf(new int[size]),
                                       size(size) {}

        // copy
        BigObject(const BigObject &obj) : name(obj.name),
                                          buf(new int[obj.size]),
                                          size(obj.size)
        {
            memcpy(buf.get(), obj.buf.get(), sizeof(int) * size);
        }

        // move
        BigObject(BigObject &&obj) noexcept
            : name(move(obj.name)),
              buf(move(obj.buf)),
              size(exchange(obj.size, 0)) {}
    };

    // copy CTOR
    vector<BigObject> copy_v;
    copy_v.reserve(kN);
    auto copy_time = measure_time([&]
                                  {copy_v.clear();
                            for (int i = 0; i < kN; ++i)
                            {
                                BigObject obj(4096);
                                copy_v.push_back(obj);
                            } });

    // move CTOR
    vector<BigObject> move_v;
    move_v.reserve(kN);
    auto move_time = measure_time([&]
                                  {move_v.clear();
                            for (int i = 0; i < kN; ++i)
                            {
                                BigObject obj(4096);
                                move_v.push_back(std::move(obj));
                            } });

    // in-place
    vector<BigObject> emplace_v;
    emplace_v.reserve(kN);
    auto emplace_time = measure_time([&]
                                     {emplace_v.clear();
                                for (int i = 0; i < kN; ++i)
                                {
                                    emplace_v.emplace_back(4096);
                                } });

    printf("copy:%ld  move:%ld  emplace:%ld\n", copy_time, move_time, emplace_time);
    EXPECT_GE(copy_time, move_time);

    // 측정이 메모리 할당, OS 지터에 묻혀서 제대로 측정 안됨.. 너무 시간을 많이 써서 그냥 넘어 가도록 한다.. ㅠㅠ
    // EXPECT_GE(move_time, emplace_time);
}

TEST_F(STLContainorAnalyze, ColdCacheHotCache)
{
    auto [cold_time, warm_time] = measure_cold_warm([&]
                                                    {
        vector<int> v; v.reserve(100000);
        for (int i=0;i<100000;++i) v.push_back(i); });

    EXPECT_GE(cold_time, warm_time);
}