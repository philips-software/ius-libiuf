//
// Created by Ruijzendaal on 27/07/2018.
//

#ifndef IUSLIBRARY_IUSHLPOSITIONIMP_H
#define IUSLIBRARY_IUSHLPOSITIONIMP_H

#include <hdf5.h>
#include "iusPosition.h"


// ADT
/** \brief Position in 3D in meters */
struct Ius3DPosition
{
  float x;
  float y;
  float z;
} ;


/** \brief Position in 3D in meters */
struct Ius2DPosition
{
  float x;
  float z;
} ;


int ius3DPositionSave
(
    iu3dp_t position,
    char *parentPath,
    hid_t handle
);

iu3dp_t ius3DPositionLoad
(
    hid_t handle,
    char *parentPath
);


int ius2DPositionSave
(
    iu2dp_t position,
    char *parentPath,
    hid_t handle
);

iu2dp_t ius2DPositionLoad
(
    hid_t handle,
    char *parentPath
);

#endif //IUSLIBRARY_IUSHLPOSITIONIMP_H
