
//===========================================================================*/
//     COPYRIGHT 2016 PHILIPS RESEARCH
//===========================================================================*/
//
//  Source Name   : iusCWC4DWD.c
//                  Coherent Wave Compounding for Diverging Wave Driving
//  iusVersion    : 255.255.255.255
//  author        : Harm Belt and Kees Janse
//
//==============================================================================

#ifndef _IUSALGOCWC4DWD_H
#define _IUSALGOCWC4DWD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusLUT.h"   // for the struct type called iusIntervalLUT
#include "iusTypes.h" // for the struct type called iusGrid


/**
 * \brief Cwc4Dwd internal data structure
 */
typedef struct
{
    float       centerFrequency;       /**< [Hz]                                   */
    int         numTransducerElements; /**< []                                     */
    float       elementWidth;          /**< [m]                                    */
    float       kerf;                  /**< [m]                                    */
    float * *   ppElementPositions;    /**< x,y,z [m]                              */
    int         numTransmitWaves;      /**< []                                     */
    float       focusRadius;           /**< [m], value must be smaller than 0      */
    float *     pFocusAngle;           /**< [rad], has numTransmitWaves elem  */
    int         numLinesPerWave;       /**< must equal numTransducerElements  */
    int         numSamplesPerLine;     /**< []                                     */
    float       sampleFrequency;       /**< [Hz]                                   */
    float       speedOfSound;          /**< [m/s]                                  */
    float *     pApodR;                /**< receive apodization                    */
    IusGrid *   pSampleGrid;
    //int        numThetaGridPoints,
    //int        numRadiusGridPoints,

    int         upFactor;                 /**< [], typical 8, for subsample delays */
    int         applyWeightedCompounding; /**< yes=1, no=0                         */
    /*
     * Internal
     */
    float * * * pppUp;                 /**< temporary for up-sampled RF data       */
    float *     pDelayOffsetSec;       /**< recovering lost common transmit delay  */
    int *       pDelayIndices;         /**<                                 */
    int *       pKStart;               /**< indices for the first wave to compound */
    int *       pKEnd;                 /**< indices for the last wave to compound  */
    int         outputGridWasSet;      /**< checking out grid was set before run   */
} IusCwc4DwdInstance;

/*
 * \brief Returns the amount of static memory that the Create method would
 * allocate for a given parameter set. Keep below 1 GBytes to avoid system
 * instability from a failing malloc.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty
 * \post        The instance object is empty
 * \param[in]   numTransducerElements : number of transducers
 * \param[in]   numTransmitWaves      : number of transmit events in one frame
 * \param[in]   numLinesPerWave       : number of lines per wave (=numElements)
 * \param[in]   numSamplesPerLine     : number of samples per line
 * \param[in]   numThetaGridPoints    : number of elements of pThetaGridPoints
 * \param[in]   numRadiusGridPoints   : number of elements of pRadiusGridPoints
 * \param[in]   upFactor              : upsample factor for RF data samples
 * \return      number of bytes that would be allocated for this parameter set
 *              with iusCwc4DwdCreate
 */
long long iusCwc4DwdStaticMemoryUsage
(
    int                 numTransducerElements,
    int                 numTransmitWaves,
    int                 numLinesPerWave,
    int                 numSamplesPerLine,
    int                 numThetaGridPoints,
    int                 numRadiusGridPoints,
    int                 upFactor
);

