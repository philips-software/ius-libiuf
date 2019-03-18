//
// Created by nlv12901 on 14/01/2019.
//

#ifndef IUFLIBRARY_IUFHLIQPATTERNLISTIMP_H
#define IUFLIBRARY_IUFHLIQPATTERNLISTIMP_H

#include <hdf5.h>
#include <iufIqPatternList.h>

int iufIqPatternListSave
(
  iuiqpal_t list,
  hid_t handle
);

iuiqpal_t iufIqPatternListLoad
(
  hid_t handle
);

#endif //IUFLIBRARY_IUFHLIQPATTERNLISTIMP_H
