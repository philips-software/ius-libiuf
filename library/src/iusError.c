//
// Created by Ruijzendaal on 30/11/2018.
//
#include <stdarg.h>
#include <memory.h>

#include "ius.h"
#include "library_config.h"

static hid_t iusErrorStack = H5I_INVALID_HID;
static hid_t iusErrorClass;

hid_t  IUS_ERR_MAJ_GENERAL;  //     (100001)   /**< general error */
hid_t  IUS_ERR_MAJ_MEMORY;   //    (100002)    /**< memory error  */
hid_t  IUS_ERR_MAJ_VALUE;    //    (100003)    /**< value error  */
hid_t  IUS_ERR_MAJ_ERROR;

hid_t  IUS_ERR_MIN_ARG_FILENAME;  //     (100001)   /**< general error */
hid_t  IUS_ERR_MIN_ALLOC;
hid_t  IUS_ERR_MIN_FORMAT;

#define ERR_CLS_NAME                    "IUS"
#define IUS_ERR_MAJ_GENERAL_MSG         "General Error"
#define IUS_ERR_MAJ_MEMORY_MSG          "Memory Error"
#define IUS_ERR_MAJ_VALUE_MSG           "Value Error"
#define IUS_ERR_MAJ_ERROR_MSG           "Error handling"
#define IUS_NAME                        "IUS-SDK"
#define IUS_VERSION                     IUS_VERSION_MAJOR "." IUS_VERSION_MINOR "." IUS_VERSION_PATCH


#define IUS_ERR_MIN_ARG_FILENAME_MSG    "Invalid filename argument"
#define IUS_ERR_MIN_ALLOC_MSG           "Memory allocation failed"
#define IUS_ERR_MIN_FORMAT_MSG          "Error message formatting failed"

static int iusErrorInit
(
    void
)
{
    if((iusErrorClass = H5Eregister_class(ERR_CLS_NAME, IUS_NAME, IUS_VERSION)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_GENERAL = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_GENERAL_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_MEMORY = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_MEMORY_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_VALUE = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_VALUE_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_ERROR = H5Ecreate_msg(iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_ERROR_MSG)) < 0)
        return IUS_ERR_VALUE;

    if((IUS_ERR_MIN_ARG_FILENAME = H5Ecreate_msg(iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_FILENAME_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ALLOC = H5Ecreate_msg(iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ALLOC_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_FORMAT = H5Ecreate_msg(iusErrorClass, H5E_MINOR, IUS_ERR_MIN_FORMAT_MSG)) < 0)
        return IUS_ERR_VALUE;

//    if((ERR_MIN_SUBROUTINE = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_SUBROUTINE_MSG)) < 0)
//        TEST_ERROR;
//    if((ERR_MIN_ERRSTACK = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_ERRSTACK_MSG)) < 0)
//        TEST_ERROR;
//    if((ERR_MIN_CREATE = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_CREATE_MSG)) < 0)
//        TEST_ERROR;
//    if((ERR_MIN_WRITE = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_WRITE_MSG)) < 0)
//        TEST_ERROR;
//    if((ERR_MIN_GETNUM = H5Ecreate_msg(iusErrorClass, H5E_MINOR, ERR_MIN_GETNUM_MSG)) < 0)
//        TEST_ERROR;

    iusErrorStack = H5Ecreate_stack();
    return IUS_E_OK;
}

int iusErrorPush
(
    const char *pFileName,
    const char *pFunctionName,
    int lineNumber,
    hid_t maj,
    hid_t min,
    char *msg
)
{
    if (iusErrorStack == H5I_INVALID_HID)
        iusErrorInit();
    int status = H5Epush(iusErrorStack, pFileName, pFunctionName, lineNumber, iusErrorClass, maj, min, strdup(msg));
    return status;
}

int iusErrorFormatAndPush
(
    const char *pFileName,
    const char *pFunctionName,
    int lineNumber,
    hid_t maj,
    hid_t min,
    char *msgFormat,
    ...
)
{
    int count;
    va_list aptr;
    static char msg[IUS_MAX_STRING_LENGTH];
    va_start(aptr, msgFormat);
    count = vsprintf(msg, msgFormat, aptr);
    va_end(aptr);
    if (count<0)
    {
        return IUS_ERROR_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_FORMAT, "unable to format error message");
    }
    return iusErrorPush(pFileName,pFunctionName,lineNumber,maj,min,msg);
}

int iusErrorGetCount
(
    void
)
{
    if (iusErrorStack == H5I_INVALID_HID)
        iusErrorInit();
    return H5Eget_num(iusErrorStack);
}


//
// This routine frees memory allocated by the iusErrorPush routine.
//
/* Note that it uses the following H5E_error_t struct, which is updated from
 * the current H5E_error_t:
 *
 * typedef struct H5E_error_t {
 *    hid_t 	  cls_id;   // Error class ID
 *    hid_t 	  maj_id;	// Major error ID
 *    hid_t       min_id;	// Minor error ID
 *    const char *func_name;    // Function in which error occurred
 *    const char *file_name;    // File in which error occurred
 *    unsigned	  line;		 // Line in file where error occurs
 *    const char *desc;		 //  Optional supplied description
 * } H5E_error_t;
 */
#if 0
static herr_t iusErrorCleanupCallback(unsigned int n, const H5E_error2_t *err_desc, void *cb_data)
{
    herr_t error=0;
    char className[IUS_MAX_STRING_LENGTH];
    char majorName[IUS_MAX_STRING_LENGTH];
    char minorName[IUS_MAX_STRING_LENGTH];

    IUS_UNUSED(n);
    IUS_UNUSED(cb_data);

    /* Get the error class's name */
    error = H5Eget_class_name(err_desc->cls_id, className, IUS_MAX_STRING_LENGTH);
    if( error < 0) return error;

    /* Get the major error description */
    error = H5Eget_msg(err_desc->maj_num, NULL, majorName, IUS_MAX_STRING_LENGTH);
    if( error < 0) return error;

    /* Get the minor error description */
    error = H5Eget_msg(err_desc->min_num, NULL, minorName, IUS_MAX_STRING_LENGTH);
    if( error < 0) return error;

    printf("=====> %s\n", err_desc->desc);
    /* Print error information */
    free((void *)err_desc->desc);
    return error;
}
#endif
int iusErrorClear()
{
    herr_t status = 0;//H5Ewalk2(iusErrorStack, H5E_WALK_UPWARD, &iusErrorCleanupCallback, NULL);
//    status |= H5Eclear2(iusErrorStack);
    return (int) status;
}

int iusErrorPrint
(
    FILE *pFILE
)
{
    if (iusErrorStack == H5I_INVALID_HID)
        iusErrorInit();

    herr_t status = H5Eprint2(iusErrorStack, pFILE);
    status |= iusErrorClear();
    return (int) status;
}
