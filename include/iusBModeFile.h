//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusBModeFile.h
//                  Bmode file (in hdf5 format)
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#ifndef _IUSBMODEFILE_H
#define _IUSBMODEFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../base/iusTypes.h"
// #include "fileio/iusCWXFile.h" /* V3 should no longer directly depend on this parent structure */
#include <hdf5.h>

/** 
 * \brief Bmode file data structure contains the propoerties of a BMode ultrasound image
 */
typedef struct 
{
	
	iusBModeInstance *pIUSBModeInstance   /**< The Bmode instance type */
    int startFrame;                       /**< The frame that the parent started processing */
    int length;                           /**< The number of input frames that have been processed */
    
    /*
     * state variables
     */
    hid_t fileChunkConfig;                /**< chunk handle */
    hid_t handle;                         /**< file handle */
    hid_t IData;                          /**< dataset handle to I component */
    hid_t QData;                          /**< dataset handle to Q component */
    int currentFrame;                     /**< current frame number */

} IUSBModeFileInstance;


/** 
 * \brief Creates a BMode File instance from the specified parameters, allocates
 * internal memory and initializes internal header data.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFilePath: location of the output file
 * \param[in]   pCWXFileInst: The file instance of the CWX File that this file is based on
 * \param[in]   startFrame: the frame number in the input file that processing was started 
 * \param[in]   length: the number of frames to process
 * \param[in]   pOutputGrid: the output grid. Note: currently the origin is assumed top,center and sample positions must be uniform
 * \param[in]   interpolationMethod The interpolation method that is used during format conversion (e.g. linear, cubic), with or without prefiltering 
 * \param[in]   verbose Generate verbose output
 * \return      File instance pointer (or NULL in case of an error)
 */
IUSBModeFileInstance *iusBModeFileCreateFrom CWX(const char *pFilePath,
        IUSBModeInstance,
        int startFrame, 
		int length,
        IUSGrid *pOutputGrid,
        int interpolationMethod, 
        int verbose);

/** 
 * \brief Opens a File instance from an existing file, allocates
 * internal memory and initializes internal data, except the bmode data itself
 *
 * In-place:    n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFilepath: filename of hdf5 .
 * \param[in]   verbose: boolean, print file information to stdout.
 * \return      File instance pointer (or NULL)
 */
IUSBModeFileInstance *iusBModeFileOpen
(
    const char * const pFilepath,
    const int verbose
);

/** 
 * \brief Reads all frames of a BMode File
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data has been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst   : File instance
 * \param[out]  pppFrame: the BMode (Intensity component) frames width x height x numFrames
 * \return      int     : 0 on success
 */
int iusBModeFileRead
(    
    IUSBModeFileInstance *pInst,
    float * * *pppFrame
);

/**
 * \brief Reads frame of bmode samples 
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst : File instance for InputFile
 * \param[out]  ppFrame : one BMode frame: width x height 
 * \param[in]   frameNum: the frame number to read.
 * \return      int   : 0 on success
  */
int iusBModeReadFrame
(    
    IUSBModeFileInstance *pInst,
    float * *ppFrame,
    int frameNum
);

/**
 * \brief Reads next frame of bmode samples 
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst : File instance for InputFile
 * \param[out]  ppFrame : the current BMode frame: width x height 
 * \return      int   : 0 on success
  */
int iusBModeReadNextFrame
(    
    IUSBModeFileInstance *pInst,
    float * *ppFrame
);

/**
* \brief writes a specific bmode frame 
*
* In-place:   n/a
* \pre         The instance object was created. Output data lines have been allocated.
* \post        Frame was written, pInst->currentFrame is set to next frame
* \param[in]   pInst : File instance
* \param[out]  ppFrame : the current BMode frame: width x height 
* \param[in]   frameNum: The frame number to write 
* \return      int   : 0 on success
*/
int iusBModeWriteFrame
(
    IUSBModeFileInstance * pInst,
    float * *ppFrame, 
    int frameNum
);

/**
* \brief writes next (or first) frame of bmode samples
*
* In-place:   n/a
* \pre         The instance object was created. Output data lines have been allocated.
* \post        Data was read, pInst->currentFrame is set to next frame
* \param[in]   pInst : File instance
* \param[out]  ppFrame : the current BMode frame: width x height 
* \return      int   : 0 on success
*/
int iusBModeWriteNextFrame
(
    IUSBModeFileInstance * pInst,
    float * *ppFrame 
);


/** 
 * \brief Frees all File resources and closes file.
 *
 * In-place:   n.a.
 * \pre        The instance object was created.
 * \post       The contents of File is freed.
 * \param[in]  pInst:  pointer to File instance
 * \return     0 on success
 */
int iusBModeFileClose
(
    IUSBModeFileInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  // _IUSBMODEFILE_H
