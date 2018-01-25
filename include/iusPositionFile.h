//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusPositionFile.h
//                  Position file (in hdf5 format)
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#ifndef _IUSPOSITIONFILE_H
#define _IUSPOSITIONFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "base/iusTypes.h"
#include <hdf5.h>

/** 
 * \brief  file data structure
 * The file has these top level variables:
 */
typedef struct 
{
    int IUSVersion;                       /**< Cersion of cwx file format */
    char *ID;                             /**< Unique file identifier */

	IUSPositionInstance pInst;            /**< The instance containing the positions data
    
    /*
     * state variables
     */
    hid_t handle;                         /**< file handle */
    hid_t positionData;                   /**< handle to /positionData  */
    hid_t confusionData;                  /**< handle to /confusionData */
    hid_t timeStampData;                  /**< handle to /timeStampData */
    int currentPosition;                  /**< current index in the arrays */
    const int verbose;                    /**< The verbosity level */
} IUSPositionFileInstance;

/**
 * \brief Creates a position file, sets the type and number of positions, but does not add the data, youe write for that
 *
 * In-place: n.a.
 * \param[in]   pFullFileName: the name of the file that will be created, typically has a .position extension
 * \param[in]   pInst: The position instance
 * \param[in]   verbose: print debugging or not
 * \return      PositionFile instance pointer (or NULL in case of failure)
 */
IUSPositionFileInstance *iusPositionFileCreate
(
   const char * pFullFileName,
   IUSPositionInstance *pInst,
   const int verbose
);

/**
 * \brief Opens a File instance from an existing file, allocates
 * internal memory and initializes internal data, except the bmode data itself
 * 
 * In-place:   n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFilepath: filename of hdf5 .
 * \param[in]   verbose: print file information to stdout.
 * \return      File instance pointer (or NULL in case of failure)
 * */
IUSPositionFileInstance *iusPositionFileOpen
(
   const char * const pFilepath,
   const int verbose
);

/**
 *  \brief Read all position data
 * In-place:    n/a
 * \pre         The file has been successfully opened
 * \post        Data was read, internal states have been updated.
 * \param[in]   pFileInst   : The file instance
 * \return      int     : 0 on success
 * */
int iusPositionFileRead
(
   IUSPositionFileInstance *pFileInst,
   int verbose
);

/**
 *  \brief write all positions
 * In-place:    n/a
 * \pre         The file has been successfully opened/created
 * \post        Data has been written to file, internal states have been updated.
 * \param[in]   pFileInst   : The file instance
 * \return      int     : 0 on success
 * */
int iusPositionFileWrite
(
   IUSPositionFileInstance *pFileInst,
   int verbose
);

/** 
 * \brief Frees all File resources and close file.
 *
 * In-place:   n.a.
 * \pre        The instance object was created.
 * \post       The contents of File is freed.
 * \param[in]  pInst:  pointer to File instance
 * \return     0 on success
 */
int iusPositionFileClose
(
    IUSPositionFileInstance *pFileInst
);

#ifdef __cplusplus
}
#endif

#endif  // _IUSPOSITIONFILE_H
