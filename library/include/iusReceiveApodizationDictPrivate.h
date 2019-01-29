//
// Created by Frank van Heesch on 28/01/2019.
//
#ifndef IUSLIBRARY_IUSRECEIVEAPODIZATIONDICTIMP_H
#define IUSLIBRARY_IUSRECEIVEAPODIZATIONDICTIMP_H

#include <hdf5.h>
#include "iusReceiveApodizationDict.h"


int iusReceiveApodizationDictSave
(
	iurad_t receiveApodizaionDict,
	hid_t   handle
);

iurad_t iusReceiveApodizationDictLoad
(
	hid_t handle
);


#endif
