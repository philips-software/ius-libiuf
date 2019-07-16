#ifndef IUFLIBRARY_IUFHLRECEIVESETTINGSIMP_H
#define IUFLIBRARY_IUFHLRECEIVESETTINGSIMP_H


#include <hdf5.h>
#include <iufReceiveSettings.h>


int iufReceiveSettingsSave
(
    iurs_t receiveSettings,
    hid_t handle
);

iurs_t iufReceiveSettingsLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHLRECEIVESETTINGSIMP_H
