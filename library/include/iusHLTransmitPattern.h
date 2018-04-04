//
// Created by Ruijzendaal on 04/04/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPATTERN_H
#define IUSLIBRARY_IUSHLTRANSMITPATTERN_H


#ifndef IUSLIBRARY_IMPLEMENTATION

typedef  struct IusTransmitPattern IusTransmitPattern;
typedef  struct IusTransmitPatternList IusTransmitPatternList;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusTransmitPattern    * iutpa_t;
typedef  IusTransmitPatternList   * iutpal_t;
#define  IUTPA_INVALID (iutpa_t) NULL
#define  IUTPAL_INVALID (iutpal_t) NULL



// operations
iutpal_t iusHLCreateTransmitPatternList
(
    int numTransmitPulses
);

int iusHLTransmitPatternListSet
(
    iutpal_t list,
    float time,
    int pulseIndex,
    int patterniIndex
);

int iusHLTransmitPatternListGetSize
(
    iutpal_t list
);

int iusCompareTransmitPatternList
(
    IusTransmitPatternList *reference,
    IusTransmitPatternList *actual
);

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

int iusHLTransmitPatternGetIndex
(
    iutpa_t pattern
);

// set
int iusHLTransmitPatternSetTime
(
    iutpa_t pattern,
    float time
);

int iusHLTransmitPatternSetIndex
(
    iutpa_t pattern,
    int pulseIndex
);

int iusHLTransmitPatternSet
(
    iutpa_t pattern,
    float time,
    int pulseIndex
);

iutpa_t iusHLTransmitPatternListGet
(
    IusTransmitPatternList* list,
    int patternIndex
);
#endif //IUSLIBRARY_IUSHLTRANSMITPATTERN_H
