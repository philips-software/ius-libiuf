// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H

#include <iusHDF5.h>
#include <iusReceiveSettings.h>

// ADT
typedef struct IusReceiveSettingsDict IusReceiveSettingsDict;
typedef IusReceiveSettingsDict *iursd_t;
#define  IURSD_INVALID (iursd_t) NULL

iursd_t iusReceiveSettingsDictCreate
(
    void
);

int iusReceiveSettingsDictDelete
(
    iursd_t dict
);

int iusReceiveSettingsDictCompare
(
    iursd_t reference,
    iursd_t actual
);

int iusReceiveSettingsDictGetSize
(
    iursd_t dict
);

iurs_t iusReceiveSettingsDictGet
(
    iursd_t dict,
    char * key
);

int iusReceiveSettingsDictSet
(
    iursd_t dict,
    char * key,
    iurs_t member
);

#endif //IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H
