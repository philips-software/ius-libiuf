//
// Created by nlv12901 on 14/01/2019.
//

#ifndef IUSLIBRARY_IUSHLIQPATTERNIMP_H
#define IUSLIBRARY_IUSHLIQPATTERNIMP_H

#include <hdf5.h>
#include <iusIqPattern.h>

int iusIqPatternSave
(
    iuiqpa_t pulse,
    hid_t handle
);

iuiqpa_t iusIqPatternLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHLIQPATTERNIMP_H
