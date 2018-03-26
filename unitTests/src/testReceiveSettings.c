//
// Created by Ruijzendaal on 23/03/2018.
//

#include <unity_fixture.h>
#include "testReceiveSettings.h"

TEST_GROUP(InputfileReceiveSettings);

TEST_SETUP(InputfileReceiveSettings)
{
}

TEST_TEAR_DOWN(InputfileReceiveSettings)
{
}

TEST(InputfileReceiveSettings, testFirst)
{
    TEST_ASSERT(1);
}

TEST_GROUP_RUNNER(InputfileReceiveSettings)
{
    RUN_TEST_CASE(InputfileReceiveSettings, testFirst);

}