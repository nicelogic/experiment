
#include "gtest/gtest.h"

int fun()
{
  return 0;
}

TEST(fun, funtest) {
  EXPECT_EQ(fun(), 0);
}