//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLFRAMEIMP_H
#define IUSLIBRARY_IUSHLFRAMEIMP_H

#include <iusTypes.h>
#include <iusFrame.h>
#include <hdf5.h>

// ADT
int iusFrameSave
(
    iufr_t frame,
    hid_t handle
);

iufr_t iusFrameLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHLFRAMEIMP_H