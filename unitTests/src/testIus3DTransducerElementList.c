
//
// Created by nlv09165 on 21/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <ius3DTransducerElementListPrivate.h>

TEST_GROUP(Ius3DTransducerElementList);

TEST_SETUP(Ius3DTransducerElementList)
{
}

TEST_TEAR_DOWN(Ius3DTransducerElementList)
{
}

TEST(Ius3DTransducerElementList, testIus3DTransducerElementListCreate)
{
    // Put your test code here
    int num3DTransducerElements = 100;
    iu3dtel_t _3dTransducerElementList = ius3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    TEST_ASSERT_EQUAL(num3DTransducerElements, ius3DTransducerElementListGetSize(_3dTransducerElementList));

    _3dTransducerElementList = ius3DTransducerElementListCreate(-1);
    TEST_ASSERT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    ius3DTransducerElementListDelete(_3dTransducerElementList);
}


TEST(Ius3DTransducerElementList, testIus3DTransducerElementListDelete)
{
    // Put your test code here
    int num3DTransducerElements = 100;
    iu3dtel_t _3dTransducerElementList = ius3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    int status = ius3DTransducerElementListDelete(_3dTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = ius3DTransducerElementListDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

}

TEST(Ius3DTransducerElementList, testIus3DTransducerElementListCompare)
{
    // Put your test code here
    int num3DTransducerElements = 100;
    int status;
    IUS_BOOL equal;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);

    // Empty lists should be equal
    iu3dtel_t _3dTransducerElementList = ius3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    iu3dtel_t nother3DTransducerElementList = ius3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, nother3DTransducerElementList);
    equal = ius3DTransducerElementListCompare(_3dTransducerElementList, nother3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    // Change one list..add bmode
    status = ius3DTransducerElementListSet(_3dTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = ius3DTransducerElementListCompare(_3dTransducerElementList, nother3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = ius3DTransducerElementListSet(nother3DTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = ius3DTransducerElementListCompare(_3dTransducerElementList, nother3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    ius3DTransducerElementListDelete(_3dTransducerElementList);
    ius3DTransducerElementDelete(element);
}

TEST(Ius3DTransducerElementList, testIus3DTransducerElementListSetGet)
{
    int num3DTransducerElements = 2;
    int status;
    //IUS_BOOL equal;

    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);

    iu3dtel_t _3dTransducerElementList = ius3DTransducerElementListCreate(num3DTransducerElements);
    status = ius3DTransducerElementListSet(_3dTransducerElementList,element,0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius3DTransducerElementListSet(NULL,element,0));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius3DTransducerElementListSet(_3dTransducerElementList,NULL,0));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius3DTransducerElementListSet(_3dTransducerElementList,element,-1));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius3DTransducerElementListSet(_3dTransducerElementList,element,num3DTransducerElements));

}

TEST(Ius3DTransducerElementList, testIus3DTransducerElementListSerialization)
{
    int i, num3DTransducerElements = 2;
    int status;
    IUS_BOOL equal;
    char *pFilename = "testIus3DTransducerElementListSerialization.hdf5";
    //char *p3DTransducerElementListPath = "/3DTransducerElementList";
    const float transducerPitch = 0.000005f;

    // Non-happy flow
    // for lists it should not be possible to save, if not all elements have been
    // filled.
    iu3dtel_t _3dTransducerElementList = ius3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = ius3DTransducerElementListSave(_3dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // Happy Flow
    //
    // fill list
    for (i = 0; i < num3DTransducerElements; i++)
    {
        iu3dp_t elemPos = ius3DPositionCreate((float)(10 - num3DTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
        iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
        ius3DTransducerElementListSet(_3dTransducerElementList, element, i);
    }

    // save
    handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = ius3DTransducerElementListSave(_3dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iu3dtel_t saved3DTransducerElementList = ius3DTransducerElementListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, saved3DTransducerElementList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // compare
    equal = ius3DTransducerElementListCompare(_3dTransducerElementList, saved3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    ius3DTransducerElementListDelete(_3dTransducerElementList);

}

TEST_GROUP_RUNNER(Ius3DTransducerElementList)
{
    RUN_TEST_CASE(Ius3DTransducerElementList, testIus3DTransducerElementListDelete);
    RUN_TEST_CASE(Ius3DTransducerElementList, testIus3DTransducerElementListCreate);
    RUN_TEST_CASE(Ius3DTransducerElementList, testIus3DTransducerElementListSetGet);
    RUN_TEST_CASE(Ius3DTransducerElementList, testIus3DTransducerElementListCompare);
    RUN_TEST_CASE(Ius3DTransducerElementList, testIus3DTransducerElementListSerialization);
}
