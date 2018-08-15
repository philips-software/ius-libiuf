// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGSIMP_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGSIMP_H


#include <hdf5.h>
#include <iusHLReceiveSettings.h>


int iusHLReceiveSettingsSave
(
    iurs_t pulse,
    char *parentPath,
    hid_t handle
);

iurs_t iusHLReceiveSettingsLoad
(
    hid_t handle,
    char *parentPath,
    char *label
);


#endif //IUSLIBRARY_IUSHLRECEIVESETTINGSIMP_H
