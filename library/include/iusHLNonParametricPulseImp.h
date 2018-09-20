//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLNONPARAMETRICPULSEIMP_H
#define IUSLIBRARY_IUSHLNONPARAMETRICPULSEIMP_H
#include <hdf5.h>
#include <iusHLNonParametricPulse.h>

int iusHLNonParametricPulseSave
(
  iunpp_t pulse,
  hid_t handle
);

iunpp_t iusHLNonParametricPulseLoad
(
  hid_t handle
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICPULSEIMP_H
