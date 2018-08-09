// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGS_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGS_H

#include <iusHLTGC.h>

// ADT
typedef struct IusReceiveSettings IusReceiveSettings;
typedef IusReceiveSettings *iurs_t;
#define  IURS_INVALID (iurs_t) NULL

iurs_t iusHLReceiveSettingsCreate
(
    char *pLabel,
    float sampleFrequency,
    int numDelays,
    int numSamplesPerLine,
    int numTGCentries
);

int iusHLReceiveSettingsDelete
(
    iurs_t iusReceiveSettings
);


// operations
int iusHLReceiveSettingsCompare
(
    iurs_t reference,
    iurs_t actual
);

// Getters
char *iusHLReceiveSettingsGetLabel
(
    iurs_t iusReceiveSettings
);

float iusHLReceiveSettingsGetSampleFrequency
(
    iurs_t iusReceiveSettings
);

int iusHLReceiveSettingsGetNumSamplesPerLine
(
    iurs_t iusReceiveSettings
);

int iusHLReceiveSettingsGetNumDelays
(
    iurs_t iusReceiveSettings
);

int iusHLReceiveSettingsGetNumTGCentries
(
    iurs_t iusReceiveSettings
);

float iusHLReceiveSettingsGetStartDelay
(
    iurs_t iusReceiveSettings,
    int index
);

iutgc_t iusHLReceiveSettingsGetTGC
(
    iurs_t iusReceiveSettings
);

// Setters
int iusHLReceiveSettingsSetStartDelay
(
    iurs_t iusReceiveSettings,
    int index,
    float delay
);

#endif //IUSLIBRARY_IUSHLRECEIVESETTINGS_H
