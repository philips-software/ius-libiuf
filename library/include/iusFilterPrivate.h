//
// Created by nlv12901 on 16/01/2019.
//

#ifndef IUSLIBRARY_IUSFIRFILTERIMP_H
#define IUSLIBRARY_IUSFIRFILTERIMP_H

#include <iusFilter.h>
#include <hdf5.h>
#include <hdf5_hl.h>

int iusFirFilterSave
(
	iuff_t filter,
	hid_t handle
);

iuff_t iusFirFilterLoad
(
	hid_t handle
);


#endif //IUSLIBRARY_IUSIFIRFILTERMP_H
