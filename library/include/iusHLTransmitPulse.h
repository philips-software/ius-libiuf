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
    float * pRawPulseAmplitudes,          /**< shape of waveform [in Volts] */
    float * pRawPulseTimes,               /**< corresponding timestamps of amplitudes [in seconds] */
    int     numPulseValues                /**< number of points to describe waveform, 0 implies a parametric description only */
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

#endif //IUSLIBRARY_IUSHLTRANSMITPULSE_H
