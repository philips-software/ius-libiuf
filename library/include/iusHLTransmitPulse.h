//
// Created by Ruijzendaal on 30/03/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPULSE_H
#define IUSLIBRARY_IUSHLTRANSMITPULSE_H

#ifndef IUSLIBRARY_IMPLEMENTATION


struct IusDrivingScheme;
typedef  struct IusTransmitPulse IusTransmitPulse;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusTransmitPulse    * iutp_t;
#define  IUTP_INVALID (iutp_t) NULL



// operations
iutp_t iusHLCreateParametricPulse
(
    float pulseFrequency,
    float pulseAmplitude,
    int pulseCount
);

iutp_t iusHLCreateNonParametricPulse
(

    int     numPulseValues                /**< number of points to describe waveform, 0 implies a parametric description only */
);

int iusCompareTransmitPulse
(
    IusTransmitPulse *reference,
    IusTransmitPulse *actual
);

int iusHLDeleteTransmitPulse
(
    IusTransmitPulse *pulse
);

// Getters
float iusHLParametricPulseGetFrequency
(
    iutp_t tramsmitPulse
);

float iusHLParametricPulseGetPulseAmplitude
(
    iutp_t transmitPulse
);

int iusHLParametricPulseGetCount
(
    iutp_t transmitPulse
);

int iusHLNonParametricPulseGetCount
(
    iutp_t transmitPulse
);

int iusHLNonParametricPulseSetAmplitudeTime
(
    iutp_t transmitPulse,
    float pulseTime,
    float pulseAmplitude,
    int index
);

#endif //IUSLIBRARY_IUSHLTRANSMITPULSE_H
