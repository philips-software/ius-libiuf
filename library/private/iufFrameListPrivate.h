//
// Created by Ruijzendaal on 19/07/2018.
//

#ifndef IUFLIBRARY_IUFHLFRAMELISTIMP_H
#define IUFLIBRARY_IUFHLFRAMELISTIMP_H

#include <hdf5.h>
#include <iufFrameList.h>

int iufFrameListSave
(
  iufl_t list,
  hid_t handle
);

iufl_t iufFrameListLoad
(
  hid_t handle
);

#endif //IUFLIBRARY_IUFHLFRAMELISTIMP_H

