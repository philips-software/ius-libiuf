//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusInputFile.h
//                  Input file (in hdf5 format (aka v7.3 .mat file)).
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#ifndef _IUSINPUTFILE_H
#define _IUSINPUTFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdf5.h>
#include <iusNode.h>
#include <iusInput.h>

#define IUS_INPUT_TYPE "IUSINPUTFILETYPE_V3"

/** 
 * \brief Input file data structure
 * The file has these top level variables:
 */
typedef struct 
{
    IusInputInstance   *pIusInput;         /**< The data object */
    /*
     * state variables
     */
    hid_t fileChunkConfig;                /**< file chunck handle */
    hid_t handle;                         /**< file handle */
    hid_t rfDataset;                      /**< dataset handle */
    int currentFrame;                     /**< current frame number */
    int currentPulse;                     /**< current pulse number */

} IusInputFileInstance;

int iusInputFileSave
(
	IusInputFileInstance *pIFI,
	int verbose
);

/**
 * \brief Creates a File instance from the specified parameters, allocates
 * internal memory and initializes internal header data.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFullFileName   : full name of hdf5 output file (path + name + ext).
 * \param[in]   pInst: File instance pointer (or NULL)
 * \param[in]   version: the ius compatibility vesion number (or NULL)
 */
IusInputFileInstance *iusInputFileCreate
(
    const char *pFullFileName
);

/** 
 * \brief Opens a File instance from an existing file, allocates
 * internal memory and initializes internal data, except the rfdata itself
 *
 * In-place:   n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFullFileName: full name of hdf5 input file (path + name + ext).
 * \param[in]   verbose  : boolean, print file information to stdout.
 * \return      File instance pointer (or NULL)
 */
IusInputFileInstance * iusInputFileOpen
(
    const char * const pFullFileName,
    const int verbose
);

/** 
 * \brief Reads existing input data (RF) from a File
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data has been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst   : File instance
 * \param[out]  pRFout  : RF data of size numSamplesPerLine x nrlements x numTransmitPulses X numFrames
 * \return      int     : 0 on success
 */
int iusInputFileRead
(    
    IusInputFileInstance * pInst,
    float *const *const *const *const pRFout   /* 4 starts :-)  please use filetypes :-)*/
);

/**
 * \brief Reads one frame of input samples 
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal state currentFrame has been updated
 * \param[in]   pInst : File instance
 * \param[out]  pRFout: RF data of size numSamplesPerLine x nrlements x numTransmitPulses
 * \param[in]   frameNum: Frame number to read
 * \return      int   : 0 on success
  */
int iusInputFileReadFrame
(    
    IusInputFileInstance * pInst,
    float *const *const *const pRFout,
    int frameNum
);

/**
 * \brief Reads next (or first) frame of input samples 
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal state currentFrame has been updated
 * \param[in]   pInst : File instance
 * \param[out]  pRFout: RF data of size numSamplesPerLine x nrlements x numTransmitPulses
 * \return      int   : 0 on success
  */
int iusInputFileReadNextFrame
(    
    IusInputFileInstance * pInst,
    float *const *const *const pRFout
);

/**
* \brief Reads next (or first) frame of input samples
*
* In-place:   n/a
* \pre         The instance object was created. Output data lines have been allocated.
* \post        Data was read, internal states have been updated.
* \param[in]   pInst : File instance
* \param[out]  pFrame: RF data of size numSamplesPerLine x nrlements x numTransmitPulses
* \return      int   : 0 on success
*/
int iusInputFileWriteNextPulse
(
	IusInputFileInstance * pInst,
	const float *pFrame
);

/**
 * \brief Reads samples of next (or first) event of current frame 
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst   : File instance
 * \param[out]  pRFout  : RF data of size numSamplesPerLine x nrlements 
 * \return      int     : 0 on success
*/
int iusInputFileReadNextPulse
(    
    IusInputFileInstance * pInst,
    float *const *const pRFout
);

/**
 * \brief Reads one frame of input samples for a ROI
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal state currentFrame has been updated
 * \param[in]   pInst : File instance
 * \param[out]  pDepthRange: RF data of size numSamplesPerLine x nrlements x numTransmitPulses
 * \param[in]   pPage: Frame number to read
 * \param[in]   frameNum: the frame number to read
 * \return      int   : 0 on success
 */
int iusInputFileReadFrameDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const *const pPage,
    int frameNum
);

/**
 * \brief Reads next (or first) frame of input samples in a Range
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal state currentFrame has been updated
 * \param[in]   pInst : File instance
 * \param[in]   pDepthRange: depth region of interest
 * \param[out]  pPage: RF data of size numSamplesPerLine x nrlements x numTransmitPulses
 * \return      int   : 0 on success
 */
int iusInputFileReadNextFrameDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const *const pPage
);

int iusInputFileReadNextPulseDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const ppPageOut
);

/**
 * \brief Reads a range of samples of next (or first) event of current frame
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst   : File instance
 * \param[in]   pDepthRange: depth region of interest
 * \param[out]  ppPageOut  : RF data of size numSamplesPerLine x nrlements
 * \return      int     : 0 on success
 */
int iusInputFileReadNextPulseDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const ppPageOut
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
int iusInputFileClose
(
    IusInputFileInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  // _IUSINPUTFILE_H
