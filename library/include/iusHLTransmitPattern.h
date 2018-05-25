//
// Created by Ruijzendaal on 04/04/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSMITPATTERN_H
#define IUSLIBRARY_IUSHLTRANSMITPATTERN_H

// ADT
typedef  struct IusTransmitPattern IusTransmitPattern;
typedef  IusTransmitPattern    * iutpa_t;
#define  IUTPA_INVALID (iutpa_t) NULL

int iusHLTransmitPatternDelete
(
    iutpa_t pattern
);


// operations
int iusHLTransmitPatternCompare
(
    iutpa_t reference,
    iutpa_t actual
);

// getters
float iusHLTransmitPatternGetTime
(
    iutpa_t pattern
);

int iusHLTransmitPatternGetSourceIndex
(
    iutpa_t pattern
);

int iusHLTransmitPatternGetPulseIndex
(
    iutpa_t pattern
);

// set
int iusHLTransmitPatternSet
(
    iutpa_t pattern,
    float time,
    int sourceIndex,
    int pulseIndex
);

#endif //IUSLIBRARY_IUSHLTRANSMITPATTERN_H
