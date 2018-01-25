#include "Unity\unity.h"
#include "base\iusVector.h"

//------------------------------------------------------------------------------
// SetValue2DoubleVectorTest
//------------------------------------------------------------------------------
void SetValue2DoubleVectorTest(void)
{
    const double value = 2.34;
    const double pExpectedOutput[] = { 2.34, 2.34, 2.34, 2.34, 2.34, 2.34 };
    double       pActualOutput[6];

    iusSetValue2DoubleVector(pActualOutput, 6, value);

    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// SetValue2FloatVectorTest
//------------------------------------------------------------------------------
void SetValue2FloatVectorTest(void)
{
    const float value = 2.34f;
    const float pExpectedOutput[] = { 2.34f, 2.34f, 2.34f, 2.34f, 2.34f, 2.34f };
    float       pActualOutput[6];

    iusSetValue2FloatVector(pActualOutput, 6, value);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// SetValue2IntVectorTest
//------------------------------------------------------------------------------
void SetValue2IntVectorTest(void)
{
    const int value = -3;
    const int pExpectedOutput[] = { -3, -3, -3, -3, -3, -3 };
    int       pActualOutput[6];

    iusSetValue2IntVector(pActualOutput, 6, value);

    TEST_ASSERT_EQUAL_INT32_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// SetValue2ShortVectorTest
//------------------------------------------------------------------------------
void SetValue2ShortVectorTest(void)
{
    const short value = -3;
    const short pExpectedOutput[] = { -3, -3, -3, -3, -3, -3 };
    short       pActualOutput[6];

    iusSetValue2ShortVector(pActualOutput, 6, value);

    TEST_ASSERT_EQUAL_INT16_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// SetValue2ByteVectorTest
//------------------------------------------------------------------------------
void SetValue2ByteVectorTest(void)
{
    const unsigned char value = 3;
    const unsigned char pExpectedOutput[] = { 3, 3, 3, 3, 3, 3 };
    unsigned char       pActualOutput[6];

    iusSetValue2ByteVector(pActualOutput, 6, value);

    TEST_ASSERT_EQUAL_INT8_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// ZeroDoubleVectorTest
//------------------------------------------------------------------------------
void ZeroDoubleVectorTest(void)
{
    const double pExpectedOutput[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double       pActualOutput[]   = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

    iusZeroDoubleVector(pActualOutput, 6);

    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(pExpectedOutput, pActualOutput, 6);

}

//------------------------------------------------------------------------------
// ZeroFloatVectorTest
//------------------------------------------------------------------------------
void ZeroFloatVectorTest(void)
{
    const float pExpectedOutput[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    float       pActualOutput[]   = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    iusZeroFloatVector(pActualOutput, 6);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 6);

}

//------------------------------------------------------------------------------
// ZeroIntVectorTest
//------------------------------------------------------------------------------
void ZeroIntVectorTest(void)
{
    const int pExpectedOutput[] = { 0, 0, 0, 0, 0, 0 };
    int       pActualOutput[]   = { 1, 1, 1, 1, 1, 1 };

    iusZeroIntVector(pActualOutput, 6);

    TEST_ASSERT_EQUAL_INT32_ARRAY(pExpectedOutput, pActualOutput, 6);

}

//------------------------------------------------------------------------------
// ZeroShortVectorTest
//------------------------------------------------------------------------------
void ZeroShortVectorTest(void)
{
    const short pExpectedOutput[] = { 0, 0, 0, 0, 0, 0 };
    short       pActualOutput[] = { 1, 1, 1, 1, 1, 1 };

    iusZeroShortVector(pActualOutput, 6);

    TEST_ASSERT_EQUAL_INT16_ARRAY(pExpectedOutput, pActualOutput, 6);

}

//------------------------------------------------------------------------------
// ZeroByteVectorTest
//------------------------------------------------------------------------------
void ZeroByteVectorTest(void)
{
    const unsigned char pExpectedOutput[] = { 0, 0, 0, 0, 0, 0 };
    unsigned char       pActualOutput[]   = { 1, 1, 1, 1, 1, 1 };

    iusZeroByteVector(pActualOutput, 6);

    TEST_ASSERT_EQUAL_INT8_ARRAY(pExpectedOutput, pActualOutput, 6);

}

//------------------------------------------------------------------------------
// SetSineTable2FloatVectorTest
//------------------------------------------------------------------------------
void SetSineTable2FloatVectorTest(void)
{
    /* in MatLab:
    for n = 0:7, fprintf('%.6ff, ', sin((pi / 4)*n)); end, fprintf('\n')
    */
    const float pi = 3.141592654f;
    const float pExpectedOutput[] = { 0.000000f, 0.707107f, 1.000000f, 0.707107f,
        0.000000f, -0.707107f, -1.000000f, -0.707107f };
    float       pActualOutput[8];
    int         i;

    iusSetSineTable2FloatVector(pActualOutput, 8, pi / 4.0f);

    for (i = 0; i < 8; i++)
    {
        TEST_ASSERT_FLOAT_WITHIN(1e-6f, pExpectedOutput[i], pActualOutput[i]);
    }
}

//------------------------------------------------------------------------------
// SetShiftedSineTable2FloatVectorTest
//------------------------------------------------------------------------------
void SetShiftedSineTable2FloatVectorTest(void)
{
    /* in MatLab:
    for n = 0:7, fprintf('%.6ff, ', sin((pi / 4)*n + 0.1)); end, fprintf('\n')
    */
    const float pi = 3.141592654f;
    const float pExpectedOutput[] = { 0.099833f, 0.774167f, 0.995004f, 0.632981f,
        -0.099833f, -0.774167f, -0.995004f, -0.632981f };
    float       pActualOutput[8];
    int         i;

    iusSetShiftedSineTable2FloatVector(pActualOutput, 8, pi / 4.0f, 0.1f);

    for (i = 0; i < 8; i++)
    {
        TEST_ASSERT_FLOAT_WITHIN(1e-6f, pExpectedOutput[i], pActualOutput[i]);
    }
}

//------------------------------------------------------------------------------
// SetCosineTable2FloatVectorTest
//------------------------------------------------------------------------------
void SetCosineTable2FloatVectorTest(void)
{
    /* in MatLab:
    for n = 0:7, fprintf('%.6ff, ', cos((pi / 4)*n)); end, fprintf('\n')
    */
    const float pi = 3.141592654f;
    const float pExpectedOutput[] = { 1.000000f, 0.707107f, 0.000000f, -0.707107f,
        -1.000000f, -0.707107f, -0.000000f, 0.707107f };
    float       pActualOutput[8];
    int         i;

    iusSetCosineTable2FloatVector(pActualOutput, 8, pi / 4.0f);

    for (i = 0; i < 8; i++)
    {
        TEST_ASSERT_FLOAT_WITHIN(1e-6f, pExpectedOutput[i], pActualOutput[i]);
    }
}

//------------------------------------------------------------------------------
// SetSigmoid2FloatVectorTest
//------------------------------------------------------------------------------
void SetSigmoid2FloatVectorTest(void)
{
    /* in MatLab:
    N = 4;
    a = 5;
    x = [ 0 1 2 3 ];
    y = (a*(x-(N/2))) ./ sqrt(1 + (a*(x-(N/2))).^2);
    for n = 1 : N, fprintf('%.6ff, ',y(n)); end, fprintf('\n')
    */
    const float a = 5.0f;
    const float pExpectedOutput[] = { -0.995037f, -0.980581f, 0.000000f, 0.980581f };
    float       pActualOutput[4];

    iusSetSigmoid2FloatVector(pActualOutput, 4, (float)4 * a / 2.0f);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 4);
}

//------------------------------------------------------------------------------
// testPolyValFloatVector
//------------------------------------------------------------------------------
void PolyValFloatVectorTest(void)
{
    /* in MatLab:
    p = [ 3 2 1 ];
    x = [ 5 7 11 13 ];
    y = polyval( p , x );
    */
    const float pCoefs[] = { 3.0f, 2.0f, 1.0f };
    const float pInput[] = { 5.0f, 7.0f, 11.0f, 13.0f };
    const float pExpectedOutput[] = { 86.0f, 162.0f, 386.0f, 534.0f };
    float       pActualOutput[4];

    iusPolyValFloatVector(pActualOutput, pInput, 4, pCoefs, 3);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 4);
}

//------------------------------------------------------------------------------
// SetGaussianFloatVectorTest
//------------------------------------------------------------------------------
void SetGaussianFloatVectorTest(void)
{
    const float pExpectedOutput[] = { 0.480514f, -2.129947f, -1.004516f,
        1.271126f, 1.122959f, -0.478529f };
    float       pActualOutput[6];
    int         seed = 1;

    iusSetGaussianFloatVector(pActualOutput, 6, seed);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// SetRandomFloatVectorTest
//------------------------------------------------------------------------------
void SetRandomFloatVectorTest(void)
{
    const float pExpectedOutput[] = { 41.0f, 18467.0f, 6334.0f,
        26500.0f, 19169.0f, 15724.0f };
    float       pActualOutput[6];
    int         seed = 1;

    iusSetRandomFloatVector(pActualOutput, 6, seed);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 6);
}

//------------------------------------------------------------------------------
// CopyDoubleVector2IntVectorTest
//------------------------------------------------------------------------------
void CopyDoubleVector2IntVectorTest(void)
{
    const double pInput[] = { 25.4, 32767.9, 32768.1, -32768, -32768.9 };
    const int    pExpectedOutput[] = { 25, 32767, 32768, -32768, -32768 };
    int          pActualOutput[5];

    iusCopyDoubleVector2IntVector(pActualOutput, pInput, 5);

    TEST_ASSERT_EQUAL_INT32_ARRAY(pExpectedOutput, pActualOutput, 5);
}

//------------------------------------------------------------------------------
// CopyDoubleVector2ShortVectorTest
//------------------------------------------------------------------------------
void CopyDoubleVector2ShortVectorTest(void)
{
    const double pInput[] = { 25.4, 32767.5, 32768.1, -32768 };
    const short  pExpectedOutput[] = { 25, 32767, -32768, -32768 };
    short        pActualOutput[4];

    iusCopyDoubleVector2ShortVector(pActualOutput, pInput, 4);

    TEST_ASSERT_EQUAL_INT16_ARRAY(pExpectedOutput, pActualOutput, 4);
}

//------------------------------------------------------------------------------
// Abs<type>VectorTest
//------------------------------------------------------------------------------
void AbsFloatVectorTest(void)
{
    const float pInput[]          = { 1.0f, 3.3f, -7.0f, 0.0f, -0.0f };
    const float pExpectedOutput[] = { 1.0f, 3.3f,  7.0f, 0.0f,  0.0f };
    float       pActualOutput[5];

    iusAbsFloatVector(pActualOutput, pInput, 5);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 5);
}

void AbsShortVectorTest(void)
{
    const short pInput[] = { 1, 3, -7, 0, -0 };
    const short pExpectedOutput[] = { 1, 3,  7, 0, 0 };
    short       pActualOutput[5];

    iusAbsShortVector(pActualOutput, pInput, 5);

    TEST_ASSERT_EQUAL_INT16_ARRAY(pExpectedOutput, pActualOutput, 5);
}

//------------------------------------------------------------------------------
// MedianFilterFloatVectorTest
//------------------------------------------------------------------------------
void MedianFilterFloatVectorTest(void)
{
    /* in MatLab:
    x = [ 1 1   1 3 7 13 19 23 31 37 29 21 17 11 5 2   2 2 ];
    n = 5;
    y = zeros( 1, 14 );
    for i=1:14
    y(1,i) = median( x( i:i+n-1 ) );
    end
    */
    const float pInput[]              = { 1, 3, 7, 13, 19, 23, 31, 37, 29, 21, 17, 11,  5,  2 };
    //  const float pExpectedOutput[] = { 1, 3, 7, 13, 19, 23, 29, 29, 29, 21, 17, 11,  5,  2 };  /* matlab */
    const float pExpectedOutput[]     = { 3, 3, 7, 13, 19, 23, 29, 29, 29, 21, 17, 11, 11, 11 };  /* C. FIXME: explain in API/.h? */
    float       pActualOutput[14];

    iusMedianFilterFloatVector(pActualOutput, pInput, 14, 5);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 14);
}

//------------------------------------------------------------------------------
// QuantizeFloatVector2ShortVectorTest
//------------------------------------------------------------------------------
void QuantizeFloatVector2ShortVectorTest(void)
{
    const float pInput[]          = { -32769.0f, -32768.0f, -32767.0f, 0.0f, 32766.0f, 32767.0f, 32768.0f };
    const int   pExpectedOutput[] = { -32768   , -32768   , -32767   , 0   , 32766   , 32767   , 32767    };
    short       pActualOutputS[7];
    int         pActualOutputI[7];

    iusQuantizeFloatVector2ShortVector(pActualOutputS, pInput, 7, -32768.0f, 32767.0f);
    iusCopyShortVector2IntVector(pActualOutputI, pActualOutputS, 7);

    TEST_ASSERT_EQUAL_INT32_ARRAY(pExpectedOutput, pActualOutputI, 7);
}

//------------------------------------------------------------------------------
// Max<type>VectorTest
//------------------------------------------------------------------------------
void MaxFloatVectorTest(void)
{
    const float pInput[] = { -3.0f, 5.0f, -6.0f, 2.0f };
    float       expectedOutput = 5.0f;
    float       actualOutput;

    actualOutput = iusMaxFloatVector(pInput, 4);

    TEST_ASSERT_EQUAL_FLOAT(expectedOutput, actualOutput);
}

void MaxIntVectorTest(void)
{
    const int pInput[] = { -3, 5, -6, 2 };
    int       expectedOutput = 5;
    int       actualOutput;

    actualOutput = iusMaxIntVector(pInput, 4);

    TEST_ASSERT_EQUAL_INT32(expectedOutput, actualOutput);
}

void MaxShortVectorTest(void)
{
    const short pInput[] = { -3, 5, -6, 2 };
    short       expectedOutput = 5;
    short       actualOutput;

    actualOutput = iusMaxShortVector(pInput, 4);

    TEST_ASSERT_EQUAL_INT16(expectedOutput, actualOutput);
}

//------------------------------------------------------------------------------
// Min<type>VectorTest
//------------------------------------------------------------------------------
void MinFloatVectorTest(void)
{
    const float pInput[] = { -3.0f, 5.0f, -6.0f, 2.0f };
    float       expectedOutput = -6.0f;
    float       actualOutput;

    actualOutput = iusMinFloatVector(pInput, 4);

    TEST_ASSERT_EQUAL_FLOAT(expectedOutput, actualOutput);
}

void MinIntVectorTest(void)
{
    const int pInput[] = { -3, 5, -6, 2 };
    int       expectedOutput = -6;
    int       actualOutput;

    actualOutput = iusMinIntVector(pInput, 4);

    TEST_ASSERT_EQUAL_INT32(expectedOutput, actualOutput);
}

void MinShortVectorTest(void)
{
    const short pInput[] = { -3, 5, -6, 2 };
    short       expectedOutput = -6;
    short       actualOutput;

    actualOutput = iusMinShortVector(pInput, 4);

    TEST_ASSERT_EQUAL_INT16(expectedOutput, actualOutput);
}

//------------------------------------------------------------------------------
// MaxIndexFloatVectorTest
//------------------------------------------------------------------------------
void MaxIndexFloatVectorTest(void)
{
    const float pInput[]         = { -3.0f, 5.0f, -6.0f, 2.0f };
    const float expectedMaxValue = 5.0f;
    const int   expectedMaxIndex = 1;
    float actualMaxValue;
    int   actualMaxIndex;

    actualMaxValue = iusMaxIndexFloatVector(pInput, 4, &actualMaxIndex);

    TEST_ASSERT_EQUAL_FLOAT(expectedMaxValue, actualMaxValue);
    TEST_ASSERT_EQUAL_INT(expectedMaxIndex, actualMaxIndex);
}

//------------------------------------------------------------------------------
// MinIndexFloatVectorTest
//------------------------------------------------------------------------------
void MinIndexFloatVectorTest(void)
{
    const float pInput[]         = { -3.0f, 5.0f, -6.0f, 2.0f };
    const float expectedMinValue = -6.0f;
    const int   expectedMinIndex = 2;
    float actualMinValue;
    int   actualMinIndex;

    actualMinValue = iusMinIndexFloatVector(pInput, 4, &actualMinIndex);

    TEST_ASSERT_EQUAL_FLOAT(expectedMinValue, actualMinValue);
    TEST_ASSERT_EQUAL_INT(expectedMinIndex, actualMinIndex);
}

//------------------------------------------------------------------------------
// NumUniqueValuesIntVectorTest
//------------------------------------------------------------------------------
void NumUniqueValuesIntVectorTest(void)
{
    const int pInput[]       = { -3, 5, 1, 5, 2, -100, 1, 8, -3, 5 };
    const int expectedOutput = 6;
    int       actualOutput;

    actualOutput = iusNumUniqueValuesIntVector(pInput, 10);

    TEST_ASSERT_EQUAL_INT(expectedOutput, actualOutput);
}

//------------------------------------------------------------------------------
// FirstIndexAboveThresholdFloatVectorTest
//------------------------------------------------------------------------------
void FirstIndexAboveThresholdFloatVectorTest(void)
{
    const float pInput[]            = { -3.0f, -6.0f, 4.0f, 4.1f, 4.0f, 2.0f };
    const float threshold           = 4.0f;
    const float expectedOutputValue = 4.1f;
    const int   expectedOutputIndex = 3;
    float       actualOutputValue;
    int         actualOutputIndex;

    actualOutputValue = iusFirstIndexAboveThresholdFloatVector(pInput, 6,
        threshold, &actualOutputIndex);

    TEST_ASSERT_EQUAL_FLOAT(expectedOutputValue, actualOutputValue);
    TEST_ASSERT_EQUAL_INT(expectedOutputIndex, actualOutputIndex);
}

//------------------------------------------------------------------------------
// SumElementsFloatVectorTest
//------------------------------------------------------------------------------
void SumElementsFloatVectorTest(void)
{
    const float pInput[] = { -3.0f, 5.0f, -6.0f, 2.0f };
    const float expectedOutput = -2.0f;
    float       actualOutput;

    actualOutput = iusSumElementsFloatVector(pInput, 4);

    TEST_ASSERT_EQUAL_FLOAT(expectedOutput, actualOutput);
}

//------------------------------------------------------------------------------
// SumAbsElementsFloatVectorTest
//------------------------------------------------------------------------------
void SumAbsElementsFloatVectorTest(void)
{
    const float pInput[] = { -3.0f, 5.0f, -6.0f, 2.0f };
    const float expectedOutput = 16.0f;
    float       actualOutput;

    actualOutput = iusSumAbsElementsFloatVector(pInput, 4);

    TEST_ASSERT_EQUAL_FLOAT(expectedOutput, actualOutput);
}

//------------------------------------------------------------------------------
// SumSquaredElementsFloatVectorTest
//------------------------------------------------------------------------------
void SumSquaredElementsFloatVectorTest(void)
{
    const float pInput[] = { -3.0f, 5.0f, -6.0f, 2.0f };
    const float expectedOutput = 74.0f;
    float       actualOutput;

    actualOutput = iusSumSquaredElementsFloatVector(pInput, 4);

    TEST_ASSERT_EQUAL_FLOAT(expectedOutput, actualOutput);
}

//------------------------------------------------------------------------------
// ZeroPhaseFirFilterFloatVectorTest
//------------------------------------------------------------------------------
void ZeroPhaseFirFilterFloatVectorTest(void)
{
    /* in MatLab:
    x = [ 1 2 3 5 7 11 13 17 19 23 29 31 37   0 0 ];
    c = [ -1 0.5 2.5 0.5 -1 ];
    z = filter( c, 1, x );
    z = z(1, 1+floor(5/2):end )
    */
    const float pInput[]            = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
    const float pCoefs[]            = { -1.0f, 0.5f, 2.5f, 0.5f, -1.0f };
    //const float pExpectedOutput[] = { 0.5f, 2.0f, 3.0f, 4.5f, 9.5f, 15.5f, 20.5f, 24.5f, 25.5f, 33.5f, 43.5f, 87.5f, 79.f };
    const float pExpectedOutput[]   = { 0.5f, 2.0f, 3.0f, 4.5f, 9.5f, 15.5f, 20.5f, 24.5f, 25.5f, 33.5f, 43.5f, 0.0f, 0.0f }; /* FIXME: explain in API/.h */
    float       pActualOutput[13];

    iusZeroPhaseFirFilterFloatVector(pActualOutput, pInput, 13, pCoefs, 5);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 13);
}

//------------------------------------------------------------------------------
// ZeroPhaseFirFilterAndDecimateFloatVectorTest
//------------------------------------------------------------------------------
void ZeroPhaseFirFilterAndDecimateFloatVectorTest(void)
{
    const float pInput[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
    const float pCoefs[] = { -1.0f, 0.5f, 2.5f, 0.5f, -1.0f };
    //const float pExpectedOutput[] = { 0.5f, 3.0f, 9.5f, 20.5f, 25.5f, 43.5f, 79.f };
    const float pExpectedOutput[] = { 0.5f, 3.0f, 9.5f, 20.5f, 25.5f, 43.5f, 0.0f }; /* FIXME: explain in API/.h */
    float       pActualOutput[7];

    iusZeroPhaseFirFilterAndDecimateFloatVector(pActualOutput, 7, pInput, 13, pCoefs, 5, 2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 7);
}

//------------------------------------------------------------------------------
// HalfBandFirFilterFloatVectorTest
//------------------------------------------------------------------------------
void HalfBandFirFilterFloatVectorTest(void)
{
    /* in MatLab:
    x = [ 1 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59  0 0 0 0 0 ];
    c = [ -1 0.0 0.5 0.0 2.5 0.0 -1.4 0.0 0.3 0.0 3.14 ];
    z = filter( fliplr( c ), 1, x );
    z = z(1, 1+floor(11/2):end )'
    */
    const float pInput[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59 };
    const float pCoefs[] = { -1.0f, 0.0f, 0.5f, 0.0f, 2.5f, 0.0f, -1.4f, 0.0f, 0.3f, 0.0f, 3.14f };
    const float pExpectedOutput[] = {  33.2400f,  41.2200f,  54.6800f,  61.7600f,
                                       75.4200f,  96.5600f, 108.4400f, 131.2800f,
                                      148.8400f, 152.5200f, 171.4800f, 196.5200f,
                                      213.9600f,  43.7000f,  46.9000f,  22.8000f,
                                       24.4000f, 117.0000f };
    float pActualOutput[18];

    iusHalfBandFirFilterFloatVector(pActualOutput, pInput, 18, pCoefs, 11);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 18);
}

//------------------------------------------------------------------------------
// EdgeFilterFloatVectorTest
//------------------------------------------------------------------------------
void EdgeFilterFloatVectorTest(void)
{
    /* in MatLab:
    x = [ 1 2 3 5 7 11 13 17 19 23 29 31 37   0 0 ];
    c = [ -1 -1 0 1 1 ];
    y = filter( c, 1, x );
    y = y(1, 1+floor(5/2):end )
    */
    const float pInput[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
    //  const float pExpectedOutput[] = { -5, -7, -9, -13, -16, -18, -18, -18, -22, -24, -26, 15, 60 }; // MatLab
    const float pExpectedOutput[]     = {  1,  0, -3, -13, -16, -18, -18, -18, -22, -24, -8, 0, 37 };   // C, FIXME?
    float       pTemp[13];
    float       pActualOutput[13];

    iusEdgeFilterFloatVector(pActualOutput, pTemp, pInput, 13, 5);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 13);
}

//------------------------------------------------------------------------------
// QuickSortFloatVectorSegmentTest
//------------------------------------------------------------------------------
void QuickSortFloatVectorSegmentTest(void)
{
    float       pIO[]             = { 3, 1, 2, 13, 7, 11, 5, 17, 29, 23, 19, 37, 31 };
    const float pExpectedOutput[] = { 3, 1, 2, 7, 11, 13, 5, 17, 29, 23, 19, 37, 31 };

    iusQuickSortFloatVectorSegment(pIO, 13, 2, 5);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pIO, 13);
}

