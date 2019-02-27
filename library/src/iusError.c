//
// Created by Ruijzendaal on 30/11/2018.
//
#include <stdarg.h>
#include <memory.h>

#include "ius.h"
#include "library_config.h"


hid_t  IUS_ERR_MAJ_GENERAL;  //     (100001)   /**< general error */
hid_t  IUS_ERR_MAJ_MEMORY;   //    (100002)    /**< memory error  */
hid_t  IUS_ERR_MAJ_VALUE;    //    (100003)    /**< value error  */
hid_t  IUS_ERR_MAJ_ERROR;
hid_t  IUS_ERR_MAJ_HDF5;

hid_t  IUS_ERR_MIN_ARG_FILENAME;  //     (100001)   /**< general error */
hid_t  IUS_ERR_MIN_ALLOC;
hid_t  IUS_ERR_MIN_MEMCOPY;

hid_t  IUS_ERR_MIN_FORMAT;
hid_t  IUS_ERR_MIN_ARG_NULL_VALUE;
hid_t  IUS_ERR_MIN_ARG_DUPLICATE_KEY;
hid_t  IUS_ERR_MIN_ARG_INVALID_KEY;
hid_t  IUS_ERR_MIN_HDF5;
hid_t  IUS_ERR_MIN_ARG_VALUE;
hid_t  IUS_ERR_MIN_ASSERT_FAILED;

#define ERR_CLS_NAME                    "IUS"
#define IUS_ERR_MAJ_HDF5_MSG            "HDF5 call failed"
#define IUS_ERR_MAJ_GENERAL_MSG         "General Error"
#define IUS_ERR_MAJ_MEMORY_MSG          "Memory Error"
#define IUS_ERR_MAJ_VALUE_MSG           "Value Error"
#define IUS_ERR_MAJ_ERROR_MSG           "Error handling"
#define IUS_NAME                        "IUS-SDK"
#define IUS_VERSION                     IUS_VERSION_MAJOR "." IUS_VERSION_MINOR "." IUS_VERSION_PATCH


#define IUS_ERR_MIN_ARG_VALUE_MSG           "Value invalid for argument"
#define IUS_ERR_MIN_ARG_NULL_VALUE_MSG      "NULL value invalid for argument"
#define IUS_ERR_MIN_ARG_FILENAME_MSG        "Invalid filename argument"
#define IUS_ERR_MIN_ALLOC_MSG               "Memory allocation failed"
#define IUS_ERR_MIN_MEMCOPY_MSG             "Memory copy failed"
#define IUS_ERR_MIN_FORMAT_MSG              "Error message formatting failed"
#define IUS_ERR_MIN_ARG_DUPLICATE_KEY_MSG   "Duplicate key"
#define IUS_ERR_MIN_ARG_INVALID_KEY_MSG     "Invalid key, lookup failed"
#define IUS_ERR_MIN_HDF5_MSG                "Turn on HDF5 error log to get error details: iusHDF5ErrorLog(IUS_TRUE)"
#define IUS_ERR_MIN_ASSERT_FAILED_MSG       "Assertion failed"

struct IusError
{
    hid_t iusErrorStack;
    hid_t iusErrorClass;
    IUS_BOOL enable;
    IUS_BOOL autoReport;
    FILE *errorStream;
    // HDF5 error handling
    H5E_auto2_t func;
    void *client_data;
};

// ADT
typedef struct IusError IusError;
typedef IusError *iue_t;
#define  IUE_INVALID (iue_t) NULL

static IusError iusErrorState = {
    .iusErrorStack = H5I_INVALID_HID,
    .iusErrorClass = H5I_INVALID_HID,
    .enable = IUS_TRUE,
    .autoReport = IUS_FALSE,
    .func = NULL,
    .client_data = NULL,
    .errorStream = NULL
};



