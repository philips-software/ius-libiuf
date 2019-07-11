#ifndef IUFLIBRARY_IUFHLRECEIVESETTINGSDICTIMP_H
#define IUFLIBRARY_IUFHLRECEIVESETTINGSDICTIMP_H

#include <iufHDF5.h>
#include <iufReceiveSettingsDict.h>

int iufReceiveSettingsDictSave
(
    iursd_t dict,
    hid_t handle
);

iursd_t iufReceiveSettingsDictLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHLRECEIVESETTINGSDICTIMP_H
