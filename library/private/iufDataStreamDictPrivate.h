// Created by nlv12901 on 26/03/2018.
#ifndef IUFLIBRARY_IUFDATASTREAMDICTIMP_H
#define IUFLIBRARY_IUFDATASTREAMDICTIMP_H

#include <iufHDF5.h>
#include <iufDataStreamDict.h>

int iufDataStreamDictSave
(
	iudsd_t dict,
	hid_t handle
);

iudmd_t iufDataStreamDictLoad
(
	hid_t handle
);

#endif //IUFLIBRARY_IUFDATASTREAMDICTIMP_H

