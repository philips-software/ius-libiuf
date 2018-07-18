
// Created by nlv09165 on 13/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERN_H
#define IUSLIBRARY_IUSHLPATTERN_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct IusPattern IusPattern;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct IusPattern IusPattern;
typedef IusPattern *iupa_t;
#define  IUP_INVALID (iupa_t) NULL

iupa_t iusHLPatternCreate
(
    const char *pLabel,
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
);

int iusHLPatternDelete
(
    iupa_t iusPattern
);


// operations
int iusHLPatternCompare
(
    iupa_t reference,
    iupa_t actual
);

// Getters
const char * iusHLPatternGetLabel
(
    iupa_t iusPattern
);

const char * iusHLPatternGetPulseLabel
(
    iupa_t iusPattern
);

const char * iusHLPatternGetSourceLabel
(
    iupa_t iusPattern
);

const char * iusHLPatternGetChannelMapLabel
(
    iupa_t iusPattern
);

const char * iusHLPatternGetApodizationLabel
(
    iupa_t iusPattern
);

const char * iusHLPatternGetReceivesettingsLabel
(
    iupa_t iusPattern
);

float iusHLPatternGetTimeInFrame
(
    iupa_t iusPattern
);


#endif //IUSLIBRARY_IUSHLPATTERN_H
