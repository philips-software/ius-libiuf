//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICPULSEIMP_H
#define IUSLIBRARY_IUSHLPARAMETRICPULSEIMP_H
#include <hdf5.h>
#include <iusParametricPulse.h>

int iusParametricPulseSave
(
    iupp_t pulse,
    hid_t handle
);

iupp_t iusParametricPulseLoad
(
  hid_t handle
);

#endif //IUSLIBRARY_IUSHLPARAMETRICPULSEIMP_H
