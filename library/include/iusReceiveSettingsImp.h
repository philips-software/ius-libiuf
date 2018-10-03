// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGSIMP_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGSIMP_H


#include <hdf5.h>
#include <iusReceiveSettings.h>


int iusReceiveSettingsSave
(
    iurs_t receiveSettings,
    hid_t handle
);

iurs_t iusReceiveSettingsLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHLRECEIVESETTINGSIMP_H
