//
// Created by nlv12901 on 14/01/2019.
//

#ifndef IUFLIBRARY_IUFHLIQPATTERNIMP_H
#define IUFLIBRARY_IUFHLIQPATTERNIMP_H

#include <hdf5.h>
#include <iufIqPattern.h>

int iufIqPatternSave
(
    iuiqpa_t pulse,
    hid_t handle
);

iuiqpa_t iufIqPatternLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHLIQPATTERNIMP_H
