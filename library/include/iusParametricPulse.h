//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICPULSE_H
#define IUSLIBRARY_IUSHLPARAMETRICPULSE_H

// ADT
typedef struct IusParametricPulse IusParametricPulse;
typedef IusParametricPulse *iupp_t;
#define  IUPP_INVALID (iupp_t) NULL

iupp_t iusParametricPulseCreate
(
    float pulseFrequency,
    float pulseAmplitude,
    int numPulses
);

int iusParametricPulseDelete
(
    iupp_t pulse
);


// operations

IUS_BOOL iusParametricPulseCompare
(
    iupp_t reference,
    iupp_t actual
);

// Getters
float iusParametricPulseGetFrequency
(
    iupp_t pulse
);

float iusParametricPulseGetPulseAmplitude
(
    iupp_t pulse
);

int iusParametricPulseGetNumPulses
(
    iupp_t transmitPulse
);

#endif //IUSLIBRARY_IUSHLPARAMETRICPULSE_H
