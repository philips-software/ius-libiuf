//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusVelocityFile.h
//                  Routines for i/o on velocity file containing velocity
//                  vectors and velocity variances (one vector and variance per
//                  spatio-temporal neighborhood)
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#ifndef _IUSVELOCITYFILE_H
#define _IUSVELOCITYFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdf5.h>
#include "fileio/iusCWXFile.h"
#include "base/iusTypes.h"

/** 
 * \brief Velocity file data structure
 */
typedef struct 
{

    int IUSVersion;                       /**< Version of ius library */
    char *ID;                             /**< Unique file identifier */

    enum IUSGridType gridType;            /**< is pGrid, Polar, Cartesian or Sparse? */
    IUSGrid grid;                         /**< the grid, can be polar or cartesian, full (or sparse) */
    int startFrame;                       /**< The frame in the Input file that processing was strated */
    int numFrames;                        /**< The number of input frames that have been processed */

    int hasVariance;                      /**< Is Variance data included */
    int hasAbsCorr ;                      /**< Is Abs Corr data included */
    
    /* * state variables */
    hid_t fileChunkConfig;                /**< chunk handle */
    hid_t handle;                         /**< file handle */
    hid_t velocityData;                    /**< velocity handle */
    hid_t velocityVariance;               /**< velocity variance handle */
    hid_t velocityAbsCorr;                /**< velocity AbsCorr handle */
    int currentFrame;                     /**< current frame number */
} IUSVelocityFileInstance;

/** 
 * \brief Creates a Velocity File instance from the specified parameters, allocates
 * internal memory and initializes internal header data.
 *
 * In-place:    n.a.
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   pFullFileName         : full name of the output file (path + name + ext)
 * \param[in]   startFrame            : the frame number in the input file that processing was started 
 * \param[in]   numFrames             : the number of frames in the file
 * \param[in]   gridType              : The velocities can be defined on different types of grids (IUS_GRID_POLAR, IUS_GRID_CARTSIAN, etc.)
 * \param[in]   pGrid                 : The grid of the velocity data (and the variance) 
 * \param[in]   hasVariance           : For each velocity vector a variance is known 
 * \param[in]   hasAbsCorr            : For each velocity vector the absolute value of the complex correlation is known 
 * \param[in]   timeStampOfStartFrame : in [s] The starting timestamp of the velocity frames (typically derived from a parent algorithm)
 * \param[in]   pFrameDelays          : in [s] for each frame the time span it is valid. pFrameDelays[i] is the duration of the ith frame.
 * \param[in]   numAlgParams          : number of algorithmic parameters that are stored in the header
 * \param[in]   pAlgParams            : General algorithmic values (floats)
 * \param[in]   verbose               : generate verbose output
 * \return      File instance pointer (or NULL)
 */
IUSVelocityFileInstance * iusVelocityFileCreate
(
    const char *           pFullFileName,
    IUSCWXFileInstance *   pCWXFileInst,
    int                    startFrame,
    int                    numFrames,
    enum IUSGridType       gridType,
    IUSGrid *              pGrid,
    int                    hasVariance,
    int                    hasAbsCorr,
    float                  timeStampOfStartFrame,
    float *                pFrameDelays,
    int                    numAlgParams,
    float *                pAlgParams,
    int                    verbose
);

/** 
 * \brief Opens a Velocity File instance from an existing file, allocates
 * internal memory and initializes internal data.
 *
 * In-place:    n.a.
 * \pre         The instance object is empty
 * \post        The instance object is initialized and parameters have been set
 * \param[in]   pFullFileName : full name of hdf5 input file (path + name + ext)
 * \param[in]   verbose       : boolean, print file information to stdout
 * \return      File instance pointer (or NULL)
 */
IUSVelocityFileInstance * iusVelocityFileOpen
(
    const char * const pFullFileName,
    const int          verbose
);

