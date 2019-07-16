#ifndef IUFLIBRARY_IUFFIRFILTERIMP_H
#define IUFLIBRARY_IUFFIRFILTERIMP_H

#include <iufFilter.h>
#include <hdf5.h>
#include <hdf5_hl.h>

int iufFirFilterSave
(
	iuff_t filter,
	hid_t handle
);

iuff_t iufFirFilterLoad
(
	hid_t handle
);


#endif //IUFLIBRARY_IUFIFIRFILTERMP_H
