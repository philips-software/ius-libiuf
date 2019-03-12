

//
// Created by nlv09165 on 13/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusIqPatternPrivate.h>


static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pDemodulationLabel = "pDemodulationLabel";

static char *pErrorFilename = "IusIqPattern.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusIqPattern);

TEST_SETUP(IusIqPattern)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusIqPattern)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusIqPattern, testIusPatternCreate)
{
    iuiqpa_t obj = iusIqPatternCreate( 0.01f,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pDemodulationLabel);

    iuiqpa_t notherObj = iusIqPatternCreate( 0.01f,
                                         pPulseLabel,
                                         pSourceLabel,
                                         pChannelMapLabel,
                                         pApodizationLabel,
                                         pDemodulationLabel);
    TEST_ASSERT(obj != IUIQPA_INVALID);
    TEST_ASSERT(notherObj != IUIQPA_INVALID);
    iusIqPatternDelete(obj);
    iusIqPatternDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    obj = iusIqPatternCreate(  0.01f,
                             "",
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iusIqPatternCreate(  0.01f,
                             pPulseLabel,
                             "",
                             pChannelMapLabel,
                             pApodizationLabel,
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iusIqPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             "",
                             pApodizationLabel,
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iusIqPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             "",
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iusIqPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             "");
    TEST_ASSERT(obj == IUIQPA_INVALID);

    TEST_ASSERT_EQUAL(5, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(IusIqPattern, testIusPatternDelete)
{
    iuiqpa_t obj = iusIqPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);
    TEST_ASSERT(obj != IUIQPA_INVALID);
    int status = iusIqPatternDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0, iusErrorGetCount());

    status = iusIqPatternDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


static int createAndCompare
        (
                iuiqpa_t reference,
                float timeInFrame,
                const char *pPulseLbl,
                const char *pSourceLbl,
                const char *pChannelMapLbl,
                const char *pApodizationLbl,
                const char *pDemodulationLbl
        )
{
    iuiqpa_t actual = iusIqPatternCreate( timeInFrame,
                                      pPulseLbl,
                                      pSourceLbl,
                                      pChannelMapLbl,
                                      pApodizationLbl,
                                      pDemodulationLbl);
    if(actual == IUIQPA_INVALID) return IUS_ERR_VALUE;
    int equal = iusIqPatternCompare(reference, actual);
    iusIqPatternDelete(actual);
    return equal;
}

TEST(IusIqPattern, testIusPatternCompare)
{
    IUS_BOOL equal;
    iuiqpa_t obj = iusIqPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);
#define CREATE_AND_COMPARE(e,a,o,t,pul,sl,cml,al,rl) a = createAndCompare(o, t, pul,sl,cml,al,rl); \
  TEST_ASSERT_EQUAL(e,a)


    CREATE_AND_COMPARE(IUS_TRUE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.02f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       "MyPulseLabel", pSourceLabel, pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, "MySourceLabel", pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, "MyChannelMapLabel", pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, "MyApodizationLabel", pDemodulationLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, "MyReceivesettingsLabel");

    // invalid params
    equal = iusIqPatternCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusIqPatternCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusIqPatternDelete(obj);
}


TEST(IusIqPattern, testIusPatternSetGet)
{
    float timeInFrame = 0.01f;
    iupa_t obj = iusPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);
    TEST_ASSERT(obj != IUPA_INVALID);

    // label
    TEST_ASSERT_EQUAL_STRING(pPulseLabel, iusPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pSourceLabel, iusPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pChannelMapLabel, iusPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pApodizationLabel, iusPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pDemodulationLabel, iusPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(timeInFrame, iusPatternGetTimeInFrame(obj) );


    // invalid param
    iusPatternDelete(obj);
    obj = IUPA_INVALID;
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iusPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusPatternGetTimeInFrame(obj) );

    TEST_ASSERT_EQUAL(6, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iusPatternDelete(obj);
}

TEST(IusIqPattern, testIusSerialization)
{
    char *filename = "testIusPatternSerialization.hdf5";

    // create and save
    float timeInFrame = 0.01f;
    iuiqpa_t obj = iusIqPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);

    iuiqpa_t notherObj = iusIqPatternCreate(0.02f,
                                        pPulseLabel,
                                        pSourceLabel,
                                        pChannelMapLabel,
                                        pApodizationLabel,
                                        pDemodulationLabel);
    // fill
    TEST_ASSERT(obj != IUIQPA_INVALID);
    TEST_ASSERT(notherObj != IUIQPA_INVALID);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusIqPatternSave( obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iuiqpa_t savedObj = iusIqPatternLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusIqPatternCompare(obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusIqPatternCompare(notherObj,savedObj));
    iusIqPatternDelete((iuiqpa_t)obj);
    iusIqPatternDelete((iuiqpa_t)notherObj);
    iusIqPatternDelete(savedObj);
}

TEST_GROUP_RUNNER(IusIqPattern)
{
    RUN_TEST_CASE(IusIqPattern, testIusPatternCreate);
    RUN_TEST_CASE(IusIqPattern, testIusPatternDelete);
    RUN_TEST_CASE(IusIqPattern, testIusPatternCompare);
    RUN_TEST_CASE(IusIqPattern, testIusPatternSetGet);
    RUN_TEST_CASE(IusIqPattern, testIusSerialization);
}