/*
 * \brief Creates a Cwc4Dwd instance with the specified parameters,
 * allocates internal memory and initializes internal data.
 *
 * In-place:   n.a.
 * \pre         The instance object is empty
 * \post        The instance object is initialized and pars have been set
 * \brief parameters defining the transducer
 * \param[in]   centerFrequency        center frequency in Hz
 * \param[in]   numTransducerElements  number of transducers
 * \param[in]   elementWidth           width of each transducer in m
 * \param[in]   kerf                   horizontal space between transducers in m
 * \brief parameters defining the input data from diverging waves driving
 * \param[in]   numTransmitWaves       number of transmit events in one frame
 * \param[in]   focusRadius            transmit focus radius in m (> 0)
 * \param[in]   pFocusAngle            array of equidistant focus angles 
 * \param[in]   numLinesPerWave        number of lines per wave (=numElements)
 * \param[in]   numSamplesPerLine      number of samples per line
 * \param[in]   sampleFrequency        sampling frequency in Hz
 * \param[in]   speedOfSound           speed of sound in the medium in m/s
 * \brief parameters for computing the output
 * \param[in]   pApodR                 array with receive apodization values
 * \param[in]   pSampleGrid            the polar position of the output sampels
 * \param[in]   upFactor               upsample factor for RF data samples
 * \param[in]   applyWeightedCompounding   zero or non-zero
 * \return      iusCwc4Dwd instance pointer
 */
IusCwc4DwdInstance * iusCwc4DwdCreate
(
    float               centerFrequency,
    int                 numTransducerElements,
    float               elementWidth,
    float               kerf,
    int                 numTransmitWaves,
    float               focusRadius,
    float * const       pFocusAngle,
    int                 numLinesPerWave,
    int                 numSamplesPerLine,
    float               sampleFrequency,
    float               speedOfSound,
    float * const       pApodR,
    IusGrid * const     pSampleGrid,
//    int                 numThetaGridPoints,
//    int                 numRadiusGridPoints,

    int                 upFactor,
    int                 applyWeightedCompounding
);

/**
 * \brief Sets the output grid 
 * 
 * In-place     n.a.
 * \pre         pInst must have been created by iusCwc4DwdCreate, theta and radius
 *              input pointers must be valid and array lengths need to correspond
 *              with what was set during iusCwc4DwdCreate
 * \post        output grid was set
 * \param[in]   pInst the instance   that contains the ouput grid
 * \param[in]   pThetaGridPoints     the positions of the grid points along the theta axis
 * \param[in]   numThetaGridPoints   the number of grid points in the theta dimension
 * \param[in]   pRadiusGridPoints    the positions of the grid points along the radial axis
 * \param[in]   numRadiusGridPoints  the number of grid points in the radial dimension
 * \param[in]   pNumOverlapBeamsLUT
 */
void iusCwc4DwdSetNewOutputGrid
(
    IusCwc4DwdInstance * const        pInst,
    const IusGrid * const             pSampleGrid,
    const iusFloatIntervalLUT * const pNumOverlapBeamsLUT
);

/**
 * \brief Runs a Cwc4Dwd instance, returning coherently-compounded RF lines
 *
 * In-place:   no (input and output may not occupy the same memory space)
 * \pre         The instance object was created, initialized. In- and output
 *              data lines have been allocated. Input data line full of data.
 * \post        Data was processed, internal states have been updated.
 * \param[in]   pInst    Cwc4Dwd instance
 * \param[in]   pppIn    3D matrix containing input RF data lines for one
 *                       complete scan. The index along the 1st dimension is the
 *                       index of the diverging wave, the index along the second
 *                       dimension is the line index within one of the waves,
 *                       the index along the 3rd dimension is the index of a
 *                       sample in a line.
 * \param[out]  ppOut    2D matrix containing coherently-compounded RF data. The
 *                       index along the first dimension is the index of a line,
 *                       and the index along the second dimension is the index
 *                       of a sample in the line.
 * \return      void
 */
void iusCwc4DwdRun
(
    IusCwc4DwdInstance * const          pInst,
    const float * const * const * const pppIn,
    float * const * const               ppOut
);

/**
 * \brief Frees all Cwc4Dwd resources
 *
 * In-place:   n.a.
 * \pre        The instance object was created
 * \post       The contents of Cwc4Dwd is freed
 * \param[in]  pInst: pointer to Cwc4Dwd instance
 * \return     void
 */
void iusCwc4DwdDestroy
(
    IusCwc4DwdInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  /* _IUSCWC4DWD_H */
