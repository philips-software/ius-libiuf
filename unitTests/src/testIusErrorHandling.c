//
// Created by Ruijzendaal on 26/11/2018.
//

#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <memory.h>

#include "ius.h"

TEST_GROUP(IusErrorHandling);

TEST_SETUP(IusErrorHandling)
{
}

TEST_TEAR_DOWN(IusErrorHandling)
{
}


int
my_hdf5_error_handler(int a, void *unused)
{
    IUS_UNUSED(a);
    IUS_UNUSED(unused);

    fprintf (stderr, "An HDF5 error was detected. Bye.\n");
    exit (1);
}



#define DIM0    100
#define DIM1    200


hid_t   iusErrorClass;
hid_t   ERR_CLS2;
hid_t   ERR_STACK;

hid_t   ERR_MAJ_TEST;
hid_t   ERR_MAJ_IO;
hid_t   ERR_MAJ_API;

hid_t   ERR_MIN_SUBROUTINE;
hid_t   ERR_MIN_ERRSTACK;
hid_t   ERR_MIN_CREATE;
hid_t   ERR_MIN_WRITE;
hid_t   ERR_MIN_GETNUM;

#define DSET_NAME               "a_dataset"
#define FAKE_ID                 (hid_t)0

#define ERR_CLS_NAME            "Error Class Name"
#define ERR_CLS2_NAME           "Second Test"
#define PROG_NAME               "Error Program"
#define PROG2_NAME              "Second Program"
#define PROG_VERS               "1.0"

#define ERR_MAJ_TEST_MSG             "Error in test"
#define ERR_MAJ_IO_MSG               "Error in IO"
#define ERR_MAJ_API_MSG              "Error in API"
#define ERR_MIN_SUBROUTINE_MSG       "Error in subroutine"
#define ERR_MIN_ERRSTACK_MSG         "Error in error stack"
#define ERR_MIN_CREATE_MSG           "Error in H5Dcreate2"
#define ERR_MIN_WRITE_MSG            "Error in H5Dwrite"
#define ERR_MIN_GETNUM_MSG           "Error in H5Eget_num"

#define MSG_SIZE                64
#define LONG_DESC_SIZE          8192

#define HDstrlen strlen
#define HDmalloc malloc
#define HDfree free
#define HDstrcmp strcmp
#define TEST_ERROR goto error
#define HDfprintf fprintf

/*-------------------------------------------------------------------------
 * Function:	init_error
 *
 * Purpose:	Initialize error information.
 *
 * Return:	Success:	0
 *
 *		Failure:	-1
 *
 * Programmer:	Raymond Lu
 *		July 10, 2003
 *
 *-------------------------------------------------------------------------
 */
static herr_t
init_error(void)
{
    ssize_t cls_size = (ssize_t)HDstrlen(ERR_CLS_NAME) + 1;
    ssize_t msg_size = (ssize_t)HDstrlen(ERR_MIN_SUBROUTINE_MSG) + 1;
    char   *cls_name = NULL;
    char   *msg = NULL;
    H5E_type_t msg_type;

    if(NULL == (cls_name = (char *)HDmalloc(HDstrlen(ERR_CLS_NAME) + 1)))
        TEST_ERROR;
    if(NULL == (msg = (char *)HDmalloc(HDstrlen(ERR_MIN_SUBROUTINE_MSG) + 1)))
        TEST_ERROR;

    if((iusErrorClass = H5Eregister_class(ERR_CLS_NAME, PROG_NAME, PROG_VERS)) < 0)
        TEST_ERROR;

    if(cls_size != H5Eget_class_name(iusErrorClass, cls_name, (size_t)cls_size) + 1)
        TEST_ERROR;
    if(HDstrcmp(ERR_CLS_NAME, cls_name))
        TEST_ERROR;

    if((ERR_MAJ_TEST = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, ERR_MAJ_TEST_MSG)) < 0)
        TEST_ERROR;
    if((ERR_MAJ_IO = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, ERR_MAJ_IO_MSG)) < 0)
        TEST_ERROR;
    if((ERR_MAJ_API = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, ERR_MAJ_API_MSG)) < 0)
        TEST_ERROR;

    if((ERR_MIN_SUBROUTINE = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_SUBROUTINE_MSG)) < 0)
        TEST_ERROR;
    if((ERR_MIN_ERRSTACK = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_ERRSTACK_MSG)) < 0)
        TEST_ERROR;
    if((ERR_MIN_CREATE = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_CREATE_MSG)) < 0)
        TEST_ERROR;
    if((ERR_MIN_WRITE = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_WRITE_MSG)) < 0)
        TEST_ERROR;
    if((ERR_MIN_GETNUM = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_GETNUM_MSG)) < 0)
        TEST_ERROR;

    if(msg_size != H5Eget_msg(ERR_MIN_SUBROUTINE, &msg_type, msg, (size_t)msg_size) + 1)
        TEST_ERROR;
    if(msg_type != H5E_MINOR)
        TEST_ERROR;
    if(HDstrcmp(msg, ERR_MIN_SUBROUTINE_MSG))
        TEST_ERROR;

    /* Register another class for later testing. */
    if((ERR_CLS2 = H5Eregister_class(ERR_CLS2_NAME, PROG2_NAME, PROG_VERS)) < 0)
        TEST_ERROR;

    HDfree(cls_name);
    HDfree(msg);

    return 0;

    error:
    if(cls_name)
        HDfree(cls_name);
    if(msg)
        HDfree(msg);

    return -1;
} /* end init_error() */


