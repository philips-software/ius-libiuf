//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusCWCFile.h
//                  CWC file (in hdf5 format)
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#ifndef _IUSCWCFILE_H
#define _IUSCWCFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#define IUS_CWC_TYPE "IUSCWCFILETYPE_V3"

#include "iusCWC.h"
#include <hdf5.h>


/**
 * \brief CWC internal data structure
 */
typedef struct
{
    IusCwcInstance * pIusCwc;     /**< The data object                        */

    /*
     * state variables
     */
    hid_t fileChunkConfig;        /**< file chunck handle                     */
    hid_t handle;                 /**< file handle                            */
    hid_t cwcDataset;             /**< dataset handle                         */

    int   currentFrame;           /**< current frame number                   */
    int   currentComponent;       /**< current component number               */
} IusCwcFileInstance;


/**
 * \brief Creates a File instance from the specified parameters, allocates
 * internal memory and initializes internal header data.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFullFileName : full name of hdf5 output file (path+name+ext).
 * \param[in]   pInst         : Cwc data instance pointer (or NULL)
 * \return      File instance pointer
 */
IusCwcFileInstance * iusCwcFileCreate
(
    const char * const     pFullFileName,
    IusCwcInstance * const pInst,
    int                    version
);


/**
 * \brief Opens a File instance from an existing file, allocates
 * internal memory and initializes internal data, except the CwcData itself
 *
 * In-place:   n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFullFileName : full name of hdf5 input file (path + name + ext)
 * \param[in]   verbose       : boolean, print file information to stdout.
 * \return      File instance pointer (or NULL)
 */
IusCwcFileInstance * iusCwcFileOpen
(
    const char * const pFullFileName,
    const int          verbose
);


/**
 * \brief Reads existing coherent waveform compound data (Cwc) from a File
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data has been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst      : File instance
 * \param[out]  ppppCwcOut : Cwc data 
 * \return      int        : 0 on success
 */
int iusCwcFileRead
(
    IusCwcFileInstance * const            pInst,
    float * const * const * const * const ppppCwcOut   /* TODO: 4 levels :-)  please use filetypes :-)*/
);


/**
 * \brief Reads a specific frame of cwc samples
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been
 *              allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst     : File instance for InputFile
 * \param[out]  pppCwcOut : Cwc data of size 2D grid * I/Q/Lambda 
 * \param[in]   frameNum  : the frame number to read.
 * \return      int       : 0 on success
 */
int iusCwcFileReadFrame
(
    IusCwcFileInstance * const    pInst,
    float * const * const * const pppCwcOut,
    int                           frameNum
);


/**
 * \brief Reads next frame of cwc samples
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been
 *              allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst     : File instance for InputFile
 * \param[out]  pppCwcOut : Cwc data of size 2D grid * I/Q/Lambda 
 * \return      int       : 0 on success
 */
int iusCwcFileReadNextFrame
(
    IusCwcFileInstance * const    pInst,
    float * const * const * const pppCwcOut
);


/**
 * \brief writes a frame of cwc samples with a specific frame number and
 *        increments internal frame counter
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been
 *              allocated.
 * \post        Data was written, internal frame counter was incremented
 * \param[in]   pInst    : file instance
 * \param[in]   pppFrame : data matrix of size numComponents x ...
 *                             numRadiusGridPoints x numThetaGridPoints
 * \param[in]   frameNum : the frame number to write
 * \return      int      : 0 on success
 */
int iusCwcFileWriteFrame
(
    IusCwcFileInstance * const          pInst,
    const float * const * const * const pppFrame,
    int                                 frameNum
);


/**
 * \brief writes next (or first) frame of cwc samples and increments internal
 * frame counter
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been
 *              allocated.
 * \post        Data was written, internal frame counter was incremented
 * \param[in]   pInst    : file instance
 * \param[in]   pppFrame : data matrix of size numComponents x
 *                         numRadiusGridPoints x numThetaGridPoints
 * \return      int      : 0 on success
 */
int iusCwcFileWriteNextFrame
(
    IusCwcFileInstance * const          pInst,
    const float * const * const * const pppFrame
);



/**
 * \brief set polar region of interest of cwx samples
 *
 * In-place:   n/a
 * \pre         The file object was created, the IusCwcPolarROI pointer was
 *              initialized
 * \post        The region of interest values have been set
 * \param[in]   pFileInst               : file instance (is used for value range
 *                                        checking of the next inputs)
 * \param[in]   indexRadiusGridPointROI : index of first radius value in m for
 *                                        polar ROI
 * \param[in]   numRadiusGridPointsROI  : number of radius values in polar ROI
 * \param[in]   indexThetaGridPointROI  : index of first angle value in rad for
 *                                        polar ROI
 * \param[in]   numThetaGridPointsROI   : number of angle values in polar ROI
 * \param[in]   numComponents           : number of components
 * \param[out]  pPolarROI               : polar region of interest (ROI)
 * \return      int                     : 0 on success
 */
int iusCwcFileSetPolarROI
(
    const IusCwcFileInstance * const pFileInst,
    int                              indexRadiusGridPointROI,
    int                              numRadiusGridPointsROI,
    int                              indexThetaGridPointROI,
    int                              numThetaGridPointsROI,
    int                              numComponents,
    IusCwcPolarROI * const           pPolarROI
);


/**
 * \brief writes a polar ROI of cwx samples in the frame with a specific frame
 * number and increments internal frame counter
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been
 *              allocated.
 * \post        Data was written, internal frame counter was incremented
 * \param[in]   pFileInst   : File instance
 * \param[in]   pppFrameROI : data array of size numComponents x ...
 *                               numRadiusGridPointsROI x numThetaGridPointsROI
 * \param[in]   frameNum    : the number of the frame to which the ROI data is
 *                            written
 * \param[in]   pPolarROI   : polar Region Of Interest (ROI) within a full frame
 * \return      int         : 0 on success
 */
int iusCwcFileWriteFramePolarROI
(
    IusCwcFileInstance * const          pFileInst,
    const float * const * const * const pppFrameROI,
    int                                 frameNum,
    const IusCwcPolarROI * const        pPolarROI
);


/**
 * \brief writes polar ROI of cwx samples in the next frame and increments
 * internal frame counter
 *
 * In-place:   n/a
 * \pre         The instance object was created. Output data lines have been
 *              allocated.
 * \post        Data was read, pInst->currentFrame is set to next frame
 * \param[in]   pFileInst   : File instance
 * \param[in]   pppFrameROI : data array of size numComponents x ...
 *                                numRadiusGridPointsROI x numThetaGridPointsROI
 * \param[in]   pPolarROI   : polar Region Of Interest (ROI) within a full frame
 * \return      int         : 0 on success
 */
int iusCwcFileWriteNextFramePolarROI
(
    IusCwcFileInstance * const          pFileInst,
    const float * const * const * const pppFrameROI,
    const IusCwcPolarROI * const        pPolarROI
);


/**
 * \brief Frees all File resources and closes file.
 *
 * In-place:   n.a.
 * \pre        The instance object was created.
 * \post       The contents of File is freed.
 * \param[in]  pInst :  pointer to File instance
 * \return     0 on success
 */
int iusCwcFileClose
(
    IusCwcFileInstance * const pInst
);

#ifdef __cplusplus
}
#endif

#endif  // _IUSCWCFILE_H
