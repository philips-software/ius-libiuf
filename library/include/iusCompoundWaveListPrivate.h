//
// Created by nlv12901 on 23/01/2019.
//

#ifndef IUSLIBRARY_IUSCOMPOUNDWAVELISTIMP_H
#define IUSLIBRARY_IUSCOMPOUNDWAVELISTIMP_H

#include <hdf5.h>
#include "iusCompoundWaveList.h"

int iusCompoundWaveListSave
(
	iucwl_t list,
	hid_t handle
);

iucwl_t iusCompoundWaveListLoad
(
	hid_t handle
);

#endif //IUSLIBRARY_IUSCOMPOUNDWAVELISTIMP_H

