//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLNONPARAMETRICPULSEIMP_H
#define IUSLIBRARY_IUSHLNONPARAMETRICPULSEIMP_H
#include <hdf5.h>
#include <iusNonParametricPulse.h>

int iusNonParametricPulseSave
(
  iunpp_t pulse,
  hid_t handle
);

iunpp_t iusNonParametricPulseLoad
(
  hid_t handle
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICPULSEIMP_H
