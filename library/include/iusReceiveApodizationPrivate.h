//
// Created by Frank van Heesch on 20/07/2018.
//
#ifndef IUSLIBRARY_IUSRECEIVEAPODIZATIONIMP_H
#define IUSLIBRARY_IUSRECEIVEAPODIZATIONIMP_H

#include <hdf5.h>
#include "iusReceiveApodization.h"

struct IusReceiveApodization
{
	int numElements;
	float *apodization;
};

int iusReceiveApodizationSave
(
	iura_t  receiveApodization,
	hid_t   handle
);

iura_t iusReceiveApodizationLoad
(
	hid_t handle
);


#endif
