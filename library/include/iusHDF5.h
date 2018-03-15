//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusHDF5.h
//                  helper functions to read (header) data in HDF format
//  iusVersion    : 255.255.255.255
//
//=============================================================================
#ifndef IUSHDF5_H
#define IUSHDF5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdf5.h>
#include "iusTypes.h"

/** 
 * \brief Reads one or an array of floats from file stored in the variable
 * pVariableString
 *
 * In-place:   n.a.
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the float(s)
 * \param[in]   handle          : the file or group handle containing the
 *                                variable to be read
 * \param[in]   pVariableString : the name of the variable to be read
 * \param[out]  pValue          : the float value or array read from file
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTread_dataset_int()
 */
herr_t iusHdf5ReadFloat
(
    hid_t        handle,
    const char * pVariableString,
    float *      pValue,
    int          verbose
);

/** 
 * \brief Reads one or an array of ints from file stored in the variable
 * pVariableString
 *
 * In-place:   n.a.
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the int(s)
 * \param[in]   handle          : the file or group handle containing the
 *                                variable to be read
 * \param[in]   pVariableString : the name of the variable to be read
 * \param[out]  pValue          : the int value or array that is read from file
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTread_dataset_float()
 */
herr_t iusHdf5ReadInt
(
    hid_t        handle,
    const char * pVariableString,
    int *        pValue,
    int          verbose
);

/** 
 * \brief Reads one or an array of longs from file stored in the variable
 * pVariableString
 *
 * In-place:   n.a.
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the long int(s)
 * \param[in]   handle          : the file or group handle containing the
 *                                variable to be read
 * \param[in]   pVariableString : the name of the variable to be read
 * \param[out]  pValue          : the long int value or array that is read from
 *                                file
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTread_dataset_long()
 */
herr_t iusHdf5ReadLong
(
    hid_t        handle,
    const char * pVariableString,
    long int *   pValue,
    int          verbose
);

/** 
 * \brief Reads a string from file stored in the variable pVariableString
 *
 * In-place:   n.a.
 * \pre         The Hdf5 file is opened, no memory is allocated yet for the
 *              return string
 * \post        *ppReturnString contains the string (char *). Its memory is
 *              created and needs to be freed separately
 * \param[in]   handle          : the file or group handle containing the
 *                                variable to be read
 * \param[in]   pVariableString : the name of the variable to be read
 * \param[out]  ppReturnString  : the string (memory is allocated for it)
 * \param[in]   verbose         : boolean, print file information to stdout
 * \return      the result of the reading
 */
herr_t iusHdf5ReadString
(
    hid_t        handle,
    const char * pVariableString,
    char * *     ppReturnString,
    int verbose
);

/** 
 * \brief Reads the grid from file that is defined by pGridName. 
 *
 * The grid can be a Polar or Cartesian grid. The grid type is identified by the
 * variable name containing either 'Cartesian' or 'Polar' 
 *
 * In-place:   n.a.
 * \pre         The Hdf5 file is opened, no memory is allocated yet for the grid
 *              arrays, but the grid struct is allocated
 * \post        *pGrid contains the grid data. Its memory is created and needs
 *              to be freed separately
 * \param[in]   handle    : the file or group handle containing the dataset to be
 *                         read
 * \param[in]   pGridName : the name of the variable to be read
 * \param[out]  pGrid     : the grid structure that is filled (memory is
 *                         allocated for its contents)
 * \param[out]  verbose   : print verbose output
 * \return     0 if okay, the error status otherwise
 */
herr_t iusHdf5ReadGrid
(
    hid_t              handle,
    const char * const pGridName,
    IusGrid *          pGrid,
    int                verbose
);

/**
 * \brief Writes one or an array of floats to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         array with values was allocated and initialized
 * \post        values have been written to Hdf5 handle
 * \param[in]   handle          : the file or group handle for writing
 * \param[in]   pVariableString : the name of the variable to be written
 * \param[out]  pValue          : the float value or array that is to be written
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTmake_dataset_float()
 */
herr_t iusHdf5WriteFloat
(
    hid_t        handle,
    const char * pVariableString,
    float *      pValues,
    int          numValues,
    int          verbose
);
//return H5LTmake_dataset_float( handle,  "centerFrequency",  1, dims,
//    &(pInst->centerFrequency));

/**
 * \brief Writes one or an array of int values to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         array with values was allocated and initialized
 * \post        values have been written to Hdf5 handle
 * \param[in]   handle          : the file or group handle for writing
 * \param[in]   pVariableString : the name of the variable to be written
 * \param[out]  pValue          : the int value or array that is to be written
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTmake_dataset_int()
 */
herr_t iusHdf5WriteInt
(
    hid_t        handle,
    const char * pVariableString,
    int *        pValues,
    int          numValues,
    int          verbose
);

/**
 * \brief Writes one or an array of long values to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         array with values was allocated and initialized
 * \post        values have been written to Hdf5 handle
 * \param[in]   handle          : the file or group handle for writing
 * \param[in]   pVariableString : the name of the variable to be written
 * \param[out]  pValue          : the long value or array that is to be written
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTmake_dataset_long()
 */
herr_t iusHdf5WriteLong
(
    hid_t        handle,
    const char * pVariableString,
    long *       pValues,
    int          numValues,
    int          verbose
);

/**
 * \brief Writes a string to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         string was created and initialized
 * \post        string has been written to Hdf5 handle
 * \param[in]   handle          : the file or group handle for writing
 * \param[in]   pVariableString : the name of the string to be written
 * \param[out]  pValue          : the actual string itself that is to be written
 * \param[in]   verbose         : (0/1) print file information to stdout
 * \return      the result of H5LTmake_dataset_string()
 */
herr_t iusHdf5WriteString
(
    hid_t        handle,
    const char * pVariableString,
    char *       pString,
    int          numValues,
    int          verbose
);

/** 
 * \brief Writes a grid to an Hdf5 handle
 *
 * The grid can be a Polar or Cartesian grid. The grid type is identified by the
 * variable name containing either 'Cartesian' or 'Polar' 
 *
 * In-place:   n.a.
 * \pre         The Hdf5 file is opened, no memory is allocated yet for the grid
 *              arrays, but the grid struct is allocated
 * \post        *pGrid contains the grid data. Its memory is created and needs
 *              to be freed separately
 * \param[in]   handle          : the file or group handle containing the dataset to be
 *                         written
 * \param[in]   pVariableString : "PolarGrid" or "CartesianGrid"
 * \param[out]  pGrid           : the grid structure that is filled
 * \param[out]  verbose         : print verbose output
 * \return
 */
herr_t iusHdf5WriteGrid
(
    hid_t        handle,
    const char * pVariableString,
    IusGrid *    pGrid,
    int          verbose
);

herr_t iusHdf5DisableMessages();


#ifdef __cplusplus
}
#endif

#endif


