//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICPULSE_H
#define IUSLIBRARY_IUSHLPARAMETRICPULSE_H




// ADT
typedef struct IusParametricPulse IusParametricPulse;
typedef IusParametricPulse *iupp_t;
#define  IUPP_INVALID (iupp_t) NULL

iupp_t iusHLParametricPulseCreate
(
char *label,
float pulseFrequency,
float pulseAmplitude,
int pulseCount
);

int iusHLParametricPulseDelete
(
iupp_t pulse
);


// operations

int iusHLParametricPulseCompare
(
iupp_t reference,
iupp_t actual
);

// Getters
float iusHLParametricPulseGetFrequency
(
    iupp_t pulse
);

float iusHLParametricPulseGetPulseAmplitude
(
    iupp_t pulse
);

int iusHLParametricPulseGetCount
(
    iupp_t transmitPulse
);

#endif //IUSLIBRARY_IUSHLPARAMETRICPULSE_H
