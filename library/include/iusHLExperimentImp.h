//
// Created by Ruijzendaal on 28/05/2018.
//

#ifndef IUSLIBRARY_IUSHLEXPERIMENTIMP_H
#define IUSLIBRARY_IUSHLEXPERIMENTIMP_H

#include <hdf5.h>
#include <iusHLExperiment.h>


int iusHLExperimentSave
(
    iue_t experiment,
    char *parentPath,
    hid_t handle
);

iue_t iusHLExperimentLoad
(
    hid_t handle,
    char *parentPath
);

#endif //IUSLIBRARY_IUSHLEXPERIMENTIMP_H