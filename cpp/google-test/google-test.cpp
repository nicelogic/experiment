
#include "gtest/gtest.h"
#include <iostream>

using namespace std;

bool fun(int ival)
{
    return false;
}

TEST(fun, funtest)
{
    SUCCEED();
}

class BaseTestFixture : public testing::Test
{
protected:
    void SetUp() override
    {
        cout << "test base fixture" << endl;
    }
};

class DerivedTestFixture : public BaseTestFixture
{
protected:
    void SetUp() override
    {
        cout << "derived fixture" << endl;
    }
};

TEST_F(DerivedTestFixture, JustTest)
{
    SUCCEED();
}