/**
 * \brief Reads a specific frame of Velocity samples 
 *
 * In-place:    n.a.
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst                   : file instance for InputFile
 * \param[out]  ppVelocityFrame         : 2D array of Vx,Vy,Vz vectors, size defined by the grid
 * \param[out]  ppVelocityVarianceFrame : 2D array of variances, size defined by the grid
 * \param[out]  ppVelocityAbsCorrFrame  : 2D array of absolute correlation values , size defined by the grid
 * \param[in]   frameNum                : the frame index in the sequence
 * \return      int                     : 0 on success
  */
int iusVelocityReadFrame
(    
    IUSVelocityFileInstance *pInst,
    IUSVelocity * *         ppVelocityFrame,
    float * *               ppVelocityVarianceFrame,
    float * *               ppVelocityAbsCorrFrame,
    int                     frameNum
);

/**
 * \brief Reads frame of Velocity samples. 
 *
 * In-place:    n.a.
 * \pre         The instance object was created. Output data lines have been allocated.
 * \post        Data was read, internal states have been updated.
 * \param[in]   pInst                   : file instance for InputFile
 * \param[out]  pppVelocityFrame        : 2D velocity matrix of size 2 x numRadiusGridPoints x numThetaGridPoints
 * \param[out]  ppVelocityVarianceFrame : 2D velocity variance matrix of size numRadiusGridPoints x numThetaGridPoints
 * \param[out]  ppVelocityAbsCorrFrame  : 2D array of absolute correlation values , size defined by the grid
 * \return      int                     : 0 on success
  */
int iusVelocityReadNextFrame
(    
    IUSVelocityFileInstance * pInst,
    IUSVelocity * *           ppVelocityFrame,
    float * *                 ppVelocityVarianceFrame,
    float * *                 ppVelocityAbsCorrFrame
);

/**
* \brief Writes a frame of Velocity samples with a specific frame number and increments
*        internal frame counter.
*
* In-place:    n.a
* \pre         The instance object was created. Output data lines have been allocated.
* \post        Data was written, internal frame counter was incremented
* \param[in]   pInst                   : File instance
* \param[in]   ppVelocityFrame         : 2D velocity matrix of size 2 x numRadiusGridPoints x numThetaGridPoints
* \param[in]   ppVelocityVarianceFrame : 2D velocity variance matrix of size numRadiusGridPoints x numThetaGridPoints
* \param[out]  ppVelocityAbsCorrFrame  : 2D array of absolute correlation values , size defined by the grid
* \param[in]   frameNum                : The frame number to write
* \return      int                     : 0 on success
*/
int iusVelocityWriteFrame
(
    IUSVelocityFileInstance * pInst,
    IUSVelocity * *           ppVelocityFrame,
    float * *                 ppVelocityVarianceFrame,
    float * *                 ppVelocityAbsCorrFrame,
    int                       frameNum
);

/**
* \brief Writes next (or first) frame of Velocity samples and increments internal frame counter.
*
* In-place:    n.a
* \pre         The instance object was created. Output data lines have been allocated.
* \post        Data was written, internal frame counter was incremented
* \param[in]   pInst                   : File instance
* \param[in]   ppVelocityFrame         : 2D velocity matrix of size 2 x numRadiusGridPoints x numThetaGridPoints
* \param[in]   ppVelocityVarianceFrame : 2D velocity variance matrix of size numRadiusGridPoints x numThetaGridPoints
* \param[out]  ppVelocityAbsCorrFrame  : 2D array of absolute correlation values , size defined by the grid
* \return      int      : 0 on success
*/
int iusVelocityWriteNextFrame
(
    IUSVelocityFileInstance * pInst,
    IUSVelocity * *           ppVelocityFrame,
    float * *                 ppVelocityVarianceFrame,
    float * *                 ppVelocityAbsCorrFrame
);


/** 
 * \brief Frees all File resources and closes file.
 *
 * In-place:   n.a.
 * \pre        The instance object was created
 * \post       The contents of File is freed
 * \param[in]  pInst:  pointer to File instance
 * \return     0 on success
 */
int iusVelocityFileClose
(
    IUSVelocityFileInstance * const pInst
);

#ifdef __cplusplus
}
#endif

#endif  // _IUSVELOCITYFILE_H
