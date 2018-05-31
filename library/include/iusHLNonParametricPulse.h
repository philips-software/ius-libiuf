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
iunpp_t iusHLNonParametricPulseCreate
(
    char *label,
    int numPulseValues                /**< number of points to describe waveform, 0 implies a parametric description only */
);


int iusHLNonParametricPulseDelete
(
    iunpp_t pulse
);


int iusHLNonParametricPulseCompare
(
    iunpp_t reference,
    iunpp_t actual
);

int iusHLNonParametricPulseGetNumValues
(
    iunpp_t pulse
);

float iusHLNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,
    int index
);

float iusHLNonParametricPulseGetValueTime
(
    iunpp_t pulse,
    int index
);

int iusHLNonParametricPulseSetValue
(
    iunpp_t pulse,
    int index,
    float pulseTime,
    float pulseAmplitude
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H