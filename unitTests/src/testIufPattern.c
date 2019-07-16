#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufPatternPrivate.h>


static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";

static char *pErrorFilename = "IufPattern.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufPattern);

TEST_SETUP(IufPattern)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufPattern)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufPattern, testIufPatternCreate)
{
    iupa_t obj = iufPatternCreate( 0.01f,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pReceivesettingsLabel);

    iupa_t notherObj = iufPatternCreate( 0.01f,
                                         pPulseLabel,
                                         pSourceLabel,
                                         pChannelMapLabel,
                                         pApodizationLabel,
                                         pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    TEST_ASSERT(notherObj != IUPA_INVALID);
    iufPatternDelete(obj);
    iufPatternDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iufPatternCreate(  0.01f,
                             "",
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iufPatternCreate(  0.01f,
                             pPulseLabel,
                             "",
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iufPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             "",
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iufPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             "",
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iufPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                               "");
    TEST_ASSERT(obj == IUPA_INVALID);

    TEST_ASSERT_EQUAL(5, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(IufPattern, testIufPatternDelete)
{
    iupa_t obj = iufPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    int status = iufPatternDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufPatternDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


int createAndCompare
(
    iupa_t reference,
    float timeInFrame,
    const char *pPulseLbl,
    const char *pSourceLbl,
    const char *pChannelMapLbl,
    const char *pApodizationLbl,
    const char *pReceiveSettingsLbl
)
{
  iupa_t actual = iufPatternCreate( timeInFrame,
                                    pPulseLbl,
                                    pSourceLbl,
                                    pChannelMapLbl,
                                    pApodizationLbl,
                                    pReceiveSettingsLbl);
  if(actual == IUPA_INVALID) return IUF_ERR_VALUE;
  int equal = iufPatternCompare(reference,actual);
  iufPatternDelete(actual);
  return equal;
}

TEST(IufPattern, testIufPatternCompare)
{
    IUF_BOOL equal;
    iupa_t obj = iufPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);
#define CREATE_AND_COMPARE(e,a,o,t,pul,sl,cml,al,rl) a = createAndCompare(o, t, pul,sl,cml,al,rl); \
  TEST_ASSERT_EQUAL(e,a)


    CREATE_AND_COMPARE(IUF_TRUE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.02f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                      "MyPulseLabel", pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, "MySourceLabel", pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, "MyChannelMapLabel", pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, "MyApodizationLabel", pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUF_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, "MyReceivesettingsLabel");

    // invalid params
    equal = iufPatternCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufPatternCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    iufPatternDelete(obj);
}


TEST(IufPattern, testIufPatternSetGet)
{
    float timeInFrame = 0.01f;
    iupa_t obj = iufPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    
    // label
    TEST_ASSERT_EQUAL_STRING(pPulseLabel, iufPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pSourceLabel, iufPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pChannelMapLabel, iufPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pApodizationLabel, iufPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pReceivesettingsLabel, iufPatternGetReceivesettingsLabel(obj));
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

TEST(IufPattern, testIufSerialization)
{
    char *filename = "testIufPatternSerialization.hdf5";

    // create and save
    float timeInFrame = 0.01f;
    iupa_t obj = iufPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);

    iupa_t notherObj = iufPatternCreate(0.02f,
                                        pPulseLabel,
                                        pSourceLabel,
                                        pChannelMapLabel,
                                        pApodizationLabel,
                                        pReceivesettingsLabel);
    // fill
    TEST_ASSERT(obj != IUPA_INVALID);
    TEST_ASSERT(notherObj != IUPA_INVALID);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iufPatternSave( obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupa_t savedObj = iufPatternLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufPatternCompare(obj,savedObj));
    TEST_ASSERT_EQUAL(IUF_FALSE, iufPatternCompare(notherObj,savedObj));
    iufPatternDelete((iupa_t)obj);
    iufPatternDelete((iupa_t)notherObj);
    iufPatternDelete(savedObj);
}

TEST_GROUP_RUNNER(IufPattern)
{
    RUN_TEST_CASE(IufPattern, testIufPatternCreate);
    RUN_TEST_CASE(IufPattern, testIufPatternDelete);
    RUN_TEST_CASE(IufPattern, testIufPatternCompare);
    RUN_TEST_CASE(IufPattern, testIufPatternSetGet);
    RUN_TEST_CASE(IufPattern, testIufSerialization);
}
