//
// Created by nlv12901 on 23/01/2019.
//

#ifndef IUSLIBRARY_IUSCOMPOUNDFRAMELISTIMP_H
#define IUSLIBRARY_IUSCOMPOUNDFRAMELISTIMP_H

#include <hdf5.h>
#include "iusCompoundFrameList.h"

int iusCompoundFrameListSave
(
	iucfrl_t list,
	hid_t handle
);

iucfrl_t iusCompoundFrameListLoad
(
	hid_t handle
);

#endif //IUSLIBRARY_IUSCOMPOUNDFRAMELISTIMP_H

