#ifndef IUFLIBRARY_IUFHLDEMODULATIONDICTIMP_H
#define IUFLIBRARY_IUFHLDEMODULATIONDICTIMP_H

#include <iufHDF5.h>
#include <iufDemodulationDict.h>

int iufDemodulationDictSave
(
	iudmd_t dict,
	hid_t handle
);

iudmd_t iufDemodulationDictLoad
(
	hid_t handle
);

#endif //IUFLIBRARY_IUFHLDEMODULATIONDICTIMP_H

