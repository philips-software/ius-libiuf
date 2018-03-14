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

#include "iusTypes.h"
#include "iusNode.h"
 
#define IUS_INPUT_TYPE "IUSINPUTFILETYPE_V3"


/** \brief Transducer element description: position, orientation and size */
typedef struct
{
     IusPosition position; /**< 3D Location of the element */
     IusAngle    angle;    /**< orientation of the elements */
     IusSize     size;     /**< size of the element */
} IusTransducerElement;


/** \brief Time gain control point (time, gain) */
typedef struct
{
    float time;         /**< timestamp in seconds */
    float gain;         /**< value dimensionless */
} IusTimeGainControl;


/** \brief Transmitpattern point (time, index) i.e. source triggers at time */
typedef struct
{
    int    index;       /**< index of transmit source */
    float  time;        /**< time to the source transmits */
} IusTransmitPattern;


/** \brief ... */
typedef struct
{
    int   numChannels;  /**< The number of simultaneous active elements per pulse */
    int * pChannelMap;  /**< A table of size numTransmitPulses x numChannels  at each index the transducer element that is mapped to a (channel,transmitPulse) */
} IusChannelCoding;


/** \brief Possible driving patterns */
enum IusDrivingSchemeType
{
    IUS_DIVERGING_WAVES = 0, /**< focus point has negative z value */
    IUS_FOCUSED_WAVES,       /**< focus point has positive z value */
    IUS_PLANE_WAVES,         /**< neighbouring elements fire consecutively with constant inter-element delay */
    IUS_SINGLE_ELEMENT,      /**< simplified case where there is only one element */
    IUS_CUSTOM_WAVES         /**< non-specified driving scheme category, the transmit pattern and transmit delays of elements can be anything */
};


/** \brief An Ultrasound experiment is identified by a date and a description, also the speed of sound has been determined */
typedef struct
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * pDescription;    /**< Experiment notes */
} IusExperiment;


/** \brief a Transducer object */
typedef struct
{
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    int                    numElements;       /**< number of transducer Elements in the probe */
    IusTransducerElement * pElements;         /**< an array of numElements transducer element (position, angle, size) */
} IusTransducer;

/** \brief Ultrasound recording settings (TGC and Fs) */
typedef struct
{
    IusChannelCoding     receiveChannelCoding;      /**< Describes the mapping between transducer elemenents and receive pulses */
    float                sampleFrequency;           /**< The sampling frequency that was used */
    float *              pStartDepth;               /**< The start depths of RFlines, array length is the number of pulses per frame, values are in seconds */
    float *              pEndDepth;                 /**< The end depths of RFLines, this data can be considered redundant as it is startDepth+(sampleFrequency*numSamplesPerLine)  */
    int                  numTimeGainControlValues;  /**< number of control points that describe the TGC */
    IusTimeGainControl * pTimeGainControl;          /**< TimeGainControl points (time,gain) */
} IusReceiveSettings;

/** \brief Transmit wave object
 *  \details The Transmitwave function is described by a set of points
 *  (time,voltage) to know what has been sent, but also parametrically by
 *  pulseFrequency (Hz), pulseAmplitude (V), and pulseCount for calculation
 *  purposes.
 */
typedef struct
{
    int     numPulseValues;       /**< number of points to describe waveform, 0 implies a parametric description only */
    float * pRawPulseAmplitudes;  /**< shape of waveform [in Volts] */
    float * pRawPulseTimes;       /**< corresponding timestamps of amplitudes [in seconds] */
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount;           /**< number of cycles that the pulse represents */
} IusTransmitPulse;


/** \brief the driving scheme for an experiment */
typedef struct
{
    enum IusDrivingSchemeType drivingSchemeType;      /**< driving scheme: e.g. diveringwaves, planeswaves, ... */
    int                       numSamplesPerLine;      /**< length of an acquisition line */
    int                       numTransmitSources;     /**< number of US sources (tyically these are virtual) */
    int                       numTransmitPulses;      /**< number of pulses in a frame */
    //int numFrames;                                  /**< number of repetitions of the driving pattern */
    IusPosition *             pSourceLocations;       /**< position of the US sources in case of CARTESIAN coordinates*/
    float                     sourceFNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float                     sourceAngularDelta;     /**< angle in [rad] between sources */
    float                     sourceStartAngle;       /**< angle in [rad] between sources */
    float                     transmitPatternDelay;   /**< extra delay at the end of a transmit pattern */
    IusTransmitPattern *      pTransmitPattern;       /**< array (time, index) of length numTransmitPulses */
    IusTransmitPulse          transmitPulse;          /**< waveform of the transmit pulse */
    float *                   pTransmitApodization;   /**< 2D array: per transmit event we have numElements gains */
    IusChannelCoding          transmitChannelCoding;  /**< Describes the mapping between transducer elemenents and transmit pulses */
} IusDrivingScheme;
 

/**
 * \brief IusInput internal data structure
 */
typedef struct
{
    IusNode              iusNode;
	 /*
     * Properties that fully describe CWC data
     */
    IusExperiment      * pExperiment;      /**< experiment settings */
    IusTransducer      * pTransducer;      /**< transducer that has been used */
    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */
    
    int                  numFrames;        /**< The number of frames in the data */
    int                  IusVersion;       /**< version of input file format */
} IusInputInstance;


/**
 * \brief Set depth region of interest of input samples
 */
int iusInputSetDepthRange
(
    const IusInputInstance * const pInst,
    int                            startIndex,
    int                            numSamples,
    IusRange * const               pDepthRange
);

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

IusInputInstance * iusInputCreate
        (
                IusNode      * pNode,
                int numFrames
        );

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
    IusInputInstance * pIusInput
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
