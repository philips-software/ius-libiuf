//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICTIMP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICTIMP_H

#include <include/iusReceiveChannelMapDict.h>

herr_t iusReceiveChannelMapDictSave
(
    iurcmd_t dict,
    char *parentPath,
    hid_t handle
);

iurcmd_t iusReceiveChannelMapDictLoad
(
    hid_t handle,
    char *parentPath
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICTIMP_H
