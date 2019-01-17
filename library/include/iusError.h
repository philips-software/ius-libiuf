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


/**
 * \brief Prevent unused parameter warning
 */
#define IUS_UNUSED(x) (void)(x)

/**
 * \brief List of error codes
 */
#define IUS_ERR_GENERAL      (100001)   /**< general error */
#define IUS_ERR_MEMORY       (100002)    /**< memory error  */
#define IUS_ERR_VALUE        (100003)    /**< value error  */

/**
 * \brief IUS assertion, general
 */
#define IUS_ASSERT( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        fprintf( stderr, "General error occuring on line %d in file %s\n", __LINE__, __FILE__ ); \
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
        fprintf( stderr, "Memory error occuring on line %d in file %s\n", __LINE__, __FILE__ ); \
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
        fprintf( stderr, "Value error occuring on line %d in file %s\n", __LINE__, __FILE__ ); \
        exit( IUS_ERR_VALUE ); \
    } \
}

#ifdef __cplusplus
}
#endif

int iusErrorGetCount
(
    void
);

int iusErrorPrint
(
    FILE *pFILE
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

extern hid_t  IUS_ERR_MAJ_GENERAL;  //     (100001)   /**< general error */
extern hid_t  IUS_ERR_MAJ_MEMORY;   //    (100002)    /**< memory error  */
extern hid_t  IUS_ERR_MAJ_VALUE;    //    (100003)    /**< value error  */
extern hid_t  IUS_ERR_MAJ_ERROR;
extern hid_t  IUS_ERR_MAJ_HDF5;

extern hid_t  IUS_ERR_MIN_ARG_VALUE;
extern hid_t  IUS_ERR_MIN_ARG_FILENAME;  //     (100001)   /**< general error */
extern hid_t  IUS_ERR_MIN_ALLOC;
extern hid_t  IUS_ERR_MIN_FORMAT;
extern hid_t  IUS_ERR_MIN_ARG_NULL_VALUE;
extern hid_t  IUS_ERR_MIN_ARG_DUPLICATE_KEY;
extern hid_t  IUS_ERR_MIN_ARG_INVALID_KEY;
extern hid_t  IUS_ERR_MIN_HDF5;

#define IUS_ERROR_PUSH(maj,min,msg)     iusErrorPush(__FILE__, __func__, __LINE__, maj, min, msg)
#define IUS_ERROR_FMT_PUSH(maj,min,fmt,...) iusErrorFormatAndPush(__FILE__, __func__, __LINE__, maj, min, fmt, __VA_ARGS__)

#endif  // _IUSERROR_H
