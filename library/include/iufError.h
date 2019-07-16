/**
* \file iufError.h
* \brief IUF error codes
* \author Harm Belt
*/

#ifndef _IUFERROR_H
#define _IUFERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <iuf.h>
#include <stdio.h>   // fprintf()
#include <stdlib.h>  // exit()


#ifdef __cplusplus
}
#endif

/** \brief Get the number of errors that occurred so far
* \return Returns the number of errors that occurred so far
*/
int iufErrorGetCount
(
    void
);

/** \brief Prints the Error stack
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorPrint
(
);

/** \brief Create a string representation of the current Error stack
* \return Returns a string representation of the current Error stack.
*/
char * iufErrorString
(
);

/** \brief Pushes an Error context onto the IUF error stack.
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorPush
(
    const char *pFileName,          ///< The source file of the error
    const char *pFunctionName,      ///< The function that caused the error
    int lineNumber,                 ///< The line number in the source file  of the error
    hid_t maj,                      ///< The major error class
    hid_t min,                      ///< The minor error class
    char *msg                       ///< The error message
);


/** \brief Format and Push an Error context onto the IUF error stack.
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorFormatAndPush
(
    const char *pFileName,          ///< The source file of the error
    const char *pFunctionName,      ///< The function that caused the error
    int lineNumber,                 ///< The line number in the source file  of the error
    hid_t maj,                      ///< The major error class
    hid_t min,                      ///< The minor error class
    char *msgFormat,                ///< The error message format (like sprintf)
    ...                             ///< varargs matching the msgFormat argument
);


/** \brief Enable or disable HDF5 library error logging
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufHDF5ErrorLog
(
    IUF_BOOL enable                 ///< IUF_TRUE enables / IUF_FALSE disables error logging
);


/** \brief Enable or disable IUF library error logging
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorLog
(
    IUF_BOOL enable                 ///< IUF_TRUE enables / IUF_FALSE disables error logging
);


/** \brief Removes all messages from the error stack
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorLogClear
(
    void
);


/** \brief Error messages wil be printed to this FILE stream (i.e. stdout would sent the errors to console output).
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorSetStream
(
    FILE *stream                    ///< Valid FILE stream like from fopen or constants like stderr/stdout
);

/** \brief Error messages wil be printed to the file with name pFileName.
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorSetStreamToFile
(
    const char * pFileName
);

/** \brief Closes the filestream opened with iufErrorSetStreamToFile() and resets the stream to stderr.
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorCloseFileStream
(
);

/** \brief Error messages wil printed when encountered by the library calls.
* \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
*/
int iufErrorAutoReportSet
(
    IUF_BOOL enable                 ///< IUF_TRUE enables / IUF_FALSE disables automatic error logging
);

/** \brief Error messages wil printed when encountered by the library calls.
* \return Returns the value #IUF_TRUE when auto reporting is enabled, #IUF_FALSE when disabled.
*/
IUF_BOOL iufErrorAutoReportGet
(
    void
);


extern hid_t  IUF_ERR_MAJ_GENERAL;  ///< Major error handling class for general errors
extern hid_t  IUF_ERR_MAJ_MEMORY;   ///< Major error handling class for memory errors
extern hid_t  IUF_ERR_MAJ_VALUE;    ///< Major error handling class for value related errors
extern hid_t  IUF_ERR_MAJ_ERROR;    ///< Major error handling class for errors
extern hid_t  IUF_ERR_MAJ_HDF5;     ///< Major error handling class for hdf5 io errors

extern hid_t  IUF_ERR_MIN_ARG_VALUE;            ///< Minor error handling class for argument value errors
extern hid_t  IUF_ERR_MIN_ARG_FILENAME;         ///< Minor error handling class for filename errors
extern hid_t  IUF_ERR_MIN_ALLOC;                ///< Minor error handling class for memory allocation value errors
extern hid_t  IUF_ERR_MIN_MEMCOPY;              ///< Minor error handling class for memory errors
extern hid_t  IUF_ERR_MIN_FORMAT;               ///< Minor error handling class for formatting errors
extern hid_t  IUF_ERR_MIN_ARG_NULL_VALUE;       ///< Minor error handling class for NULL values
extern hid_t  IUF_ERR_MIN_ARG_DUPLICATE_KEY;    ///< Minor error handling class for duplicate key errors in a dictionary
extern hid_t  IUF_ERR_MIN_ARG_INVALID_KEY;      ///< Minor error handling class for invalid key errors in a dictionary
extern hid_t  IUF_ERR_MIN_HDF5;                 ///< Minor error handling class for invalid hdf5 file io errors
extern hid_t  IUF_ERR_MIN_ASSERT_FAILED;        ///< Minor error handling class for assertion errors



