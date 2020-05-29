#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <gtest/gtest.h>

//#include "../common/myflow.h"

#include "../../src/common/instance.h"
#include "../../src/common/solution.h"

using namespace testing;

class TestCommon: public Test{
protected:

public:
    TestCommon();
    void SetUp();
    void TearDown();
    ~TestCommon();
};

#endif // TEST_COMMON_H
