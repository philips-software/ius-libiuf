// Created by nlv09165 on 13/09/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODEIMP_H
#define IUSLIBRARY_IUSHISTORYNODEIMP_H

#include <hdf5.h>
#include <iusHistoryNode.h>

int iusHistoryNodeSave
(
    iuhn_t node,
    hid_t handle
);

int iusHistoryNodeSaveAnyType
(
    iuhn_t node,
    hid_t handle
);

iuhn_t iusHistoryNodeLoad
(
    hid_t handle
);

iuhn_t iusHistoryNodeLoadAnyType
(
    hid_t handle
);

int iusHistoryNodeSetInstanceData
(
    iuhn_t node,
    void *instanceData
);

void *iusHistoryNodeGetInstanceData
(
    iuhn_t node
);

#endif //IUSLIBRARY_IUSHISTORYNODEIMP_H
