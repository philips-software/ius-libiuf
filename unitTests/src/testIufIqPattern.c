#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufIqPatternPrivate.h>


static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pDemodulationLabel = "pDemodulationLabel";

static char *pErrorFilename = "IufIqPattern.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufIqPattern);

TEST_SETUP(IufIqPattern)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufIqPattern)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufIqPattern, testIufPatternCreate)
{
    iuiqpa_t obj = iufIqPatternCreate( 0.01f,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pDemodulationLabel);

    iuiqpa_t notherObj = iufIqPatternCreate( 0.01f,
                                         pPulseLabel,
                                         pSourceLabel,
                                         pChannelMapLabel,
                                         pApodizationLabel,
                                         pDemodulationLabel);
    TEST_ASSERT(obj != IUIQPA_INVALID);
    TEST_ASSERT(notherObj != IUIQPA_INVALID);
    iufIqPatternDelete(obj);
    iufIqPatternDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iufIqPatternCreate(  0.01f,
                             "",
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iufIqPatternCreate(  0.01f,
                             pPulseLabel,
                             "",
                             pChannelMapLabel,
                             pApodizationLabel,
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iufIqPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             "",
                             pApodizationLabel,
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iufIqPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             "",
                             pDemodulationLabel);
    TEST_ASSERT(obj == IUIQPA_INVALID);

    obj = iufIqPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             "");
    TEST_ASSERT(obj == IUIQPA_INVALID);

    TEST_ASSERT_EQUAL(5, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(IufIqPattern, testIufPatternDelete)
{
    iuiqpa_t obj = iufIqPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);
    TEST_ASSERT(obj != IUIQPA_INVALID);
    int status = iufIqPatternDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0, iufErrorGetCount());

    status = iufIqPatternDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1, iufErrorGetCount());
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
    iuiqpa_t actual = iufIqPatternCreate( timeInFrame,
                                      pPulseLbl,
                                      pSourceLbl,
                                      pChannelMapLbl,
                                      pApodizationLbl,
                                      pDemodulationLbl);
    if(actual == IUIQPA_INVALID) return IUF_ERR_VALUE;
    int equal = iufIqPatternCompare(reference, actual);
    iufIqPatternDelete(actual);
    return equal;
}

TEST(IufIqPattern, testIufPatternCompare)
{
    IUF_BOOL equal;
    iuiqpa_t obj = iufIqPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);
#define CREATE_AND_COMPARE(e,a,o,t,pul,sl,cml,al,rl) a = createAndCompare(o, t, pul,sl,cml,al,rl); \
  TEST_ASSERT_EQUAL(e,a)


    CREATE_AND_COMPARE(IUF_TRUE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.02f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       "MyPulseLabel", pSourceLabel, pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, "MySourceLabel", pChannelMapLabel, pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, "MyChannelMapLabel", pApodizationLabel, pDemodulationLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, "MyApodizationLabel", pDemodulationLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, "MyReceivesettingsLabel");

    // invalid params
    equal = iufIqPatternCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufIqPatternCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iufIqPatternDelete(obj);
}


TEST(IufIqPattern, testIufPatternSetGet)
{
    float timeInFrame = 0.01f;
    iupa_t obj = iufPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);
    TEST_ASSERT(obj != IUPA_INVALID);

    // label
    TEST_ASSERT_EQUAL_STRING(pPulseLabel, iufPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pSourceLabel, iufPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pChannelMapLabel, iufPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pApodizationLabel, iufPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pDemodulationLabel, iufPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(timeInFrame, iufPatternGetTimeInFrame(obj) );


    // invalid param
    iufPatternDelete(obj);
    obj = IUPA_INVALID;
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iufPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iufPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iufPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iufPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iufPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufPatternGetTimeInFrame(obj) );

    TEST_ASSERT_EQUAL(6, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iufPatternDelete(obj);
}

TEST(IufIqPattern, testIufSerialization)
{
    char *filename = "testIufPatternSerialization.hdf5";

    // create and save
    float timeInFrame = 0.01f;
    iuiqpa_t obj = iufIqPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pDemodulationLabel);

    iuiqpa_t notherObj = iufIqPatternCreate(0.02f,
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
    int status = iufIqPatternSave( obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iuiqpa_t savedObj = iufIqPatternLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufIqPatternCompare(obj,savedObj));
    TEST_ASSERT_EQUAL(IUF_FALSE, iufIqPatternCompare(notherObj,savedObj));
    iufIqPatternDelete((iuiqpa_t)obj);
    iufIqPatternDelete((iuiqpa_t)notherObj);
    iufIqPatternDelete(savedObj);
}

TEST_GROUP_RUNNER(IufIqPattern)
{
    RUN_TEST_CASE(IufIqPattern, testIufPatternCreate);
    RUN_TEST_CASE(IufIqPattern, testIufPatternDelete);
    RUN_TEST_CASE(IufIqPattern, testIufPatternCompare);
    RUN_TEST_CASE(IufIqPattern, testIufPatternSetGet);
    RUN_TEST_CASE(IufIqPattern, testIufSerialization);
}
