//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICTIMP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICTIMP_H

#include <include/iusHLReceiveChannelMapDict.h>

herr_t iusHLReceiveChannelMapDictSave
(
    iurcmd_t dict,
    char *parentPath,
    hid_t handle
);

iurcmd_t iusHLReceiveChannelMapDictLoad
(
    hid_t handle,
    char *parentPath
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICTIMP_H
