//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLNONPARAMETRICTRANSMITPULSE_H
#define IUSLIBRARY_IUSHLNONPARAMETRICTRANSMITPULSE_H



#ifndef IUSLIBRARY_IMPLEMENTATION

typedef  struct IusNonParametricTransmitPulse IusNonParametricTransmitPulse;

#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusNonParametricTransmitPulse    * iunptp_t;
#define  IUNPTP_INVALID (iunptp_t) NULL


// operations
iunptp_t iusHLCreateNonParametricPulse
(
    int     numPulseValues                /**< number of points to describe waveform, 0 implies a parametric description only */
);


int iusHLDeleteNonParametricPulse
(
    iunptp_t pulse
);


int iusCompareNonParametricPulse
(
    iunptp_t reference,
    iunptp_t actual
);

int iusHLNonParametricPulseGetNumValues
(
    iunptp_t transmitPulse
);

int iusHLNonParametricPulseSetAmplitudeTime
(
    iunptp_t transmitPulse,
    float pulseTime,
    float pulseAmplitude,
    int index
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICTRANSMITPULSE_H
