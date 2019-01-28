// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSHLDEMODULATIONDICTIMP_H
#define IUSLIBRARY_IUSHLDEMODULATIONDICTIMP_H

#include <iusHDF5.h>
#include <iusDemodulationDict.h>

int iusDemodulationDictSave
(
	iudmd_t dict,
	hid_t handle
);

iudmd_t iusDemodulationDictLoad
(
	hid_t handle
);

#endif //IUSLIBRARY_IUSHLDEMODULATIONDICTIMP_H

