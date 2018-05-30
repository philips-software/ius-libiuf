//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICPULSEIMP_H
#define IUSLIBRARY_IUSHLPARAMETRICPULSEIMP_H
#include <hdf5.h>
#include <iusHLParametricPulse.h>

int iusHLParametricPulseSave
(
    iupp_t pulse,
    char *parentPath,
    hid_t handle
);

iupp_t iusHLParametricPulseLoad
(
  hid_t handle,
  char *parentPath
);

#endif //IUSLIBRARY_IUSHLPARAMETRICPULSEIMP_H
