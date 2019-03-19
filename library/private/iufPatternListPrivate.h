//
// Created by Ruijzendaal on 19/07/2018.
//

#ifndef IUFLIBRARY_IUFHLPATTERNLISTIMP_H
#define IUFLIBRARY_IUFHLPATTERNLISTIMP_H

#include <hdf5.h>
#include <iufPatternList.h>

int iufPatternListSave
(
  iupal_t list,
  hid_t handle
);

iupal_t iufPatternListLoad
(
  hid_t handle
);

#endif //IUFLIBRARY_IUFHLPATTERNLISTIMP_H