/*-------------------------------------------------------------------------
 * Function:    custom_print_cb
 *
 * Purpose:	Callback function to print error stack in customized way.
 *
 * Return:	Success:	0
 *
 *		Failure:	-1
 *
 * Programmer:	Raymond Lu
 *		July 17, 2003
 *
 *-------------------------------------------------------------------------
 */
static herr_t
custom_print_cb(unsigned n, const H5E_error2_t *err_desc, void* client_data)
{
    FILE		*stream  = (FILE *)client_data;
    char                maj[MSG_SIZE];
    char                min[MSG_SIZE];
    char                cls[MSG_SIZE];
    const int		indent = 4;

    /* Get descriptions for the major and minor error numbers */
    if(H5Eget_class_name(err_desc->cls_id, cls, MSG_SIZE) < 0)
        TEST_ERROR;

    if(H5Eget_msg(err_desc->maj_num, NULL, maj, MSG_SIZE) < 0)
        TEST_ERROR;

    if(H5Eget_msg(err_desc->min_num, NULL, min, MSG_SIZE) < 0)
        TEST_ERROR;

    HDfprintf(stream, "%*serror #%03d: %s in %s(): line %u\n",
              indent, "", n, err_desc->file_name,
              err_desc->func_name, err_desc->line);
    HDfprintf(stream, "%*sclass: %s\n", indent * 2, "", cls);
    HDfprintf(stream, "%*smajor: %s\n", indent * 2, "", maj);
    HDfprintf(stream, "%*sminor: %s\n", indent * 2, "", min);

    return 0;

    error:
    return -1;
} /* end custom_print_cb() */


/*-------------------------------------------------------------------------
 * Function:    dump_error
 *
 * Purpose:	Prints error stack in default and customized ways.
 *
 * Return:	Success:	0
 *
 *		Failure:	-1
 *
 * Programmer:	Raymond Lu
 *		July 17, 2003
 *
 *-------------------------------------------------------------------------
 */
static herr_t
dump_error(hid_t estack)
{
    /* Print errors in library default way */
    HDfprintf(stderr, "********* Print error stack in HDF5 default way *********\n");
    if(H5Eprint2(estack, stderr) < 0)
        TEST_ERROR;

    /* Customized way to print errors */
    HDfprintf(stderr, "\n********* Print error stack in customized way *********\n");
    if(H5Ewalk2(estack, H5E_WALK_UPWARD, custom_print_cb, stderr) < 0)
        TEST_ERROR;

    return 0;

    error:
    return -1;
} /* end dump_error() */


hid_t   ERR_STACK;

TEST(IusErrorHandling, testIusErrorHandlerCreate)
{
//    const char          *testIusErrorHandlerCreate = "testIusErrorHandlerCreate";
    const char          *testIusErrorHandlerCreate = __FUNCTION__;

    /* Initialize errors */
    int status = init_error();
    TEST_ASSERT(status >= 0);
    hid_t estack = 0;
    /* Push an error onto error stack */
    status = H5Epush(estack, __FILE__, testIusErrorHandlerCreate, __LINE__, iusErrorClass, ERR_MAJ_TEST, ERR_MIN_ERRSTACK,
               "Error stack test failed");
    TEST_ASSERT(status >= 0);
    /* Print out the errors on stack */
    dump_error(estack);


}



TEST(IusErrorHandling, testIusErrorUsageScenario)
{
    // Force error condition
    char *pTestFileName = "nonexistingfilename.hdf5";
    char *pErrprFileName = "nonexistingfilename.errlog";
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);

    // Assert error stack is filled
    numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    // Validate some error has been reported by
    // opening an errorlogfile and
    // checking whether file position has been
    // changed.
    FILE *fp = fopen(pErrprFileName, "w+");
    long filePos = ftell(fp);
    int status = iusErrorPrint(fp);
    remove(pTestFileName);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));
//    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}


TEST_GROUP_RUNNER(IusErrorHandling)
{
//    RUN_TEST_CASE(IusErrorHandling, testIusErrorHandlerCreate);
    RUN_TEST_CASE(IusErrorHandling, testIusErrorUsageScenario);
}
