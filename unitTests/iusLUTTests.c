#include "unity\unity.h"
#include "iusLUT.h"

//------------------------------------------------------------------------------
// FloatIntervalLUTTest
//------------------------------------------------------------------------------
void FloatIntervalLUTTest(void)
{
    iusFloatIntervalLUT * pLUT;
    float pX[]     = {0.5f, 1.0f, 2.0f, -1.0f};  // The LUT floating point x values
    float pY[]     = {5.0f, 4.0f, 3.0f, 2.0f };             // The LUT integer y values
    int numEntries = (int)(sizeof(pX) / sizeof(float)); // nr LUT entries

    float pI[] = {-0.1f, 0.4f, 0.5f, 1.5f, 2.0f, 10.0f}; // several input values to test the LUT
    int   pA[6];                                         // the actual output values
    int   pE[] = {5, 5, 4, 3, 2, 2};                     // the expected output values
    int   i;
    
    pLUT = iusAllocFloatIntervalLUT( numEntries );
    iusFillFloatIntervalLUT( pLUT, pX, pY, numEntries );
    for (i = 0; i < 6; i++)
    {
        pA[i] = (int)iusLookUpFloatIntervalLUT( pLUT, pI[i] );
    }
    iusFreeFloatIntervalLUT( pLUT );

    TEST_ASSERT_EQUAL_FLOAT_ARRAY( pE, pA, 6 );
}
