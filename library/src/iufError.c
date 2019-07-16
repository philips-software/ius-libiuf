#include <stdarg.h>
#include <memory.h>
#include <string.h>

#include "iuf.h"
#include "library_config.h"


hid_t  IUF_ERR_MAJ_GENERAL;  //     (100001)   /**< general error */
hid_t  IUF_ERR_MAJ_MEMORY;   //    (100002)    /**< memory error  */
hid_t  IUF_ERR_MAJ_VALUE;    //    (100003)    /**< value error  */
hid_t  IUF_ERR_MAJ_ERROR;
hid_t  IUF_ERR_MAJ_HDF5;

hid_t  IUF_ERR_MIN_ARG_FILENAME;  //     (100001)   /**< general error */
hid_t  IUF_ERR_MIN_ALLOC;
hid_t  IUF_ERR_MIN_MEMCOPY;

hid_t  IUF_ERR_MIN_FORMAT;
hid_t  IUF_ERR_MIN_ARG_NULL_VALUE;
hid_t  IUF_ERR_MIN_ARG_DUPLICATE_KEY;
hid_t  IUF_ERR_MIN_ARG_INVALID_KEY;
hid_t  IUF_ERR_MIN_HDF5;
hid_t  IUF_ERR_MIN_ARG_VALUE;
hid_t  IUF_ERR_MIN_ASSERT_FAILED;

#define ERR_CLS_NAME                    "IUF"
#define IUF_ERR_MAJ_HDF5_MSG            "HDF5 call failed"
#define IUF_ERR_MAJ_GENERAL_MSG         "General Error"
#define IUF_ERR_MAJ_MEMORY_MSG          "Memory Error"
#define IUF_ERR_MAJ_VALUE_MSG           "Value Error"
#define IUF_ERR_MAJ_ERROR_MSG           "Error handling"
#define IUF_NAME                        "IUF-SDK"
#define IUF_VERSION                     IUF_VERSION_MAJOR "." IUF_VERSION_MINOR "." IUF_VERSION_PATCH


#define IUF_ERR_MIN_ARG_VALUE_MSG           "Value invalid for argument"
#define IUF_ERR_MIN_ARG_NULL_VALUE_MSG      "NULL value invalid for argument"
#define IUF_ERR_MIN_ARG_FILENAME_MSG        "Invalid filename argument"
#define IUF_ERR_MIN_ALLOC_MSG               "Memory allocation failed"
#define IUF_ERR_MIN_MEMCOPY_MSG             "Memory copy failed"
#define IUF_ERR_MIN_FORMAT_MSG              "Error message formatting failed"
#define IUF_ERR_MIN_ARG_DUPLICATE_KEY_MSG   "Duplicate key"
#define IUF_ERR_MIN_ARG_INVALID_KEY_MSG     "Invalid key, lookup failed"
#define IUF_ERR_MIN_HDF5_MSG                "Turn on HDF5 error log to get error details: iufHDF5ErrorLog(IUF_TRUE)"
#define IUF_ERR_MIN_ASSERT_FAILED_MSG       "Assertion failed"

struct IufError
{
    hid_t iufErrorStack;
    hid_t iufErrorClass;
    IUF_BOOL enable;
    IUF_BOOL autoReport;
    FILE *errorStream;
    // HDF5 error handling
    H5E_auto2_t func;
    void *client_data;
    char error_buffer[IUF_MAX_STRING_LENGTH];

};

// ADT
typedef struct IufError IufError;
typedef IufError *iue_t;
#define  IUE_INVALID (iue_t) NULL

static IufError iufErrorState = {
    .iufErrorStack = H5I_INVALID_HID,
    .iufErrorClass = H5I_INVALID_HID,
    .enable = IUF_TRUE,
    .autoReport = IUF_FALSE,
    .func = NULL,
    .client_data = NULL,
    .errorStream = NULL
};



