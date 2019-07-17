#ifndef IUFLIBRARY_IUFHLNONPARAMETRICPULSEIMP_H
#define IUFLIBRARY_IUFHLNONPARAMETRICPULSEIMP_H
#include <hdf5.h>
#include <iufNonParametricPulse.h>

int iufNonParametricPulseSave
(
  iunpp_t pulse,
  hid_t handle
);

iunpp_t iufNonParametricPulseLoad
(
  hid_t handle
);

#endif //IUFLIBRARY_IUFHLNONPARAMETRICPULSEIMP_H
