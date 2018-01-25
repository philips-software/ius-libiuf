//==============================================================================
//     COPYRIGHT 2018 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusCWC.h
//                  instance describing ultrasound Coherent Wave Compounded data
//  iusVersion    : 255.255.255.255
//  author        : Frank van Heesch, Ben Bierens, Harm Belt
//
//==============================================================================

#ifndef _IUSCWC_H
#define _IUSCWC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusTypes.h"       
#include "iusNode.h"        
#include "iusLUT.h"   // for the struct type called iusIntervalLUT

#define IUS_CWC_TYPE "IUSCWCFILETYPE_V3"


/**
 *  \brief IusInput internal data structure
 */
typedef struct
{
    IusNode     iusNode;
    /*
     * Properties that fully describe Cwc data
     */
    int         numFrames;          /**< The number of frames in the data       */
    int         numComponents;      /**< Number of components per frame         */
    IusGrid *   pSampleGrid;        /**< spatial grid where the samples occur   */
    float *     pTimeStamps;        /**< numFrames * numComponents time stamps indicating when the components occur */
    float       centerFrequency;    /**< [Hz]                                   */
    float       speedOfSound;       /**< [m/s]                                  */
    //int       numTransducerElements;  /**< []                                 */
    //float     elementWidth;       /**< [m]                                    */
    //float     kerf;               /**< [m]                                    */
    //float **  ppElementPositions; /**< x,y,z [m]                              */
    //int       numLinesPerComponent;    /**< pre: must equal numTransducerElements  */
    //int       numSamplesPerLine;  /**< []                                     */
    //float     sampleFrequency;    /**< [Hz]                                   */
    //int       numThetaGridPoints; /**< []                                     */
    //int       numRadiusGridPoints;/**< []                                     */
    //float     focusRadius;        /**< [m], <0 is diverging, >0 is converging =0 is plane waves */
    //float *   pFocusAngle;        /**< [rad], has numTransmitPulses elements, irrelevant for plane waves   */
    //float *   pApodR;             /**< receive apodization                    */

    int IusVersion;               /**< version of input file format */
} IusCwcInstance;


/**
 * \brief CWX polar Region Of Interest (ROI)
 */
typedef struct
{
    int   startThetaGridPointROI;           /**< index of first value  */
    int   numThetaGridPointsROI;            /**< amount of values      */
    int   startRadiusGridPointROI;          /**< index of first value  */
    int   numRadiusGridPointsROI;           /**< amount of values      */
    int   startComponent;                   /**< start component       */
    int   numComponents;                    /**< amount of components  */
} IusCwcPolarROI;


/**
 * \brief Returns the amount of static memory that the Create method would
 * allocate for a given parameter set. Keep below 1 GBytes to avoid system
 * instability from a failing malloc.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty
 * \post        The instance object is empty
 * \param[in]   numTransducerElements  number of transducers
 * \param[in]   numTransmitPulses       number of transmit events in one frame
 * \param[in]   numLinesPerPulse        number of lines per wave (=numElements)
 * \param[in]   numSamplesPerLine      number of samples per line
 * \param[in]   numThetaGridPoints     number of elements of pThetaGridPoints
 * \param[in]   numRadiusGridPoints    number of elements of pRadiusGridPoints
 * \param[in]   upFactor               upsample factor for RF data samples
 * return       number of bytes that would be allocated for this parameter set
 *              with iusCwcCreate
 */
//long long iusCwcStaticMemoryUsage
//(
// int                 numTransducerElements,
// int                 numTransmitPulses,
// int                 numLinesPerPulse,
// int                 numSamplesPerLine,
// int                 numThetaGridPoints,
// int                 numRadiusGridPoints,
// int                 upFactor
//);


/**
 * \brief Creates a Cwc instance with the specified parameters,
 * allocates internal memory and initializes internal data.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty
 * \post        The instance object is initialized and pars have been set
 *
 * \param[in]   centerFrequency        center frequency in Hz
 * \param[in]   numTransducerElements  number of transducers
 * \param[in]   elementWidth           width of each transducer in m
 * \param[in]   kerf                   horizontal space between transducers in m
 * \param[in]   numComponents          number of compound results in one frame, typically the ensemblesize
 * \param[in]   numLinesPerPulse       number of lines per wave (=numElements)
 * \param[in]   numSamplesPerLine      number of samples per line
 * \param[in]   sampleFrequency        sampling frequency in Hz
 * \param[in]   speedOfSound           speed of sound in the medium in m/s
 *
 * \ param[in]   pApodR                 array with receive apodization values
 * \ param[in]   numThetaGridPoints     number of elements of pThetaGridPoints
 * \ param[in]   numRadiusGridPoints    number of elements of pRadiusGridPoints
 * \ param[in]   upFactor               upsample factor for RF data samples
 * \ param[in]   applyWeightedCompounding   zero or non-zero
 * \return      iusCwc instance pointer
 */
IusCwcInstance * iusCwcCreate
(
    int                   numFrames,          /**< The number of frames in the data */
    int                   numComponents,      /**< []                                     */
    const IusGrid * const pSampleGrid,        /**< two dimensional grid where the samples occus */
    const float * const   pTimeStamps,        /**< [] numComponents time stamps indicatign when the components occus in a frame */
    float                 centerFrequency,    /**< [Hz]                                   */
    float                 speedOfSound        /**< [m/s]                                  */
 //float               centerFrequency,
 //int                 numTransducerElements,
 //float               elementWidth,
 //float               kerf,
 //int                 numComponents,
 //IusGrid *           pSampleGrid,
 //int                 numLinesPerPulse,
 //int                 numSamplesPerLine,
 //float               sampleFrequency,
 //float               speedOfSound
 //float * const       pApodR,                     /* to be removed bacause it is an algorithm parameter*/
// int                 numThetaGridPoints,         /* to be removed bacause it is an algorithm parameter*/
// int                 numRadiusGridPoints,        /* to be removed bacause it is an algorithm parameter*/
// int                 upFactor,                   /* to be removed bacause it is an algorithm parameter*/
 //int                 applyWeightedCompounding    /* to be removed bacause it is an algorithm parameter*/
);


/**
 * Reads a coherent waveform compounding instance and populates it with the
 * data from the HDF5 handle.
 *
 * \brief reads the Cwc data structure from a (file) handle
 * \param[in]   handle         : Handle to the HDF file or group which
 *                               represents the root node for an input data
 *                               structure
 * \param[in]   verbose        : when non-zero messages are printed to stdout
 * \return      IusCwcInstance : Instance pointer (or NULL)
 */
IusCwcInstance * iusCwcRead
(
    hid_t handle,
    int   verbose
);


/**
 * Writes an input instance to a handle
 *
 * \brief parameters for writing the data
 * \param[in]   handle  : Handle to write the IusInputInstance data to
 * \param[in]   pIusCwc : Instance pointer
 * \param[in]   verbose : when non-zero messages are printed to stdout
 * \return      error code;
 */
int iusCwcWrite
(
    hid_t                        handle,
    const IusCwcInstance * const pIusCwc,
    int                          verbose
);


/**
 * \brief Frees all Cwc resources
 *
 * In-place:   n.a.
 * \pre        The instance object was created
 * \post       The contents of Cwc is freed
 * \param[in]  pInst: pointer to Cwc instance
 * \return     void
 */
void iusCwcDestroy
(
    IusCwcInstance * pInst
);


#ifdef __cplusplus
}
#endif

#endif  /* _IUSCWC_H */