static int iufErrorInit
(
    iue_t state
)
{
    if((state->iufErrorClass = H5Eregister_class(ERR_CLS_NAME, IUF_NAME, IUF_VERSION)) < 0)
        return IUF_ERR_VALUE;

    // Register Major Message categories
    if((IUF_ERR_MAJ_GENERAL = H5Ecreate_msg(state->iufErrorClass, H5E_MAJOR, IUF_ERR_MAJ_GENERAL_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MAJ_MEMORY = H5Ecreate_msg(state->iufErrorClass, H5E_MAJOR, IUF_ERR_MAJ_MEMORY_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MAJ_VALUE = H5Ecreate_msg(state->iufErrorClass, H5E_MAJOR, IUF_ERR_MAJ_VALUE_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MAJ_ERROR = H5Ecreate_msg(state->iufErrorClass, H5E_MAJOR, IUF_ERR_MAJ_ERROR_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MAJ_HDF5 = H5Ecreate_msg(state->iufErrorClass, H5E_MAJOR, IUF_ERR_MAJ_HDF5_MSG)) < 0)
        return IUF_ERR_VALUE;


    // Register Minor Message categories
    if((IUF_ERR_MIN_ARG_FILENAME = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ARG_FILENAME_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_ALLOC = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ALLOC_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_MEMCOPY = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_MEMCOPY_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_FORMAT = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_FORMAT_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_ARG_NULL_VALUE = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ARG_NULL_VALUE_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_ARG_VALUE = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ARG_VALUE_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_ARG_DUPLICATE_KEY = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ARG_DUPLICATE_KEY_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_ARG_INVALID_KEY = H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ARG_INVALID_KEY_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_HDF5= H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_HDF5_MSG)) < 0)
        return IUF_ERR_VALUE;
    if((IUF_ERR_MIN_ASSERT_FAILED= H5Ecreate_msg(state->iufErrorClass, H5E_MINOR, IUF_ERR_MIN_ASSERT_FAILED_MSG)) < 0)
        return IUF_ERR_VALUE;


    state->iufErrorStack = H5Ecreate_stack();
    state->errorStream = stderr;
    state->error_buffer[0] = '\0';
    H5Eget_auto2(H5E_DEFAULT,&state->func,&state->client_data);
    iufHDF5ErrorLog(IUF_FALSE);
    return IUF_E_OK;
}

static iue_t iufErrorGetState
(
    void
)
{
    if (iufErrorState.iufErrorStack == H5I_INVALID_HID)
        iufErrorInit(&iufErrorState);
    return &iufErrorState;
}

int iufErrorPush
(
    const char *pFileName,
    const char *pFunctionName,
    int lineNumber,
    hid_t maj,
    hid_t min,
    char *msg
)
{
    iue_t state = iufErrorGetState();
    int status = H5Epush(state->iufErrorStack, pFileName, pFunctionName, lineNumber, state->iufErrorClass, maj, min, msg);
    if (state->autoReport == IUF_TRUE)
    {
        iufErrorPrint(state->errorStream);
    }
    return status;
}

int iufErrorFormatAndPush
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
    static char msg[IUF_MAX_STRING_LENGTH];
    va_start(aptr, msgFormat);
    count = vsprintf(msg, msgFormat, aptr);
    va_end(aptr);
    if (count<0)
    {
        return IUF_ERROR_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_FORMAT, "unable to format error message");
    }
    return iufErrorPush(pFileName,pFunctionName,lineNumber,maj,min,msg);
}


int iufErrorGetCount
(
    void
)
{
    iue_t state = iufErrorGetState();
    return (int) H5Eget_num(state->iufErrorStack);
}

/* This routine prints the information for an error on the error stack */
/* Note that it uses the following H5E_error_t struct, which is updated from
 * the current H5E_error_t:
 *
 * typedef struct H5E_error_t {
 *    hid_t 	  cls_id;	// Error class ID
 *    hid_t 	  maj_id;	// Major error ID
 *    hid_t       min_id;	// Minor error ID
 *    const char *func_name;	// Function in which error occurred
 *    const char *file_name;	// File in which error occurred
 *    unsigned	  line;		// Line in file where error occurs
 *    const char *desc;		// Optional supplied description
 * } H5E_error_t;
 *
 */
int build_error_string(unsigned int n, const struct H5E_error2_t *err_desc, void *cb_data)
{
    char class_name[32];
    const char *maj_name;
    const char *min_name;
    char *fillme = (char *)cb_data;

    IUF_UNUSED(cb_data);
    /* Get the error class's name */
    H5Eget_class_name(err_desc->cls_id,class_name,32);

    /* Get the major error description */
    maj_name = H5Eget_major (err_desc->maj_num);
    min_name = H5Eget_minor (err_desc->min_num);

    /* Print error information */
    sprintf(fillme + strlen(fillme), "%s-DIAG: Error detected in IUF-SDK:\n", class_name);
    sprintf(fillme + strlen(fillme), "  #%03d: %s line %d in %s(): %s\n",
                                     n,
                                     err_desc->file_name,
                                     err_desc->line,
                                     err_desc->func_name,
                                     err_desc->desc);
    sprintf(fillme + strlen(fillme), "   major: %s\n", maj_name);
    sprintf(fillme + strlen(fillme), "   minor: %s\n", min_name);
    return 0;
}

int iufErrorPrint
(
)
{
    iue_t state = iufErrorGetState();
    herr_t status = 0;
    if(state->enable == IUF_TRUE)
    {
        status = H5Eprint2(state->iufErrorStack, state->errorStream);
    }
    return (int) status;
}

int iufErrorLogClear
(
    void
)
{
    iue_t state = iufErrorGetState();
    herr_t status = H5Eclear2(H5E_DEFAULT);
    status |= H5Eclear2(state->iufErrorStack);
    state->error_buffer[0] = '\0';
    return (int) status;
}

char *iufErrorString()
{
    // Turn off error handling permanently
    iue_t state = iufErrorGetState();
    H5Ewalk2(state->iufErrorStack, H5E_WALK_UPWARD, build_error_string, state->error_buffer);
    return state->error_buffer;
}

int iufErrorLog(IUF_BOOL enable)
{
    // Turn off error handling permanently
    iue_t state = iufErrorGetState();
    state->enable = enable;
    return IUF_E_OK;
}

int iufHDF5ErrorLog(IUF_BOOL enable)
{
    iue_t state = iufErrorGetState();
    int status = 0;
    if ( enable == IUF_TRUE )
    {
        status = H5Eset_auto2(H5E_DEFAULT, state->func, state->client_data);
    }
    else
    {
        status = H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
    }
    return status;
}

IUF_BOOL iufErrorAutoReportGet
(
    void
)
{
    iue_t state = iufErrorGetState();
    return state->autoReport;
}

int iufErrorAutoReportSet
(
    IUF_BOOL enable
)
{
    iue_t state = iufErrorGetState();
    state->autoReport = enable;
    return IUF_E_OK;
}

int iufErrorSetStream
(
    FILE *stream
)
{
    iue_t state = iufErrorGetState();
    state->errorStream = stream;
    return 0;
}

int iufErrorSetStreamToFile
(
    const char * pFileName
)
{
    FILE * fp = fopen(pFileName, "w+");
    if (fp == NULL)
    {
        return 1;
    }
    iufErrorSetStream(fp);
    return 0;
}

int iufErrorCloseFileStream
(
)
{
    iue_t state = iufErrorGetState();
    int rv = fclose(state->errorStream);
    state->errorStream = stderr;
    return rv;
}
