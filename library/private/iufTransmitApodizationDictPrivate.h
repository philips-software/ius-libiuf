#ifndef IUFLIBRARY_IUFHLTRANSMITAPODIZATIONDICTIMP_H
#define IUFLIBRARY_IUFHLTRANSMITAPODIZATIONDICTIMP_H
#include <hdf5.h>
#include <iufTransmitApodizationDict.h>


int iufTransmitApodizationDictSave
(
	iutad_t transmitApodizaionDict,
	hid_t   handle
);

iutad_t iufTransmitApodizationDictLoad
(
	hid_t handle
);


#endif
