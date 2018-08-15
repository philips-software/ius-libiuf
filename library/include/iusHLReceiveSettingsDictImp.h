// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGSDICTIMP_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGSDICTIMP_H

#include <iusHDF5.h>
#include <iusHLReceiveSettingsDict.h>

int iusHLReceiveSettingsDictSave
(
    iursd_t dict,
    char *parentPath,
    hid_t handle
);

iursd_t iusHLReceiveSettingsDictLoad
(
    hid_t handle,
    const char *parentPath
);

#endif //IUSLIBRARY_IUSHLRECEIVESETTINGSDICTIMP_H
