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


int iusNonParametricPulseCompare
(
iunpp_t reference,
iunpp_t actual
);

int iusHLNonParametricPulseGetNumValues
(
    iunpp_t pulse
);

float iusHLNonParametricPulseGetAmplitude
(
    iunpp_t pulse,
    int index
);

float iusHLNonParametricPulseGetTime
(
    iunpp_t pulse,
    int index
);

int iusHLNonParametricPulseSetAmplitudeTime
(
    iunpp_t pulse,
    float pulseTime,
    float pulseAmplitude,
    int index
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H