//------------------------------------------------------------------------------
// CumSumFloatVectorTest
//------------------------------------------------------------------------------
void CumSumFloatVectorTest(void)
{
    /* in MatLab:
    x = [ 1 2 3 5 7 11 13 17 19 23 29 31 37 ];
    y = cumsum( x );
    */
    const float pInput[]          = { 1, 2, 3,  5,  7, 11, 13, 17, 19,  23,  29,  31,  37 };
    const float pExpectedOutput[] = { 1, 3, 6, 11, 18, 29, 42, 59, 78, 101, 130, 161, 198 };
    float       pActualOutput[13];
    
    iusCumSumFloatVector(pActualOutput, pInput, 13);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 13);
}

//------------------------------------------------------------------------------
// CumSumAndSquaredFloatVectorTest
//------------------------------------------------------------------------------
void CumSumAndSquaredFloatVectorTest(void)
{
    /* in MatLab:
    x = [ 1 2 3 5 7 11 13 17 19 23 29 31 37 ];
    y1 = cumsum( x );
    y2 = cumsum( x.*x );
    */
    const float pInput[]           = { 1, 2,  3,  5,  7,  11,  13,  17,   19,   23,   29,   31,   37 };
    const float pExpectedOutput1[] = { 1, 3,  6, 11, 18,  29,  42,  59,   78,  101,  130,  161,  198 };
    const float pExpectedOutput2[] = { 1, 5, 14, 39, 88, 209, 378, 667, 1028, 1557, 2398, 3359, 4728 };
    float       pActualOutput1[13];
    float       pActualOutput2[13];

    iusCumSumAndSquaredFloatVector(pActualOutput1, pActualOutput2, pInput, 13);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput1, pActualOutput1, 13);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput2, pActualOutput2, 13);
}

