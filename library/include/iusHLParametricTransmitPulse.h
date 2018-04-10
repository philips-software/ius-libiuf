//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICTRANSMITPULSE_H
#define IUSLIBRARY_IUSHLPARAMETRICTRANSMITPULSE_H

#ifndef IUSLIBRARY_IMPLEMENTATION

typedef struct IusParametricTransmitPulse IusParametricTransmitPulse;

#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef IusParametricTransmitPulse *iuptp_t;
#define  IUPTP_INVALID (iutp_t) NULL

iuptp_t iusHLCreateParametricPulse
(
    float pulseFrequency,
    float pulseAmplitude,
    int pulseCount
);

int iusHLDeleteParametricPulse
(
    iuptp_t pulse
);


// operations

int iusCompareParametricPulse
(
    iuptp_t reference,
    iuptp_t actual
);

// Getters
float iusHLParametricPulseGetFrequency
(
    iuptp_t tramsmitPulse
);

float iusHLParametricPulseGetPulseAmplitude
(
    iuptp_t transmitPulse
);

int iusHLParametricPulseGetCount
(
    iuptp_t transmitPulse
);

#endif //IUSLIBRARY_IUSHLPARAMETRICTRANSMITPULSE_H
