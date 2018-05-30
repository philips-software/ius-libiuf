

//
// Created by nlv09165 on 28/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusHLPulseImp.h>

TEST_GROUP(IusPulse);

TEST_SETUP(IusPulse)
{
}

TEST_TEAR_DOWN(IusPulse)
{
}


TEST(IusPulse, testIusPulseCreate)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
    iup_t notherObj = iusHLPulseCreate(IUS_NON_PARAMETRIC_PULSETYPE,notherLabel);
    TEST_ASSERT(obj != IUP_INVALID);
    TEST_ASSERT(notherObj != IUP_INVALID);
    iusHLPulseDelete(obj);
    iusHLPulseDelete(notherObj);

    // invalid params
    obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,NULL);
    TEST_ASSERT(obj == IUP_INVALID);
    iusHLPulseCreate(-1,label);
    TEST_ASSERT(obj == IUP_INVALID);
}

TEST(IusPulse, testIusPulseDelete)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
    TEST_ASSERT(obj != IUP_INVALID);
    int status = iusHLPulseDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusPulse, testIusPulseCompare)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
    iup_t notherObj = iusHLPulseCreate(IUS_NON_PARAMETRIC_PULSETYPE,notherLabel);
    TEST_ASSERT(obj != IUP_INVALID);

    TEST_ASSERT(notherObj != IUP_INVALID);
    equal = iusHLPulseCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLPulseCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    // invalid params
    equal = iusHLPulseCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLPulseCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHLPulseDelete(obj);
    iusHLPulseDelete(notherObj);
}


TEST(IusPulse, testIusPulseGet)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);

    // int param
    TEST_ASSERT_EQUAL(IUS_PARAMETRIC_PULSETYPE,iusHLPulseGetType(obj));
    TEST_ASSERT_EQUAL_STRING(label,iusHLPulseGetLabel(obj));
    

    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_PULSETYPE,iusHLPulseGetType(NULL));
    TEST_ASSERT_EQUAL(NULL,iusHLPulseGetLabel(NULL));
   
    iusHLPulseDelete(obj);
}


TEST(IusPulse, testIusSerialization)
{
  char *filename = "testIusPulseSerialization.hdf5";
  char *pulsePath =  "/Pulse";

  char *label = "label for IUS_PARAMETRIC_PULSETYPE";
  char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

  // create and save
  iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
  iup_t notherObj = iusHLPulseCreate(IUS_NON_PARAMETRIC_PULSETYPE,notherLabel);

  hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  TEST_ASSERT(handle > 0);
  int status = iusHLPulseSave(obj, pulsePath, handle);
  H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK,status);

  // read back
  handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
  iup_t savedObj = iusHLPulseLoad(handle, pulsePath);
  H5Fclose(handle);

  TEST_ASSERT_EQUAL(IUS_TRUE, iusHLPulseCompare(obj,savedObj));
  TEST_ASSERT_EQUAL(IUS_FALSE, iusHLPulseCompare(notherObj,savedObj));
  iusHLPulseDelete(obj);
  iusHLPulseDelete(savedObj);
}


TEST_GROUP_RUNNER(IusPulse)
{
    RUN_TEST_CASE(IusPulse, testIusPulseCreate);
    RUN_TEST_CASE(IusPulse, testIusPulseDelete);
    RUN_TEST_CASE(IusPulse, testIusPulseCompare);
    RUN_TEST_CASE(IusPulse, testIusPulseGet);
    RUN_TEST_CASE(IusPulse, testIusSerialization);
}
