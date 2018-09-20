//
// Created by Ruijzendaal on 27/07/2018.
//

#ifndef IUSLIBRARY_IUSHLPOSITIONIMP_H
#define IUSLIBRARY_IUSHLPOSITIONIMP_H

#include <hdf5.h>
#include "iusHLPosition.h"


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


int iusHL3DPositionSave
(
    iu3dp_t position,
    hid_t handle
);

iu3dp_t iusHL3DPositionLoad
(
    hid_t handle
);


int iusHL2DPositionSave
(
    iu2dp_t position,
    hid_t handle
);

iu2dp_t iusHL2DPositionLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHLPOSITIONIMP_H
