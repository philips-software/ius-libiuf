//
// Created by Ruijzendaal on 04/04/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPATTERN_H
#define IUSLIBRARY_IUSHLTRANSMITPATTERN_H


#ifndef IUSLIBRARY_IMPLEMENTATION

typedef  struct IusTransmitPattern IusTransmitPattern;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusTransmitPattern    * iutpa_t;
#define  IUTPA_INVALID (iutpa_t) NULL



// operations
int iusCompareTransmitPattern
(
    IusTransmitPattern *reference,
    IusTransmitPattern *actual
);


int iusHLDeleteTransmitPattern
(
    iutpa_t pattern
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
