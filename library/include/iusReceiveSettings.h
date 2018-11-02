// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGS_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGS_H

#include <iusTGC.h>

// ADT
typedef struct IusReceiveSettings IusReceiveSettings;
typedef IusReceiveSettings *iurs_t;
#define  IURS_INVALID (iurs_t) NULL

iurs_t iusReceiveSettingsCreate
(
    float sampleFrequency,
    int numDelays,
    int numSamplesPerLine,
    int numTGCentries
);

int iusReceiveSettingsDelete
(
    iurs_t iusReceiveSettings
);


// operations
int iusReceiveSettingsCompare
(
    iurs_t reference,
    iurs_t actual
);

float iusReceiveSettingsGetSampleFrequency
(
    iurs_t iusReceiveSettings
);

int iusReceiveSettingsGetNumSamplesPerLine
(
    iurs_t iusReceiveSettings
);


int iusReceiveSettingsGetNumTGCentries
(
    iurs_t iusReceiveSettings
);

iutgc_t iusReceiveSettingsGetTGC
(
    iurs_t iusReceiveSettings
);


#endif //IUSLIBRARY_IUSHLRECEIVESETTINGS_H
