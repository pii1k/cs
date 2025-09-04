#include "gtest/gtest.h"

#include <algorithm>
#include <any>
#include <cstdio>
#include <functional>
#include <memory>
#include <stdexcept>

namespace
{
using namespace std;
static bool kfclosedCalled = false;
void fcloseCustom(FILE *f)
{
    kfclosedCalled = true;
    std::fclose(f);
}

static bool kIsClassDestructed = false;
} // namespace

// 1. unique_ptr 소유권 이전 + 커스텀 deleter
class TestUniquePtr : public testing::Test
{
public:
    struct FileCloser
    {
        void operator()(FILE *f) const
        {
            if (f)
                fcloseCustom(f);
        }
    };

    unique_ptr<FILE, FileCloser> open_file(const char *path, const char *mode)
    {
        FILE *f = fopen(path, mode);
        if (!f)
            throw runtime_error("Example 1 failed! Failed to open file");
        return unique_ptr<FILE, FileCloser>(f);
    }

protected:
    void TearDown() override
    {
        kfclosedCalled = false;
    }
};

TEST_F(TestUniquePtr, CustomDeleter)
{
    ASSERT_NO_THROW({
        auto file = open_file("smartptr_demo.txt", "w");
        const char *input_msg = "hello smart ptr\n";
        std::fwrite(input_msg, 1, std::strlen(input_msg), file.get());
    });

    EXPECT_TRUE(kfclosedCalled);
}

// 2. shared 순환 참조 + weak_ptr
class TestSharedPtr : public testing::Test
{
public:
    struct LeakTrack
    {
        static inline atomic<int> alive = 0;
        LeakTrack() { ++alive; }
        virtual ~LeakTrack() noexcept { --alive; }
    };

    struct BadNode : LeakTrack, enable_shared_from_this<BadNode>
    {
        explicit BadNode(string name) : name_(std::move(name)) {}
        ~BadNode() noexcept { kIsClassDestructed = true; }

        string name_;
        shared_ptr<BadNode> parent_;
        vector<shared_ptr<BadNode>> children_;
    };

    struct Node : LeakTrack, enable_shared_from_this<Node>
    {
        explicit Node(string name) : name_(std::move(name)) {}
        ~Node() noexcept { kIsClassDestructed = true; }

        void add_child(const shared_ptr<Node> &child)
        {
            child->parent_ = shared_from_this();
            children_.push_back(child);
        };

        string name_;
        weak_ptr<Node> parent_;
        vector<shared_ptr<Node>> children_;
    };

    struct CallbackEvent : LeakTrack
    {
        void on(function<void()> func)
        {
            functions_.push_back(std::move(func));
        }

        void emit_all()
        {
            for (auto &f : functions_)
            {
                f();
            }
        }

        vector<function<void()>> functions_{};
    };

    struct BadActor : LeakTrack, enable_shared_from_this<BadActor>
    {
        explicit BadActor(string name, CallbackEvent &event) : name_(std::move(name)),
                                                               event_(event) {}

        ~BadActor() noexcept { kIsClassDestructed = true; }

        void badSubscribe()
        {
            auto self = shared_from_this();
            event_.on([self]() {});
        };

        string name_;
        CallbackEvent &event_;
    };

    struct NiceActor : LeakTrack, enable_shared_from_this<NiceActor>
    {
        explicit NiceActor(string name, CallbackEvent &event) : name_(std::move(name)),
                                                                event_(event) {}

        ~NiceActor() noexcept { kIsClassDestructed = true; }

        void niceSubscribe()
        {
            weak_ptr<NiceActor> actor_wp = shared_from_this();
            event_.on([actor_wp]()
                      {
                if(auto actor_sp = actor_wp.lock())
                {
                    (void) actor_sp; // 권한 받아서 사용
                } });
        };

        string name_;
        CallbackEvent &event_;
    };

protected:
    void SetUp() override
    {
        kIsClassDestructed = false;
    }

    void TearDown() override
    {
        LeakTrack::alive = 0;
        kIsClassDestructed = false;
    }
};

TEST_F(TestSharedPtr, CycleReference)
{
    {
        auto a = make_shared<BadNode>("a");
        EXPECT_EQ(LeakTrack::alive.load(), 1);

        auto b = make_shared<BadNode>("b");
        EXPECT_EQ(LeakTrack::alive.load(), 2);

        // stong <-> storng cycle 형성
        a->children_.push_back(b);
        b->parent_ = a;
    }
    EXPECT_NE(LeakTrack::alive.load(), 0);
}

TEST_F(TestSharedPtr, FixCycleReferenceUsingWeakPtr)
{
    // case 1`
    {
        auto a = make_shared<Node>("a");
        EXPECT_EQ(LeakTrack::alive.load(), 1);

        auto b = make_shared<Node>("b");
        EXPECT_EQ(LeakTrack::alive.load(), 2);

        a->children_.push_back(b);
        b->parent_ = a;
        EXPECT_NE(LeakTrack::alive.load(), 0);
    }
    EXPECT_EQ(LeakTrack::alive.load(), 0);

    // case 2
    {
        auto root = make_shared<Node>("root");
        auto a = make_shared<Node>("a");
        auto b = make_shared<Node>("b");
        root->add_child(a);
        root->add_child(b);

        string parent_name_buf{};
        if (auto p = a->parent_.lock())
        {
            parent_name_buf = p->name_;
        }
        EXPECT_EQ(parent_name_buf, "root");
        EXPECT_NE(LeakTrack::alive.load(), 0);
    }
    EXPECT_EQ(LeakTrack::alive.load(), 0);
}

// 사이클 형성 (event가 콜백을 들고, 콜백이 self를 strong 캡처)
TEST_F(TestSharedPtr, CallbackCycleReference)
{
    CallbackEvent event;

    // case 1
    {
        auto bad_actor = make_shared<BadActor>("batman", event);
        bad_actor->badSubscribe();
        event.emit_all();
    }
    EXPECT_FALSE(kIsClassDestructed);

    // cse 2
    {
        auto nice_actor = make_shared<NiceActor>("spiderman", event);
        nice_actor->niceSubscribe();
        event.emit_all();
    }
    EXPECT_TRUE(kIsClassDestructed);
}