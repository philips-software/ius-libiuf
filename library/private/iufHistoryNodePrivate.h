#ifndef IUFLIBRARY_IUFHISTORYNODEIMP_H
#define IUFLIBRARY_IUFHISTORYNODEIMP_H

#include <hdf5.h>
#include <iufHistoryNode.h>

int iufHistoryNodeSave
(
    iuhn_t node,
    hid_t handle
);

int iufHistoryNodeSaveAnyType
(
    iuhn_t node,
    hid_t handle
);

iuhn_t iufHistoryNodeLoad
(
    hid_t handle
);

iuhn_t iufHistoryNodeLoadAnyType
(
    hid_t handle
);

int iufHistoryNodeSetInstanceData
(
    iuhn_t node,
    void *instanceData
);

void *iufHistoryNodeGetInstanceData
(
    iuhn_t node
);

#endif //IUFLIBRARY_IUFHISTORYNODEIMP_H
