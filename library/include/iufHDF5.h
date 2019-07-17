#ifndef IUFHDF5_H
#define IUFHDF5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdf5.h>
#include "iufTypes.h"

/** 
 * \brief Reads one or an array of floats from file stored in the variable
 * pVariableString
 *
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the float(s)
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5ReadFloat
(
    hid_t        handle,              ///< the file or group handle containing the variable to be read
    const char * pVariableString,     ///< the name of the variable to be read
    float *      pValue               ///< [out] the float value or array read from file
);

/** 
 * \brief Reads one or an array of shorts from file stored in the variable
 * pVariableString
 *
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the int(s)
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5ReadShort
(
    hid_t        handle,            ///< the file or group handle containing the
    const char * pVariableString,   ///< the name of the variable to be read variable to be read
    short *      pValue             ///< [out] the short value or array that is read from file
);

/** 
 * \brief Reads one or an array of ints from file stored in the variable
 * pVariableString
 *
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the int(s)
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5ReadInt
(
    hid_t        handle,           ///< the file or group handle containing the variable to be read
    const char * pVariableString,  ///< the name of the variable to be read
    int *        pValue            ///< [out] the int value or array that is read from file
);


/** 
 * \brief Reads one or an array of longs from file stored in the variable
 * pVariableString
 *
 * \pre         Memory is allocated for pValue, the Hdf5 file has been opened
 * \post        pValue contains the long int(s)
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5ReadLong
(
    hid_t        handle,             ///< the file or group handle containing the variable to be read
    const char * pVariableString,    ///< the name of the variable to be read
    long int *   pValue              ///< [out] the long int value or array that is read from file
);

/** 
 * \brief Reads a string from file stored in the variable pVariableString
 *
 * \pre         The Hdf5 file is opened, no memory is allocated yet for the
 *              return string
 * \post        *ppReturnString contains the string (char *). Its memory is
 *              created and needs to be freed separately
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5ReadString
(
    hid_t        handle,            ///< the file or group handle containing the variable to be read
    const char * pVariableString,   ///< the name of the variable to be read
    char * ppReturnString           ///< [out] the string (memory is allocated for it)
);

/** 
 * \brief Reads the grid from file that is defined by pGridName. 
 *
 * The grid can be a Polar or Cartesian grid. The grid type is identified by the
 * variable name containing either 'Cartesian' or 'Polar' 
 *
 * \pre         The Hdf5 file is opened, no memory is allocated yet for the grid
 *              arrays, but the grid struct is allocated
 * \post        *pGrid contains the grid data. Its memory is created and needs
 *              to be freed separately
 * \return     the HDF5 error status of the reading
 */
herr_t iufHdf5ReadGrid
(
    hid_t              handle,     ///< the file or group handle containing the dataset to be
    const char *       pGridName,  ///< the name of the variable to be read
    IufGrid *          pGrid       ///< [out] the grid structure that is filled (memory is allocated for its contents)
);

/**
 * \brief Writes one or an array of floats to an Hdf5 handle
 *
 * \pre         array with values was allocated and initialized
 * \post        values have been written to Hdf5 handle
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5WriteFloat
(
    hid_t        handle,            ///< the file or group handle for writing
    const char * pVariableString,   ///< the name of the variable to be written
    float *      pValues,           ///< the float value or array that is to be written
    int          numValues          ///< The length of the pValues array
);

/**
 * \brief Writes one or an array of int values to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         array with values was allocated and initialized
 * \post        values have been written to Hdf5 handle
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5WriteInt
(
    hid_t        handle,            ///< handle          : the file or group handle for writing
    const char * pVariableString,   ///< the name of the variable to be written
    int *        pValues,           ///< the int value or array that is to be written
    int          numValues          ///< The length of the pValues array
);

/**
 * \brief Writes one or an array of long values to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         array with values was allocated and initialized
 * \post        values have been written to Hdf5 handle
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5WriteLong
(
    hid_t        handle,            ///< the file or group handle for writing
    const char * pVariableString,   ///< the name of the variable to be written
    long *       pValues,           ///< the long value or array that is to be written
    int          numValues          ///< the length of the pValues array
);

/**
 * \brief Writes a string to an Hdf5 handle
 *
 * In-place:   n.a.
 * \pre         string was created and initialized
 * \post        string has been written to Hdf5 handle
 * \return      the HDF5 error status of the reading
 */
herr_t iufHdf5WriteString
(
    hid_t        handle,           ///< the file or group handle for writing
    const char * pVariableString,  ///< the name of the string to be written
    const char * pString           ///< the actual string itself that is to be written
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
 * \return the HDF5 error status of the reading
 */
herr_t iufHdf5WriteGrid
(
    hid_t        handle,           ///< the file or group handle containing the dataset to be written
    const char  *pGridName,  ///< "PolarGrid" or "CartesianGrid"
    IufGrid *    pGrid             ///< the grid structure that will be serialized
);

#define IUF_MAX_HDF5_PATH	1024

#ifdef __cplusplus
}
#endif

#endif

