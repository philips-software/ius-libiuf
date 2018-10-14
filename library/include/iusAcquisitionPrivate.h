//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLACQUISITIONPRIVATE_H
#define IUSLIBRARY_IUSHLACQUISITIONPRIVATE_H

#include <hdf5.h>
#include <iusAcquisition.h>


int iusAcquisitionSave
(
    iua_t acquisition,
    hid_t handle
);

iua_t iusAcquisitionLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHLACQUISITIONPRIVATE_H
