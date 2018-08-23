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
    iuf_t frame,
    char *parentPath,
    hid_t handle
);

iuf_t iusFrameLoad
(
    hid_t handle,
    char *parentPath
);

#endif //IUSLIBRARY_IUSHLFRAMEIMP_H
