//==============================================================================
//    COPYRIGHT 2017 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusError.h
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

#include <stdio.h>   // fprintf()
#include <stdlib.h>  // exit()


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

#endif  // _IUSERROR_H
