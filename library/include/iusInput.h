//===========================================================================*/
//     COPYRIGHT 2016 PHILIPS RESEARCH
//===========================================================================*/
//
//  Source Name   : iusInput.h
//                  ultrasound raw capture data format
//  iusVersion    : 255.255.255.255
//  author        : Frank van Heesch, Ben Bierens, Harm Belt
//
//==============================================================================

#ifndef _IUSINPUT_H
#define _IUSINPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <iusTypes.h>
#include <iusNode.h>
#include <iusHLApodizationList.h>




/** \brief Time gain control point (time, gain) */
typedef struct
{
    float time;         /**< timestamp in seconds */
    float gain;         /**< value dimensionless */
} IusTimeGainControl;

/** \brief Ultrasound recording settings (TGC and Fs) */
typedef struct
{
    /**< Describes the mapping between transducer elemenents and receive pulses */
    int   numChannels;                              /**< The number of simultaneous active elements per pulse */
    // for indeces, see transmit apodization
    // size should be addressed like pChannelMap[numDelays][numChannels]
    int * pChannelMap;                              /**< A table of size numTransmitPulses x numChannels  at each index the transducer element that is mapped to a (channel,transmitPulse) */
    int                  numSamplesPerLine;         /**< length of an acquisition line */
    float                sampleFrequency;           /**< The sampling frequency that was used */
    int                  numDelays ;                // should be identical to drivingscheme.numtransmitpulses
    float *              pStartDelay;               /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
    float *              pEndDelay;                 /**< The end delay of RFLines, this data can be considered redundant as it is startDepth+(sampleFrequency*numSamplesPerLine)  */
    int                  numTimeGainControlValues;  /**< number of control points that describe the TGC */
    IusTimeGainControl * pTimeGainControl;          /**< TimeGainControl points (time,gain) */
} IusReceiveSettings;




/**
 * \brief Creates an input instance from scratch, e.g. without a parent.
 *
 * \brief parameters for constructing the output
 * \param[in]   pEexperiment      : the experminent information (meta data)
 * \param[in]   pTransducer      : the transducer name and configuration
 * \param[in]   pReceiveSettings : The ultrasound receive configuration
 * \param[in]   pDrivingScheme   : The ultrasound transmit frame configuration
 * \return      IusInputInstance: File instance pointer (or NULL)
 */
#ifdef OLD
IusInputInstance * iusInputCreate
(
    IusExperiment      * pExperiment,
    IusTransducer      * pTransducer,
    IusReceiveSettings * pReceiveSettings,
    IusDrivingScheme   * pDrivingScheme,
    int                  version,
    int                  numFrames
);
#endif


/**
 * \brief Creates an input instance and populates it with the data from the HDF5 handle.
 *
 * \brief parameter for reading the data
 * \param[in]   handle : Handle to the HDF file or group which represents the root node for an input data structure.
 * \return      IusInputInstance : Instance pointer (or NULL)
 */
IusInputInstance * iusInputRead
(
    hid_t handle,
    int   verbose
);

/**
 * \brief Writes an input instance to a handle
 *
 * \brief parameters for writing the data
 * \param[in]   handle : Handle to write the IusInputInstance data to
 * \param[in]   IusInputInstance : Instance pointer
 * \return      error code;
 */
int iusInputWrite
(
    hid_t              handle,
    IusInputInstance * pIusInput,
    int verbose
);

/**
 * \brief Frees all CWC4DWD resources
 *
 * In-place:   n.a.
 * \pre        The instance object was created
 * \post       The contents of CWC4DWD is freed
 * \param[in]  pInst: pointer to CWC4DWD instance
 * \return     void
 */
void iusInputDestroy
(
    IusInputInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  /* _IUSINPUT_H */
