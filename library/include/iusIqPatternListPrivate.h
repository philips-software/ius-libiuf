//
// Created by nlv12901 on 14/01/2019.
//

#ifndef IUSLIBRARY_IUSHLIQPATTERNLISTIMP_H
#define IUSLIBRARY_IUSHLIQPATTERNLISTIMP_H

#include <hdf5.h>
#include <iusIqPatternList.h>

int iusIqPatternListSave
(
  iuiqpal_t list,
  hid_t handle
);

iuiqpal_t iusIqPatternListLoad
(
  hid_t handle
);

#endif //IUSLIBRARY_IUSHLIQPATTERNLISTIMP_H