/** \brief Convenience macro, that pushes the error from current file function and line number.
*/
#define IUF_ERROR_PUSH(maj,min,msg)     iufErrorPush(__FILE__, __func__, __LINE__, maj, min, msg)

/** \brief Convenience macro, that pushes the error from current file function and line number and prints it.
*/
#define IUF_ERROR_PRINT(maj,min,msg)     iufErrorPush(__FILE__, __func__, __LINE__, maj, min, msg);\
                                            iufErrorPrint();

/** \brief Convenience macro, that formats and pushes the error from current file function and line number.
*/
#define IUF_ERROR_FMT_PUSH(maj,min,fmt,...) iufErrorFormatAndPush(__FILE__, __func__, __LINE__, maj, min, fmt, __VA_ARGS__)



/** \brief Convenience macro, that formats,  pushes the error from current file function and line number and prints it.
*/
#define IUF_ERROR_FMT_PRINT(maj,min,fmt,...) iufErrorFormatAndPush(__FILE__, __func__, __LINE__, maj, min, fmt, __VA_ARGS__);\
                                            iufErrorPrint();


// Error handling convenience macro's

/** \brief Convenience macro, that pushes IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC for var that is NULL and returns retval
* \return Returns retval.
*/
#define IUF_ERR_ALLOC_NULL_N_RETURN(var, adt,  retval) if ((var) == NULL) {                                 \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for " #adt);                   \
        return retval;                                                                                      \
    }


/** \brief Convenience macro, that pushes IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE for var that is NULL and returns retval
* \return Returns retval.
*/
#define IUF_ERR_CHECK_NULL_N_RETURN(var, retval) if ((var) == NULL) {                                       \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "argument "  #var  " was NULL");      \
        return retval;                                                                                      \
    }

/** \brief Convenience macro, that pushes IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE for empty string var and returns retval
* \return Returns retval.
*/
#define IUF_ERR_CHECK_EMPTYSTR_N_RETURN(var, return_value) if (strcmp((var),"") == 0) {                       \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "argument " #var  " was empty");           \
        return return_value;                                                                                \
    }


/** \brief Convenience macro, that sequentially calls #IUF_ERR_CHECK_NULL_N_RETURN and #IUF_ERR_CHECK_EMPTYSTR_N_RETURN
* \return Returns retval.
*/
#define IUF_ERR_STRP_NULL_EMPTY(var, retval) IUF_ERR_CHECK_NULL_N_RETURN(var, retval);                      \
        IUF_ERR_CHECK_EMPTYSTR_N_RETURN(var, retval)


/** \brief Convenience macro, that evaluates expression expr and pushes IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE when true and returns retval
* \return Returns retval.
*/
#define IUF_ERR_EVAL_N_RETURN(expr, retval) if (expr) {                                                     \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, #expr "");                                 \
        return retval;                                                                                      \
    }


/**
 * \brief List of error codes
 */
#define IUF_ERR_GENERAL      (100001)   /**< general error */
#define IUF_ERR_MEMORY       (100002)    /**< memory error  */
#define IUF_ERR_VALUE        (100003)    /**< value error  */

/**
 * \brief Prevent unused parameter warning
 */
#define IUF_UNUSED(x) (void)(x)

/**
 * \brief IUF assertion, general
 */
#define IUF_ASSERT( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_GENERAL,IUF_ERR_MIN_ASSERT_FAILED, "General error occurred, assertion failed for " #IN_CONDITION); \
        exit( IUF_ERR_GENERAL ); \
    } \
}

/**
 * \brief IUF assertion, memory
 */
#define IUF_ASSERT_MEMORY( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY,IUF_ERR_MIN_ASSERT_FAILED, "Memory error occurred, assertion failed for " #IN_CONDITION); \
        exit( IUF_ERR_MEMORY ); \
    } \
}

/**
 * \brief IUF assertion, value
 */
#define IUF_ASSERT_VALUE( IN_CONDITION ) \
{ \
    if ( !(IN_CONDITION) )\
    { \
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE,IUF_ERR_MIN_ASSERT_FAILED, "Value error occurred, assertion failed for " #IN_CONDITION); \
        exit( IUF_ERR_VALUE ); \
    } \
}


#endif  // _IUFERROR_H
