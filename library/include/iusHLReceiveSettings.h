// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGS_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGS_H

// ADT
typedef struct IusReceiveSettings IusReceiveSettings;
typedef IusReceiveSettings *iurs_t;
#define  IURS_INVALID (iurs_t) NULL

iurs_t iusHLReceiveSettingsCreate
(
int intParam,
float floatParam
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
int iusHLReceiveSettingsGetIntParam
(
    iurs_t iusReceiveSettings
);

float iusHLReceiveSettingsGetFloatParam
(
    iurs_t iusReceiveSettings
);

// Setters
int iusHLReceiveSettingsSetIntParam
(
    iurs_t iusReceiveSettings,
    int intParam
);

int iusHLReceiveSettingsSetFloatParam
(
    iurs_t iusReceiveSettings,
    float floatParam
);


#endif //IUSLIBRARY_IUSHLRECEIVESETTINGS_H
