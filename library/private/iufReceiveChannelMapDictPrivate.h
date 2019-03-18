//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUFLIBRARY_IUFHLRECEIVECHANNELMAPDICTIMP_H
#define IUFLIBRARY_IUFHLRECEIVECHANNELMAPDICTIMP_H

#include <iufReceiveChannelMapDict.h>

herr_t iufReceiveChannelMapDictSave
(
    iurcmd_t dict,
    hid_t handle
);

iurcmd_t iufReceiveChannelMapDictLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHLRECEIVECHANNELMAPDICTIMP_H
