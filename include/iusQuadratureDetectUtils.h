//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusQuadratureDetectUtils.h
//
//  iusVersion    : 255.255.255.255
//
//==============================================================================


#ifndef _IUSQUADRATUREDETECT4SAMPLESPERWAVE_H
#define _IUSQUADRATUREDETECT4SAMPLESPERWAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusComplex.h"

/**
 * \brief Runs a QuadratureDetect4SamplesPerPulse instance (once), taking input
 *        data and creating output data. The algorithm can be called on a
 *        line-by-line basis, or on the basis of an input matrix containing a
 *        number of RF lines. Each call to iusQuadratureDetect4SamplesPerPulse
 *        produces a matrix with the I and Q components of each input line.
 *        This routines makes explicit use of the assumption that the input data
 *        was sampled at a rate which is 4 times larger than the central
 *        frequency of the input data, so that for the central frequency there
 *        are precisely 4 samples per wave. Advantageously the computations are
 *        fast.
 *
 * In-place:    n.a.
 * \pre         Input and output data arrays have been allocated. Input array
 *              contains data.
 *              numLines == numLinesOut.
 *              If numSamplesPerLine is not an integer multiple of 2 then the
 *              last samples of each input ppRfLinesIn[.] are ignored.
 *              numSamplesPerLineOut == floor( numSamplesPerLine / 2 ).
 * \post        With numLines new input RF lines (including history of RF lines
 *              stored inside pInst) one output velocity vector and one output
 *              velocity variances vector have been created.
 * \param[in]   ppRFLinesIn          : matrix containing numLines consecutive
 *                                     RF lines with numSamplesPerLine samples
 *                                     per RF line. The first index of
 *                                     ppRfLinesIn is the line index, hence
 *                                     ppRFLineIn[i] is a pointer to the i-th RF
 *                                     line (with i = 0, 1, ..., numLines-1).
 * \param[in]   numLines             : number of RF lines in ppRFLinesIn
 * \param[in]   numSamplesPerLine    : number of samples per RF line.
 * \param[out]  pIQ                  : complex matrix with quadrature output I,Q
 * \param[in]   numLinesOut          : number of I,Q lines in pIQ
 * \param[in]   numSamplesPerLineOut : number of samples per line in pIQ
 * \return      void
 */
void iusQuadratureDetect4SamplesPerPulse
(
    const float * const * const   ppRfLinesIn,
    int                           numLines,
    int                           numSamplesPerLine,
    iusComplexFloatMatrix * const pIQ,
    int                           numLinesOut,
    int                           numSamplesPerLineOut
);

/**
 * \brief Compute from a packet of quadrature component vectors I and Q one
 *        vector of velocity values (in amount of samples displacement per
 *        subsequent line period) and one vector of velocity std values.
 * In-place:    n.a.
 * \pre         Input and output data arrays have been allocated. Input array
 *              contains data.
 *              ensembleSize > 1, packetSize >= 1.
 *              numElemOut = floor( numSamplesPerLine / packetSize ).
 *              If numSamplesPerLine is not an integer multiple of packetSize
 *              then the last samples of each input ppI[.] and ppQ[.] line
 *              are ignored.
 *              numSamplesPerLineOut == floor( numSamplesPerLine / 2 ).
 * \post        Output arrays pVOut and pSigmaVOut are filled with result data
 * \param[in]   pIQ               : complex matrix containing ensembleSize
 *                                  consecutive lines with numSamplesPerLine
 *                                  samples of I and Q.
 * \param[in]   ensembleSize      : number of I,Q lines in the ensemble.
 * \param[in]   numSamplesPerLine : number of samples per I and Q line.
 * \param[out]  packetSize        : amount of subsequent I,Q values in one line
 *                                  taken together for the computation of 1
 *                                  pVOut and 1 pSigmaVOut value.
 * \param[out]  pVOut             : vector of velocity estimates per packet
 * \param[out]  pSigmaVOut        : vector of velocity std estimates per packet
 * \param[out]  numElemOut        : number of I and Q values in one line
 * \return      void
 */
void iusKasaiCorr1D
(
    const iusComplexFloatMatrix * const pIQ,
    int                                 ensembleSize,
    int                                 numSamplesPerLine,
    int                                 packetSize,
    float * const                       pVOut,
    float * const                       pSigmaVOut,
    int                                 numElemOut
);

/**
* \brief Compute from a quadrature component vector I and Q one
*        velocity value (in amount of samples displacement per
*        subsequent line period) and velocity std value.
* In-place:    n.a.
* \pre         Input and output data arrays have been allocated. Input array
*              contains data.
*              ensembleSize > 1
* \post        Output arrays pVOut and pSigmaVOut are filled with result data
* \param[in]   pIQ               : complex vector containing ensembleSize
*                                  consecutive lines with one I and Q sample.
* \param[in]   ensembleSize      : number of I,Q lines in the ensemble.
* \param[out]  vOut              : velocity estimate
* \param[out]  sigmaVOut         : velocity std estimate
* \return      void
*/
void iusKasaiPerPacketCorr1D
(
    const iusComplexFloatVector * const pIQ,
    int                                 ensembleSize,
    float                               vOut,
    float                               sigmaVOut
);

/**
* \brief Compute from a packet of quadrature component vectors I and Q one
*        vector of velocity values (in amount of samples displacement per
*        subsequent line period) and one vector of velocity std values.
* In-place:    n.a.
* \pre         Input and output data arrays have been allocated. Input array
*              contains data.
*              ensembleSize > 1, packetSize >= 1.
*              Difference with iusKasaiCorr1D is that averaging (from lines
*              to packets) is done before correlation here. The creation of
*              packets should happen before calling this function.
* \post        Output arrays pVOut and pSigmaVOut are filled with result data
* \param[in]   pIQPacketData     : complex matrix containing numPackets rows 
*                                  with ensembleSize samples of I and Q.
* \param[in]   numPackets        : number of packets; a velocity and std value
*                                  is calculated for each packet.
* \param[in]   ensembleSize      : number of I,Q lines in the ensemble.
* \param[out]  pVOut             : vector of velocity estimates per packet
* \param[out]  pSigmaVOut        : vector of velocity std estimates per packet
* \return      void
*/
void iusKasaiMultiplePacketsCorr1D
(
    const iusComplexFloatMatrix * const pIqPacketData,
    int                                 numPackets,
    int                                 ensembleSize,
    float * const                       pVOut,
    float * const                       pSigmaVOut
);


#ifdef __cplusplus
}
#endif

#endif  // _IUSQuadratureDetect4SamplesPerPulse_H
