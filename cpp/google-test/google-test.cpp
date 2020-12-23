
#include "gtest/gtest.h"

//simple test
int fun()
{
    return 0;
}

TEST(fun, funtest)
{
    EXPECT_EQ(fun(), 0);
}

template <typename E> // E is the element type.
class Queue
{
public:
    Queue();
    void Enqueue(const E &element);
    E *Dequeue(); // Returns NULL if the queue is empty.
    size_t size() const;
};

class QueueTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        q1_.Enqueue(1);
        q2_.Enqueue(2);
        q2_.Enqueue(3);
    }

    // void TearDown() override {}

    Queue<int> q0_;
    Queue<int> q1_;
    Queue<int> q2_;
};


TEST_F(QueueTest, IsEmptyInitially) {
  EXPECT_EQ(q0_.size(), 0);
}

TEST_F(QueueTest, DequeueWorks) {
  int* n = q0_.Dequeue();
  EXPECT_EQ(n, nullptr);

  n = q1_.Dequeue();
  ASSERT_NE(n, nullptr);
  EXPECT_EQ(*n, 1);
  EXPECT_EQ(q1_.size(), 0);
  delete n;

  n = q2_.Dequeue();
  ASSERT_NE(n, nullptr);
  EXPECT_EQ(*n, 2);
  EXPECT_EQ(q2_.size(), 1);
  delete n;
}