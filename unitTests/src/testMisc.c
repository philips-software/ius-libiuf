//
// Created by Ruijzendaal on 20/03/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/iusTypes.h>
#include <include/iusUtil.h>
#include <include/ius.h>


TEST_GROUP(Misc);

TEST_SETUP(Misc)
{
}

TEST_TEAR_DOWN(Misc)
{
}


TEST(Misc, testVersion)
{
    TEST_ASSERT_EQUAL(3, iusGetVersionMajor());
}


TEST(Misc,  testCompareFloats)
{
    IUS_BOOL isEqual ;
    float a = -0.0000000000000000000000000000013025967;
    float b = -0.00000000000000000000000000000130261024;

    isEqual = IUS_EQUAL_FLOAT(a,b);
    TEST_ASSERT( isEqual == IUS_FALSE );

    a=0.100000001490116119384765625;
    b=0.100000001490116119384765625;
    isEqual = IUS_EQUAL_FLOAT(a,b);
    TEST_ASSERT( isEqual == IUS_TRUE );

    a=0.100000001490116119384765625;
    b=a-(FLOAT_PRECISION*a);
    isEqual = IUS_EQUAL_FLOAT(a,b);
    TEST_ASSERT( isEqual == IUS_TRUE );

    a=0.100000001490116119384765625;
    b=a-(FLOAT_PRECISION*a*2);
    isEqual = IUS_EQUAL_FLOAT(a,b);
    TEST_ASSERT( isEqual == IUS_FALSE);

    a=0.0000000000000000000813826815;
    b=0.0000000000000000000813836121;
//    TEST_ASSERT_EQUAL_FLOAT(a,b);
    isEqual = IUS_EQUAL_FLOAT(a,b);
    TEST_ASSERT( isEqual == IUS_TRUE);
}

TEST_GROUP_RUNNER(Misc)
{
    RUN_TEST_CASE(Misc, testVersion);
//    RUN_TEST_CASE(Misc, testCompareFloats);
}
