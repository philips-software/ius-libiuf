
//
// Created by nlv09165 on 22/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/iusHL2DTransducerElement.h>
#include <include/iusHL2DTransducerElementListImp.h>
#include <include/ius.h>
#include <iusError.h>

TEST_GROUP(Ius2DTransducerElementList);

TEST_SETUP(Ius2DTransducerElementList)
{
}

TEST_TEAR_DOWN(Ius2DTransducerElementList)
{
}

TEST(Ius2DTransducerElementList, testIus2DTransducerElementListCreate)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    iu2dtel_t _2dTransducerElementList = iusHL2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    TEST_ASSERT_EQUAL(num2DTransducerElements, iusHL2DTransducerElementListGetSize(_2dTransducerElementList));

    _2dTransducerElementList = iusHL2DTransducerElementListCreate(-1);
    TEST_ASSERT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    iusHL2DTransducerElementListDelete(_2dTransducerElementList);
}


TEST(Ius2DTransducerElementList, testIus2DTransducerElementListDelete)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    iu2dtel_t _2dTransducerElementList = iusHL2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    int status = iusHL2DTransducerElementListDelete(_2dTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHL2DTransducerElementListDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

}

TEST(Ius2DTransducerElementList, testIus2DTransducerElementListCompare)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    int status;
    IUS_BOOL equal;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.3f;

    // Empty lists should be equal
    iu2dtel_t _2dTransducerElementList = iusHL2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    iu2dtel_t nother2DTransducerElementList = iusHL2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, nother2DTransducerElementList);
    equal = iusHL2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    // Change one list..add bmode
    status = iusHL2DTransducerElementListSet(_2dTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHL2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = iusHL2DTransducerElementListSet(nother2DTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHL2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iusHL2DTransducerElementListDelete(_2dTransducerElementList);
    iusHL2DTransducerElementDelete(element);
}

TEST(Ius2DTransducerElementList, testIus2DTransducerElementListSetGet)
{
    int i, num2DTransducerElements = 2;
    int status;
    IUS_BOOL equal;

    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch,0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.3f;
    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, elemAngle, elemSize);

    iu2dtel_t _2dTransducerElementList = iusHL2DTransducerElementListCreate(num2DTransducerElements);
    status = iusHL2DTransducerElementListSet(_2dTransducerElementList,element,0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHL2DTransducerElementListSet(NULL,element,0));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHL2DTransducerElementListSet(_2dTransducerElementList,NULL,0));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHL2DTransducerElementListSet(_2dTransducerElementList,element,-1));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHL2DTransducerElementListSet(_2dTransducerElementList,element,num2DTransducerElements));

}

TEST(Ius2DTransducerElementList, testIus2DTransducerElementListSerialization)
{
    int i, num2DTransducerElements = 2;
    int status;
    IUS_BOOL equal;
    char *pFilename = "testIus2DTransducerElementListSerialization.hdf5";
    char *p2DTransducerElementListPath = "/2DTransducerElementList";
    const float transducerPitch = 0.000005f;

    // Non-happy flow
    // for lists it should not be possible to save, if not all elements have been
    // filled.
    iu2dtel_t _2dTransducerElementList = iusHL2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusHL2DTransducerElementListSave(_2dTransducerElementList, p2DTransducerElementListPath, handle);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // Happy Flow
    //
    // fill list
    for (i = 0; i < num2DTransducerElements; i++)
    {
        iu2dp_t elemPos = iusHL2DPositionCreate((10 - num2DTransducerElements / 2)*transducerPitch, 0.0f);
        iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
        float elemAngle = 0.4f;
        iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, elemAngle, elemSize);
        iusHL2DTransducerElementListSet(_2dTransducerElementList, element, i);
    }

    // save
    handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusHL2DTransducerElementListSave(_2dTransducerElementList, p2DTransducerElementListPath, handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iu2dtel_t saved2DTransducerElementList = iusHL2DTransducerElementListLoad(handle, p2DTransducerElementListPath);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, saved2DTransducerElementList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // compare
    equal = iusHL2DTransducerElementListCompare(_2dTransducerElementList, saved2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iusHL2DTransducerElementListDelete(_2dTransducerElementList);


}

TEST_GROUP_RUNNER(Ius2DTransducerElementList)
{
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListDelete);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListCreate);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListSetGet);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListCompare);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListSerialization);
}
