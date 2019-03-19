//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUFLIBRARY_IUFHLPULSEDICTIMP_H
#define IUFLIBRARY_IUFHLPULSEDICTIMP_H

#include <hdf5.h>
#include <iufPulseDict.h>


int iufPulseDictSave
(
    iupd_t dict,
    hid_t handle
);

iupd_t iufPulseDictLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHLPULSEDICTIMP_H
