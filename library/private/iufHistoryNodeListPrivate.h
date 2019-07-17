#ifndef IUFLIBRARY_IUFHISTORYNODELISTIMP_H
#define IUFLIBRARY_IUFHISTORYNODELISTIMP_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <iufHistoryNodeList.h>

int iufHistoryNodeListSave
(
    iuhnl_t node,
    hid_t handle
);

iuhnl_t iufHistoryNodeListLoad
(
    hid_t handle,
    int numHistoryNodes
);

#endif //IUFLIBRARY_IUFHISTORYNODELISTIMP_H
