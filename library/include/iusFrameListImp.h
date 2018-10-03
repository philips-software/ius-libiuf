//
// Created by Ruijzendaal on 19/07/2018.
//

#ifndef IUSLIBRARY_IUSHLFRAMELISTIMP_H
#define IUSLIBRARY_IUSHLFRAMELISTIMP_H

#include <hdf5.h>
#include <iusFrameList.h>

int iusFrameListSave
(
  iufl_t list,
  hid_t handle
);

iufl_t iusFrameListLoad
(
  hid_t handle
);

#endif //IUSLIBRARY_IUSHLFRAMELISTIMP_H

