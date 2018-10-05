//
// Created by Ruijzendaal on 10/04/2018.
//
#ifndef IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H
#define IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H

// ADT
typedef  struct IusNonParametricPulse IusNonParametricPulse;
typedef  IusNonParametricPulse    * iunpp_t;
#define  IUNPP_INVALID (iunpp_t) NULL

// operations
iunpp_t iusNonParametricPulseCreate
(
    char *label,                      /**< The name of this pulse */
    int numPulseValues                /**< number of points to describe waveform */
);


int iusNonParametricPulseDelete
(
    iunpp_t pulse
);


int iusNonParametricPulseCompare
(
    iunpp_t reference,
    iunpp_t actual
);

int iusNonParametricPulseGetNumValues
(
    iunpp_t pulse
);

float iusNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,
    int index
);

float iusNonParametricPulseGetValueTime
(
    iunpp_t pulse,
    int index
);

int iusNonParametricPulseSetValue
(
    iunpp_t pulse,
    int index,
    float pulseTime,
    float pulseAmplitude
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H
