
//
// Created by nlv09165 on 22/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/ius2DTransducerElement.h>
#include <include/ius2DTransducerElementListImp.h>
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
    iu2dtel_t _2dTransducerElementList = ius2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    TEST_ASSERT_EQUAL(num2DTransducerElements, ius2DTransducerElementListGetSize(_2dTransducerElementList));

    _2dTransducerElementList = ius2DTransducerElementListCreate(-1);
    TEST_ASSERT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    ius2DTransducerElementListDelete(_2dTransducerElementList);
}


TEST(Ius2DTransducerElementList, testIus2DTransducerElementListDelete)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    iu2dtel_t _2dTransducerElementList = ius2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    int status = ius2DTransducerElementListDelete(_2dTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = ius2DTransducerElementListDelete(NULL);
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
    iu2dp_t elemPos = ius2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.3f;

    // Empty lists should be equal
    iu2dtel_t _2dTransducerElementList = ius2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    iu2dtel_t nother2DTransducerElementList = ius2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, nother2DTransducerElementList);
    equal = ius2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iu2dte_t element = ius2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    // Change one list..add bmode
    status = ius2DTransducerElementListSet(_2dTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = ius2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = ius2DTransducerElementListSet(nother2DTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = ius2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    ius2DTransducerElementListDelete(_2dTransducerElementList);
    ius2DTransducerElementDelete(element);
}

TEST(Ius2DTransducerElementList, testIus2DTransducerElementListSetGet)
{
    int num2DTransducerElements = 2;
    int status;
    //IUS_BOOL equal;

    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = ius2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch,0.0f);
    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.3f;
    iu2dte_t element = ius2DTransducerElementCreate(elemPos, elemAngle, elemSize);

    iu2dtel_t _2dTransducerElementList = ius2DTransducerElementListCreate(num2DTransducerElements);
    status = ius2DTransducerElementListSet(_2dTransducerElementList,element,0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius2DTransducerElementListSet(NULL,element,0));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius2DTransducerElementListSet(_2dTransducerElementList,NULL,0));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius2DTransducerElementListSet(_2dTransducerElementList,element,-1));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, ius2DTransducerElementListSet(_2dTransducerElementList,element,num2DTransducerElements));

}

TEST(Ius2DTransducerElementList, testIus2DTransducerElementListSerialization)
{
    int i, num2DTransducerElements = 2;
    int status;
    IUS_BOOL equal;
    char *pFilename = "testIus2DTransducerElementListSerialization.hdf5";
    //char *p2DTransducerElementListPath = "/2DTransducerElementList";
    const float transducerPitch = 0.000005f;

    // Non-happy flow
    // for lists it should not be possible to save, if not all elements have been
    // filled.
    iu2dtel_t _2dTransducerElementList = ius2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = ius2DTransducerElementListSave(_2dTransducerElementList, handle); // todo change this test by creating a transducer first?
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // Happy Flow
    //
    // fill list
    for (i = 0; i < num2DTransducerElements; i++)
    {
        iu2dp_t elemPos = ius2DPositionCreate((10 - num2DTransducerElements / 2)*transducerPitch, 0.0f);
        iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
        float elemAngle = 0.4f;
        iu2dte_t element = ius2DTransducerElementCreate(elemPos, elemAngle, elemSize);
        ius2DTransducerElementListSet(_2dTransducerElementList, element, i);
    }

    // save
    handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = ius2DTransducerElementListSave(_2dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iu2dtel_t saved2DTransducerElementList = ius2DTransducerElementListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, saved2DTransducerElementList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // compare
    equal = ius2DTransducerElementListCompare(_2dTransducerElementList, saved2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    ius2DTransducerElementListDelete(_2dTransducerElementList);


}

TEST_GROUP_RUNNER(Ius2DTransducerElementList)
{
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListDelete);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListCreate);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListSetGet);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListCompare);
    RUN_TEST_CASE(Ius2DTransducerElementList, testIus2DTransducerElementListSerialization);
}
