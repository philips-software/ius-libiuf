// Created by nlv12901 on 15/01/2019.
#ifndef IUSLIBRARY_IUSHLDEMODULATIONIMP_H
#define IUSLIBRARY_IUSHLDEMODULATIONIMP_H


#include <hdf5.h>
#include <iusDemodulation.h>


int iusDemodulationSave
(
	iudm_t receiveSettings,
	hid_t handle
);

iudm_t iusDemodulationLoad
(
	hid_t handle
);


#endif //IUSLIBRARY_IUSHLDEMODULATIONIMP_H
