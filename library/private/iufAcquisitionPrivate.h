//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUFLIBRARY_IUFHLACQUISITIONPRIVATE_H
#define IUFLIBRARY_IUFHLACQUISITIONPRIVATE_H

#include <hdf5.h>
#include <iufAcquisition.h>


int iufAcquisitionSave
(
    iua_t acquisition,
    hid_t handle
);

iua_t iufAcquisitionLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHLACQUISITIONPRIVATE_H
