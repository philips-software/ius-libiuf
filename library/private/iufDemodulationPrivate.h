#ifndef IUFLIBRARY_IUFHLDEMODULATIONIMP_H
#define IUFLIBRARY_IUFHLDEMODULATIONIMP_H


#include <hdf5.h>
#include <iufDemodulation.h>


int iufDemodulationSave
(
	iudm_t demodulation,
	hid_t handle
);

iudm_t iufDemodulationLoad
(
	hid_t handle
);


#endif //IUFLIBRARY_IUFHLDEMODULATIONIMP_H
