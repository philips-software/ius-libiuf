//
// Created by Ruijzendaal on 19/07/2018.
//

#ifndef IUSLIBRARY_IUSHLPATTERNLISTIMP_H
#define IUSLIBRARY_IUSHLPATTERNLISTIMP_H

#include <hdf5.h>
#include <iusPatternList.h>

int iusPatternListSave
(
  iupal_t list,
  hid_t handle
);

iupal_t iusPatternListLoad
(
  hid_t handle
);

#endif //IUSLIBRARY_IUSHLPATTERNLISTIMP_H
