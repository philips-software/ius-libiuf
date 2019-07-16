#ifndef IUFLIBRARY_IUFHLTRANSMITAPODIZATIONIMP_H
#define IUFLIBRARY_IUFHLTRANSMITAPODIZATIONIMP_H
#include <hdf5.h>
#include <iufTransmitApodization.h>

struct IufTransmitApodization
{
	int numElements;
	float *apodization;
};

int iufTransmitApodizationSave
(
	iuta_t  transmitApodization,
	hid_t   handle
);

iuta_t iufTransmitApodizationLoad
(
	hid_t handle
);


#endif
