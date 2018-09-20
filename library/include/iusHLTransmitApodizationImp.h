//
// Created by Frank van Heesch on 20/07/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATIONIMP_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATIONIMP_H
#include <hdf5.h>
#include <iusHLTransmitApodization.h>

struct IusTransmitApodization
{
	int numElements;
	float *apodization;
};

int iusHLTransmitApodizationSave
(
	iuta_t  transmitApodization,
	hid_t   handle
);

iuta_t iusHLTransmitApodizationLoad
(
	hid_t handle
);


#endif
