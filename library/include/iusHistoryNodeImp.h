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

iuhn_t iusHistoryNodeLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHISTORYNODEIMP_H