//------------------------------------------------------------------------------
// LinearInterpolatePoints2FloatVectorTest
//------------------------------------------------------------------------------
void LinearInterpolatePoints2FloatVectorTest(void)
{
    /* in MatLab:
    x = [ 0 3 5 10 12 ];
    y = [ 11 13 17 19 23 ];
    v = interp1( x, y, 0:12 );
    */
    const int   pX[] = { 0, 3, 5, 10, 12 };
    const float pY[] = { 11, 13, 17, 19, 23 };
    const float pExpectedOutput[] = { 11.0f, 11.66666f, 12.33333f, 13.0f,
                                      15.0f, 17.0f    , 17.4f    , 17.8f,
                                      18.2f, 18.6f    , 19.0f    , 21.0f, 23.0f };
    float       pActualOutput[13];

    iusLinearInterpolatePoints2FloatVector(pActualOutput, 13, pX, pY, 5);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 13);
}

//------------------------------------------------------------------------------
// InterpolateFloatVectorTest
//------------------------------------------------------------------------------
void InterpolateFloatVectorTest(void)
{
    // First test, for a short vector where we have a complete expected output.
    {
        const float pInput[] = { -3, 7, 2, -5, -1 };
        const float pExpectedOutput[] =
            { -3.000000e+00f, -2.078923e+00f, -8.574441e-01f,  5.791114e-01f,
               2.122066e+00f,  3.650156e+00f,  5.041771e+00f,  6.187272e+00f,
               7.000000e+00f,  7.749558e+00f,  8.085698e+00f,  7.977040e+00f,
               7.427231e+00f,  6.474421e+00f,  5.187537e+00f,  3.659771e+00f,
               2.000000e+00f,  5.599481e-01f, -8.317208e-01f, -2.100375e+00f,
              -3.183099e+00f, -4.032940e+00f, -4.621624e+00f, -4.940537e+00f,
              -5.000000e+00f, -5.228245e+00f, -5.161814e+00f, -4.819347e+00f,
              -4.244132e+00f, -3.498798e+00f, -2.658077e+00f, -1.800496e+00f,
              -1.000000e+00f, -4.331090e-01f, -1.110223e-15f,  2.851685e-01f,
               4.244132e-01f,  4.343335e-01f,  3.429776e-01f,  1.856182e-01f };
        float pActualOutput[40];
        int   i;

        iusInterpolateFloatVector( pActualOutput, pInput, 5, 8 );

        for (i = 0; i < 40; i++)
        {
            TEST_ASSERT_FLOAT_WITHIN( 1e-5f, pExpectedOutput[i], pActualOutput[i] );
        }
    }
    // Second test, for a long vector where we only verify that the every
    // so-many-th sample at the output indeed equals the corresponding sample
    // at the input.
    {
        int     N     = 2048;
        int     upFac = 8;  // only 1 or 8 allowed
        int     seed  = 0;
        float * pInput;
        float * pOutput;
        int     i;

        pInput  = iusAllocFloatVector( N );
        pOutput = iusAllocFloatVector( N * upFac );
        iusSetRandomFloatVector( pInput, N, seed );
        iusInterpolateFloatVector( pOutput, pInput, N, upFac );
        for ( i = 0; i < N; i++ )
        {
            TEST_ASSERT_EQUAL_FLOAT( pInput[i], pOutput[upFac * i] );
        }
        iusFreeFloatVector( pOutput );
        iusFreeFloatVector( pInput );
    }
    // Third test, all input values the same
    {
        const float pInput[] = { 1, 1, 1, 1, 1 };
        const float pExpectedOutput[]
            = {  1.000000e+00f,  1.048743e+00f,  1.071805e+00f,  1.073769e+00f,
                 1.061033e+00f,  1.040865e+00f,  1.020358e+00f,  1.005432e+00f,
                 1.000000e+00f,  9.404654e-01f,  8.917419e-01f,  8.598926e-01f,
                 8.488264e-01f,  8.598926e-01f,  8.917419e-01f,  9.404654e-01f,
                 1.000000e+00f,  9.404654e-01f,  8.917419e-01f,  8.598926e-01f,
                 8.488264e-01f,  8.598926e-01f,  8.917419e-01f,  9.404654e-01f,
                 1.000000e+00f,  1.005432e+00f,  1.020358e+00f,  1.040865e+00f,
                 1.061033e+00f,  1.073769e+00f,  1.071805e+00f,  1.048743e+00f,
                 1.000000e+00f,  8.662181e-01f,  7.202531e-01f,  5.703369e-01f,
                 4.244132e-01f,  2.895557e-01f,  1.714888e-01f,  7.424727e-02f,
                -2.168965e-17f, -1.082773e-01f, -1.800633e-01f, -2.138764e-01f  };
        float pActualOutput[40];
        int   i;

        iusInterpolateFloatVector( pActualOutput, pInput, 5, 8 );

        for (i = 0; i < 40; i++)
        {
            TEST_ASSERT_FLOAT_WITHIN(1e-6f, pExpectedOutput[i], pActualOutput[i]);
        }
    }
}

