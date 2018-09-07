
// Created by nlv09165 on 13/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERN_H
#define IUSLIBRARY_IUSHLPATTERN_H

// ADT
typedef struct IusPattern IusPattern;
typedef IusPattern *iupa_t;
#define  IUPA_INVALID (iupa_t) NULL

iupa_t iusPatternCreate
(
    const char *pLabel,
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
);

int iusPatternDelete
(
    iupa_t iusPattern
);


// operations
int iusPatternCompare
(
    iupa_t reference,
    iupa_t actual
);

// Getters
const char * iusPatternGetLabel
(
    iupa_t iusPattern
);

const char * iusPatternGetPulseLabel
(
    iupa_t iusPattern
);

const char * iusPatternGetSourceLabel
(
    iupa_t iusPattern
);

const char * iusPatternGetChannelMapLabel
(
    iupa_t iusPattern
);

const char * iusPatternGetApodizationLabel
(
    iupa_t iusPattern
);

const char * iusPatternGetReceivesettingsLabel
(
    iupa_t iusPattern
);

float iusPatternGetTimeInFrame
(
    iupa_t iusPattern
);


#endif //IUSLIBRARY_IUSHLPATTERN_H
