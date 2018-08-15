// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H

#include <iusHDF5.h>
#include <iusHLReceiveSettings.h>

// ADT
typedef struct IusReceiveSettingsDict IusReceiveSettingsDict;
typedef IusReceiveSettingsDict *iursd_t;
#define  IURSD_INVALID (iursd_t) NULL

iursd_t iusHLReceiveSettingsDictCreate
(
    void
);

int iusHLReceiveSettingsDictDelete
(
    iursd_t dict
);

int iusHLReceiveSettingsDictCompare
(
    iursd_t reference,
    iursd_t actual
);

int iusHLReceiveSettingsDictGetSize
(
    iursd_t dict
);

iurs_t iusHLReceiveSettingsDictGet
(
    iursd_t dict,
    char * key
);

int iusHLReceiveSettingsDictSet
(
    iursd_t dict,
    char * key,
    iurs_t member
);

#endif //IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H
