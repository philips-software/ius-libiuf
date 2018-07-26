//
// Created by Ruijzendaal on 19/07/2018.
//

#ifndef IUSLIBRARY_IUSHLPATTERNLISTIMP_H
#define IUSLIBRARY_IUSHLPATTERNLISTIMP_H

#include <hdf5.h>
#include <iusHLPatternList.h>

int iusHLPatternListSave
(
  iupal_t list,
  const char *parentPath,
  hid_t handle
);

iupal_t iusHLPatternListLoad
(
  hid_t handle,
  const char *parentPath
);

#endif //IUSLIBRARY_IUSHLPATTERNLISTIMP_H
