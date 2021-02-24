
#include "gtest/gtest.h"
#include "gmock/gmock.h"
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

class Foo
{
protected:
    virtual void Resume();
};

class MockFoo : public Foo
{
public:
     MOCK_METHOD(void, Resume, (), (override));
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