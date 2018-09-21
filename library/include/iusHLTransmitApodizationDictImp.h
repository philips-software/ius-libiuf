//
// Created by Frank van Heesch on 25/07/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICTIMP_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICTIMP_H
#include <hdf5.h>
#include <iusHLTransmitApodizationDict.h>


int iusHLTransmitApodizationDictSave
(
	iutad_t transmitApodizaionDict,
	hid_t   handle
);

iutad_t iusHLTransmitApodizationDictLoad
(
	hid_t handle
);


#endif
