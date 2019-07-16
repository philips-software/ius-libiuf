#ifndef IUFLIBRARY_IUFHLPOSITIONIMP_H
#define IUFLIBRARY_IUFHLPOSITIONIMP_H

#include <hdf5.h>
#include "iufPosition.h"


// ADT
/** \brief Position in 3D in meters */
struct Iuf3DPosition
{
  float x;
  float y;
  float z;
} ;


/** \brief Position in 3D in meters */
struct Iuf2DPosition
{
  float x;
  float z;
} ;


int iuf3DPositionSave
(
    iu3dp_t position,
    hid_t handle
);

iu3dp_t iuf3DPositionLoad
(
    hid_t handle
);

int iuf2DPositionSave
(
    iu2dp_t position,
    hid_t handle
);

iu2dp_t iuf2DPositionLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHLPOSITIONIMP_H
