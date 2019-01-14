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

hid_t  IUS_ERR_MIN_ARG_FILENAME;  //     (100001)   /**< general error */
hid_t  IUS_ERR_MIN_ALLOC;
hid_t  IUS_ERR_MIN_FORMAT;
hid_t  IUS_ERR_MIN_ARG_NULL_VALUE;
extern hid_t  IUS_ERR_MIN_ARG_DUPLICATE_KEY;

#define ERR_CLS_NAME                    "IUS"
#define IUS_ERR_MAJ_GENERAL_MSG         "General Error"
#define IUS_ERR_MAJ_MEMORY_MSG          "Memory Error"
#define IUS_ERR_MAJ_VALUE_MSG           "Value Error"
#define IUS_ERR_MAJ_ERROR_MSG           "Error handling"
#define IUS_NAME                        "IUS-SDK"
#define IUS_VERSION                     IUS_VERSION_MAJOR "." IUS_VERSION_MINOR "." IUS_VERSION_PATCH


#define IUS_ERR_MIN_ARG_NULL_VALUE_MSG  "NULL value invalid for argument"
#define IUS_ERR_MIN_ARG_FILENAME_MSG    "Invalid filename argument"
#define IUS_ERR_MIN_ALLOC_MSG           "Memory allocation failed"
#define IUS_ERR_MIN_FORMAT_MSG          "Error message formatting failed"


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
    .autoReport = IUS_TRUE,
    .func = NULL,
    .client_data = NULL
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

    // Register Minor Message categories
    if((IUS_ERR_MIN_ARG_FILENAME = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_FILENAME_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ALLOC = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ALLOC_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_FORMAT = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_FORMAT_MSG)) < 0)
        return IUS_ERR_VALUE;
    if((IUS_ERR_MIN_ARG_NULL_VALUE = H5Ecreate_msg(state->iusErrorClass, H5E_MINOR, IUS_ERR_MIN_ARG_NULL_VALUE_MSG)) < 0)
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
    return H5Eget_num(state->iusErrorStack);
}

int iusErrorPrint
(
    FILE *pFILE
)
{
    iue_t state = iusErrorGetState();
    herr_t status = 0;
    if(state->enable == IUS_TRUE)
    {
        status = H5Eprint2(state->iusErrorStack, pFILE);
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

int iusErrorAutoReport
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
