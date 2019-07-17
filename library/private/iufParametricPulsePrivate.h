#ifndef IUFLIBRARY_IUFHLPARAMETRICPULSEIMP_H
#define IUFLIBRARY_IUFHLPARAMETRICPULSEIMP_H
#include <hdf5.h>
#include <iufParametricPulse.h>

int iufParametricPulseSave
(
    iupp_t pulse,
    hid_t handle
);

iupp_t iufParametricPulseLoad
(
  hid_t handle
);

#endif //IUFLIBRARY_IUFHLPARAMETRICPULSEIMP_H
