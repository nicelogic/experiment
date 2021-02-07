
#include "gtest/gtest.h"
#include <iostream>

using namespace std;

//base fun test
bool fun(int ival)
{
    return false;
}

TEST(fun, funtest)
{
    SUCCEED();
}

//mock class

class SimpleClass
{
public:
    virtual int getIval(const int ival = 0)
    {
        return ival;
    }

};

class MockSimpleClass
{
public:
    // MOCK_ME
};

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