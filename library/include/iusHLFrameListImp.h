//
// Created by Ruijzendaal on 19/07/2018.
//

#ifndef IUSLIBRARY_IUSHLFRAMELISTIMP_H
#define IUSLIBRARY_IUSHLFRAMELISTIMP_H

#include <hdf5.h>
#include <iusHLFrameList.h>

int iusHLFrameListSave
(
  iufl_t list,
  const char *parentPath,
  hid_t handle
);

iufl_t iusHLFrameListLoad
(
  hid_t handle,
  const char *parentPath
);

#endif //IUSLIBRARY_IUSHLFRAMELISTIMP_H

