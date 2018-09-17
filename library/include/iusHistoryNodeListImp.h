// Created by nlv09165 on 13/09/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODELISTIMP_H
#define IUSLIBRARY_IUSHISTORYNODELISTIMP_H

#include <hdf5.h>
#include <iusHistoryNodeList.h>

int iusHistoryNodeListSave
(
    iuhnl_t node,
    hid_t handle
);

iuhnl_t iusHistoryNodeListLoad
(
    hid_t handle,
    int numHistoryNodes
);

#endif //IUSLIBRARY_IUSHISTORYNODELISTIMP_H