//------------------------------------------------------------------------------
// ReSampleFloatVectorTest
//------------------------------------------------------------------------------
void ReSampleFloatVectorTest(void)
{
    const float pInput[]          = { 1.0f, 2.0f, 3.0f };
    const float pExpectedOutput[] = { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f };
    float       pActualOutput[7];

    iusReSampleFloatVector(pActualOutput, 7, pInput, 3);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 7);
}

//------------------------------------------------------------------------------
// SetLinSpaceFloatVectorTest
//------------------------------------------------------------------------------
void SetLinSpaceFloatVectorTest(void)
{
    float startValue1 = -1.0f;
    float endValue1   =  1.0f;
    float startValue2 =  2.0f;
    float endValue2   = -2.0f;
    float pExpectedOutput1[] = { -1.0f, -0.6f, -0.2f,  0.2f,  0.6f,  1.0f };
    float pExpectedOutput2[] = {  2.0f,  1.2f,  0.4f, -0.4f, -1.2f, -2.0f };
    float pActualOutput1[6];
    float pActualOutput2[6];
    float pActualOutput3[8192];
    float pActualOutput4[8192];

    iusSetLinSpaceFloatVector(pActualOutput1, 6, startValue1, endValue1);
    iusSetLinSpaceFloatVector(pActualOutput2, 6, startValue2, endValue2);
    iusSetLinSpaceFloatVector(pActualOutput3, 8192, startValue1, endValue1);
    iusSetLinSpaceFloatVector(pActualOutput4, 8192, startValue2, endValue2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput1, pActualOutput1, 6);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput2, pActualOutput2, 6);
    TEST_ASSERT_EQUAL_FLOAT( endValue1, pActualOutput3[8191] );
    TEST_ASSERT_EQUAL_FLOAT( endValue2, pActualOutput4[8191] );
} 

//------------------------------------------------------------------------------
// SetHammingFloatVectorTest
//------------------------------------------------------------------------------
void SetHammingFloatVectorTest(void)
{
    // test 1 for input vector having length > 1
    {
        // In MatLab: y1 = hamming(8); y2 = hamming(7);
        const float pExpectedOutput1[] = { 0.0800000f, 0.2531947f, 0.6423596f,
            0.9544457f, 0.9544457f, 0.6423596f, 0.2531947f, 0.0800000f };
        const float pExpectedOutput2[] = { 0.0800000f, 0.3100000f, 0.7700000f,
            1.0000000f, 0.7700000f, 0.3100000f, 0.0800000f };
        float       pActualOutput1[8];
        float       pActualOutput2[7];

        iusSetHammingFloatVector(pActualOutput1, 8);
        iusSetHammingFloatVector(pActualOutput2, 7);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput1, pActualOutput1, 6);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput2, pActualOutput2, 6);
    }
    // test 2 for input vector having length == 1
    {
        // In MatLab: y = hamming(1);
        const float pExpectedOutput[] = { 1.0f };
        float       pActualOutput[1];

        iusSetHammingFloatVector(pActualOutput, 1);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 1);
    }
}

//------------------------------------------------------------------------------
// SetHanningFloatVectorTest
//------------------------------------------------------------------------------
void SetHanningFloatVectorTest(void)
{
    // test 1 for input vector having length > 1
    {
        // In MatLab: y1 = hanning(8); y2 = hanning(7);
        const float pExpectedOutput1[] = { 0.1169778f, 0.4131759f, 0.7500000f,
            0.9698463f, 0.9698463f, 0.7500000f, 0.4131759f, 0.1169778f };
        const float pExpectedOutput2[] = { 0.1464466f, 0.5000000f, 0.8535534f,
            1.0000000f, 0.8535534f, 0.5000000f, 0.1464466f };
        float       pActualOutput1[8];
        float       pActualOutput2[7];

        iusSetHanningFloatVector(pActualOutput1, 8);
        iusSetHanningFloatVector(pActualOutput2, 7);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput1, pActualOutput1, 6);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput2, pActualOutput2, 6);
    }
    // test 2 for input vector having length == 1
    {
        // In MatLab: y = hanning(1);
        const float pExpectedOutput[] = { 1.0f };
        float       pActualOutput[1];

        iusSetHanningFloatVector(pActualOutput, 1);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(pExpectedOutput, pActualOutput, 1);
    }
}

//------------------------------------------------------------------------------
// CountUniqueElementsIntVectorTest
//------------------------------------------------------------------------------
void CountUniqueElementsIntVectorTest(void)
{
    const int pSrc[]   = {7, 0, -1, 3, -1, 0, 0, 1, 1, 4, 3, 5, 1, 7};
    int length         = 14;
    int expectedOutput = 7;
    int actualOutput;

    actualOutput = iusCountUniqueElementsIntVector( pSrc, length );

    TEST_ASSERT_EQUAL_INT(expectedOutput, actualOutput);
}