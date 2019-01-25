//==============================================================================
//    COPYRIGHT 2017 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusErrorState.h
//                  IUS error codes
//  iusVersion    : 255.255.255.255
//
//==============================================================================
/**
* \file iusError.h
* \brief IUS error codes
*
* \author Harm Belt
*/

#ifndef _IUSERROR_H
#define _IUSERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ius.h>
#include <stdio.h>   // fprintf()
#include <stdlib.h>  // exit()


#ifdef __cplusplus
}
#endif

int iusErrorGetCount
(
    void
);

int iusErrorPrint
(
);

int iusErrorPush
(
    const char *pFileName,
    const char *pFunctionName,
    int lineNumber,
    hid_t maj,
    hid_t min,
    char *msg
);

int iusErrorFormatAndPush
(
    const char *pFileName,
    const char *pFunctionName,
    int lineNumber,
    hid_t maj,
    hid_t min,
    char *msgFormat,
    ...
);

int iusHDF5ErrorLog
(
    IUS_BOOL enable
);

int iusErrorLog
(
    IUS_BOOL enable
);

int iusErrorLogClear
(
    void
);

int iusErrorSetStream
(
    FILE *stream
);

int iusErrorAutoReport
(
    IUS_BOOL enable
);

extern hid_t  IUS_ERR_MAJ_GENERAL;
extern hid_t  IUS_ERR_MAJ_MEMORY;
extern hid_t  IUS_ERR_MAJ_VALUE;
extern hid_t  IUS_ERR_MAJ_ERROR;
extern hid_t  IUS_ERR_MAJ_HDF5;

extern hid_t  IUS_ERR_MIN_ARG_VALUE;
extern hid_t  IUS_ERR_MIN_ARG_FILENAME;
extern hid_t  IUS_ERR_MIN_ALLOC;
extern hid_t  IUS_ERR_MIN_MEMCOPY;
extern hid_t  IUS_ERR_MIN_FORMAT;
extern hid_t  IUS_ERR_MIN_ARG_NULL_VALUE;
extern hid_t  IUS_ERR_MIN_ARG_DUPLICATE_KEY;
extern hid_t  IUS_ERR_MIN_ARG_INVALID_KEY;
extern hid_t  IUS_ERR_MIN_HDF5;
extern hid_t  IUS_ERR_MIN_ASSERT_FAILED;


#define IUS_ERROR_PUSH(maj,min,msg)     iusErrorPush(__FILE__, __func__, __LINE__, maj, min, msg)
#define IUS_ERROR_PRINT(maj,min,msg)     iusErrorPush(__FILE__, __func__, __LINE__, maj, min, msg);\
                                            iusErrorPrint();

#define IUS_ERROR_FMT_PUSH(maj,min,fmt,...) iusErrorFormatAndPush(__FILE__, __func__, __LINE__, maj, min, fmt, __VA_ARGS__)
#define IUS_ERROR_FMT_PRINT(maj,min,fmt,...) iusErrorFormatAndPush(__FILE__, __func__, __LINE__, maj, min, fmt, __VA_ARGS__);\
                                            iusErrorPrint();


// Error handling convenience macro's
#define IUS_ERR_ALLOC_NULL_N_RETURN(var, adt,  retval) if ((var) == NULL) {                                 \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for " #adt);                   \
        return retval;                                                                                      \
    }


#define IUS_ERR_CHECK_NULL_N_RETURN(var, retval) if ((var) == NULL) {                                       \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument "  #var  " was NULL");      \
        return retval;                                                                                      \
    }

#define IUS_ERR_CHECK_EMPTYSTR_N_RETURN(var, return_value) if (strcmp((var),"") == 0) {                       \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument " #var  " was empty");           \
        return return_value;                                                                                \
    }

#define IUS_ERR_STRP_NULL_EMPTY(var, retval) IUS_ERR_CHECK_NULL_N_RETURN(var, retval);                      \
        IUS_ERR_CHECK_EMPTYSTR_N_RETURN(var, retval)

#define IUS_ERR_EVAL_N_RETURN(expr, retval) if (expr) {                                                     \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, #expr "");                                 \
        return retval;                                                                                      \
    }


/**
 * \brief List of error codes
 */
#define IUS_ERR_GENERAL      (100001)   /**< general error */
#define IUS_ERR_MEMORY       (100002)    /**< memory error  */
#define IUS_ERR_VALUE        (100003)    /**< value error  */

/**
 * \brief Prevent unused parameter warning
 */
#define IUS_UNUSED(x) (void)(x)

/**
 * \brief IUS assertion, general
 */
#define IUS_ASSERT( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_GENERAL,IUS_ERR_MIN_ASSERT_FAILED, "General error occurred, assertion failed for " #IN_CONDITION); \
        exit( IUS_ERR_GENERAL ); \
    } \
}

/**
 * \brief IUS assertion, memory
 */
#define IUS_ASSERT_MEMORY( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY,IUS_ERR_MIN_ASSERT_FAILED, "Memory error occurred, assertion failed for " #IN_CONDITION); \
        exit( IUS_ERR_MEMORY ); \
    } \
}

/**
 * \brief IUS assertion, value
 */
#define IUS_ASSERT_VALUE( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE,IUS_ERR_MIN_ASSERT_FAILED, "Value error occurred, assertion failed for " #IN_CONDITION); \
        exit( IUS_ERR_VALUE ); \
    } \
}


#endif  // _IUSERROR_H
