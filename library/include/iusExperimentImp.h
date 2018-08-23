//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLEXPERIMENTIMP_H
#define IUSLIBRARY_IUSHLEXPERIMENTIMP_H

#include <hdf5.h>
#include <iusExperiment.h>


int iusExperimentSave
(
    iue_t experiment,
    hid_t handle
);

iue_t iusExperimentLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHLEXPERIMENTIMP_H
