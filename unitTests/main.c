#include "unity/unity.h"
#include "iusVectorTests.h"
#include "iusMedianFilterTests.h"
#include "iusLUTTests.h"
#include "iusInputTests.h"

int AverageThreeBytes(int a, int b, int c)
{
    return (int)(((int)a + (int)b + (int)c) / 3);
}

void setUp()
{
}

void tearDown()
{
}

void test_AverageThreeBytes_should_AverageMidRangeValues(void)
{
    TEST_ASSERT_EQUAL_HEX8(40, AverageThreeBytes(30, 40, 50));
    TEST_ASSERT_EQUAL_HEX8(40, AverageThreeBytes(10, 70, 40));
    TEST_ASSERT_EQUAL_HEX8(33, AverageThreeBytes(33, 33, 33));
}

void test_AverageThreeBytes_should_AverageHighValues(void)
{
    TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
    TEST_ASSERT_EQUAL_HEX8(127, AverageThreeBytes(127, 127, 127));
    TEST_ASSERT_EQUAL_HEX8(84, AverageThreeBytes(0, 126, 126));
}

int main(void)
{
    UNITY_BEGIN();
    // RUN_TEST(test_AverageThreeBytes_should_AverageMidRangeValues);
    // RUN_TEST(test_AverageThreeBytes_should_AverageHighValues);

    // RUN_TEST(SetValue2DoubleVectorTest);
    // RUN_TEST(SetValue2FloatVectorTest);
    // RUN_TEST(SetValue2IntVectorTest);
    // RUN_TEST(SetValue2ShortVectorTest);
    // RUN_TEST(SetValue2ByteVectorTest);
    // RUN_TEST(ZeroDoubleVectorTest);
    // RUN_TEST(ZeroFloatVectorTest);
    // RUN_TEST(ZeroIntVectorTest);
    // RUN_TEST(ZeroShortVectorTest);
    // RUN_TEST(ZeroByteVectorTest);
    // RUN_TEST(SetSineTable2FloatVectorTest);
    // RUN_TEST(SetShiftedSineTable2FloatVectorTest);
    // RUN_TEST(SetCosineTable2FloatVectorTest);
    // RUN_TEST(SetSigmoid2FloatVectorTest);
    // RUN_TEST(PolyValFloatVectorTest);
    // RUN_TEST(SetGaussianFloatVectorTest);
    // RUN_TEST(SetRandomFloatVectorTest);

    // RUN_TEST(CopyDoubleVector2IntVectorTest);
    // RUN_TEST(CopyDoubleVector2ShortVectorTest);
    // RUN_TEST(AbsFloatVectorTest);
    // RUN_TEST(AbsShortVectorTest);
    // RUN_TEST(MedianFilterFloatVectorTest);
    // RUN_TEST(QuantizeFloatVector2ShortVectorTest);
    // RUN_TEST(MaxFloatVectorTest);
    // RUN_TEST(MaxIntVectorTest);
    // RUN_TEST(MaxShortVectorTest);
    // RUN_TEST(MinFloatVectorTest);
    // RUN_TEST(MinIntVectorTest);
    // RUN_TEST(MinShortVectorTest);
    // RUN_TEST(MaxIndexFloatVectorTest);
    // RUN_TEST(MinIndexFloatVectorTest);
    // RUN_TEST(NumUniqueValuesIntVectorTest);
    // RUN_TEST(FirstIndexAboveThresholdFloatVectorTest);
    // RUN_TEST(SumElementsFloatVectorTest);
    // RUN_TEST(SumAbsElementsFloatVectorTest);
    // RUN_TEST(SumSquaredElementsFloatVectorTest);
    // RUN_TEST(ZeroPhaseFirFilterFloatVectorTest);
    // RUN_TEST(ZeroPhaseFirFilterAndDecimateFloatVectorTest);
    // RUN_TEST(HalfBandFirFilterFloatVectorTest);
    // RUN_TEST(EdgeFilterFloatVectorTest);
    // RUN_TEST(QuickSortFloatVectorSegmentTest);
    // RUN_TEST(CumSumFloatVectorTest);
    // RUN_TEST(CumSumAndSquaredFloatVectorTest);
    // RUN_TEST(LinearInterpolatePoints2FloatVectorTest);
    // RUN_TEST(InterpolateFloatVectorTest);
    // RUN_TEST(ReSampleFloatVectorTest);
    // RUN_TEST(SetLinSpaceFloatVectorTest);
    // RUN_TEST(SetHammingFloatVectorTest);
    // RUN_TEST(SetHanningFloatVectorTest);
    // RUN_TEST(CountUniqueElementsIntVectorTest);
    // RUN_TEST(FloatIntervalLUTTest);
    // RUN_TEST(MedianFilterTest);
    RUN_TEST(InputTest);

    return UNITY_END();
}