static int iusErrorInit
(
    iue_t state
)
{
    if((state->iusErrorClass = H5Eregister_class(ERR_CLS_NAME, IUS_NAME, IUS_VERSION)) < 0)
        return IUS_ERR_VALUE;

    // Register Major Message categories
    if((IUS_ERR_MAJ_GENERAL = H5Ecreate_msg(state->iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_GENERAL_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_MEMORY = H5Ecreate_msg(state->iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_MEMORY_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_VALUE = H5Ecreate_msg(state->iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_VALUE_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_ERROR = H5Ecreate_msg(state->iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_ERROR_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MAJ_HDF5 = H5Ecreate_msg(state->iusErrorClass, H5E_MAJOR, IUS_ERR_MAJ_HDF5_MSG)) < 0)
        return IUS_ERR_VALUE;


    // Register Minor Message categories
    if((IUS_ERR_MIN_ARG_FILENAME = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_FILENAME_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ALLOC = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ALLOC_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_MEMCOPY = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_MEMCOPY_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_FORMAT = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_FORMAT_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ARG_NULL_VALUE = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_NULL_VALUE_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ARG_VALUE = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_VALUE_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ARG_DUPLICATE_KEY = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_DUPLICATE_KEY_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ARG_INVALID_KEY = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_INVALID_KEY_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_HDF5= H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_HDF5_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ASSERT_FAILED= H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ASSERT_FAILED_MSG)) < 0)
        return IUS_ERR_VALUE;


    state->iusErrorStack = H5Ecreate_stack();
    state->errorStream = stderr;
    H5Eget_auto2(H5E_DEFAULT,&state->func,&state->client_data);
    iusHDF5ErrorLog(IUS_FALSE);
    return IUS_E_OK;
}

static iue_t iusErrorGetState
(
    void
)
{
    if (iusErrorState.iusErrorStack == H5I_INVALID_HID)
        iusErrorInit(&iusErrorState);
    return &iusErrorState;
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
    iue_t state = iusErrorGetState();
    int status = H5Epush(state->iusErrorStack, pFileName, pFunctionName, lineNumber, state->iusErrorClass, maj, min, msg);
    if (state->autoReport == IUS_TRUE)
    {
        iusErrorPrint(state->errorStream);
    }
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
    iue_t state = iusErrorGetState();
    return (int) H5Eget_num(state->iusErrorStack);
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

    IUS_UNUSED(cb_data);
    /* Get the error class's name */
    H5Eget_class_name(err_desc->cls_id,class_name,32);

    /* Get the major error description */
    maj_name = H5Eget_major (err_desc->maj_num);
    min_name = H5Eget_minor (err_desc->min_num);

    /* Print error information */
    sprintf(fillme + strlen(fillme), "%s-DIAG: Error detected in IUS-SDK:\n", class_name);
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

int iusErrorPrint
(
)
{
    iue_t state = iusErrorGetState();
    herr_t status = 0;
    if(state->enable == IUS_TRUE)
    {
        status = H5Eprint2(state->iusErrorStack, state->errorStream);
    }
    return (int) status;
}

int iusErrorLogClear
(
    void
)
{
    iue_t state = iusErrorGetState();
    herr_t status = H5Eclear2(H5E_DEFAULT);
    status |= H5Eclear2(state->iusErrorStack);
    return (int) status;
}

char *iusErrorString()
{
    static char buffer[IUS_MAX_STRING_LENGTH];
    // Turn off error handling permanently
    iue_t state = iusErrorGetState();
    H5Ewalk2(state->iusErrorStack, H5E_WALK_UPWARD, build_error_string, buffer);
    return strdup(buffer);
}

int iusErrorLog(IUS_BOOL enable)
{
    // Turn off error handling permanently
    iue_t state = iusErrorGetState();
    state->enable = enable;
    return IUS_E_OK;
}

int iusHDF5ErrorLog(IUS_BOOL enable)
{
    iue_t state = iusErrorGetState();
    int status = 0;
    if ( enable == IUS_TRUE )
    {
        status = H5Eset_auto2(H5E_DEFAULT, state->func, state->client_data);
    }
    else
    {
        status = H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
    }
    return status;
}

IUS_BOOL iusErrorAutoReportGet
(
    void
)
{
    iue_t state = iusErrorGetState();
    return state->autoReport;
}

int iusErrorAutoReportSet
(
    IUS_BOOL enable
)
{
    iue_t state = iusErrorGetState();
    state->autoReport = enable;
    return IUS_E_OK;
}

int iusErrorSetStream
(
    FILE *stream
)
{
    iue_t state = iusErrorGetState();
    state->errorStream = stream;
    return 0;
}

int iusErrorSetStreamToFile
(
    const char * pFileName
)
{
    FILE * fp = fopen(pFileName, "w+");
    if (fp == NULL)
    {
        return 1;
    }
    iusErrorSetStream(fp);
    return 0;
}

int iusErrorCloseFileStream
(
)
{
    iue_t state = iusErrorGetState();
    int rv = fclose(state->errorStream);
    state->errorStream = stderr;
    return rv;
}
