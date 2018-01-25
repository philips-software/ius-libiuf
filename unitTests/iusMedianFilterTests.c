#include "Unity\unity.h"
#include "iusMedianFilter.h"

//------------------------------------------------------------------------------
// MedianFilterTest
//------------------------------------------------------------------------------
void MedianFilterTest(void)
{
    IUSMedianFilterInstance * pInst;
    const float pInput[]          = { 1, 3, 7, 13, 19, 23, 31, 37, 29, 21, 17, 11, 5, 2 };
    const float pExpectedOutput[] = { 3, 3, 7, 13, 19, 23, 29, 29, 29, 21, 17, 11, 5, 5 };
    float       pActualOutput[14];
    int         windowWidth = 5;
    int         i;

    pInst = iusMedianFilterCreate( 14, windowWidth);
    iusMedianFilterRun(pInst, pActualOutput, pInput, 14);
    iusMedianFilterDestroy(pInst);

    for (i = 0; i < 14; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(pExpectedOutput[i], pActualOutput[i]);
    }
}
