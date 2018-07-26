//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATION_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATION_H

#include <iusTypes.h>

// ADT
struct IusTransmitApodization;
typedef  struct IusTransmitApodization IusTransmitApodization;
typedef  IusTransmitApodization    * iuta_t;
#define  IUTA_INVALID (iuta_t) NULL

iuta_t  iusHLTransmitApodizationCreate
(
	int numChannels
);

int iusHLTransmitApodizationDelete
(
	iuta_t transmitApodization
);

IUS_BOOL  iusHLTransmitApodizationCompare
(
	iuta_t reference,
	iuta_t actual
);

float iusHLTransmitApodizationGetElement
(
	iuta_t transmitApodization,
	int idx
);

int iusHLTransmitApodizationSetElement
(
	iuta_t transmitApodization,
	int idx,
	float attenuation
);

int iusHLTransmitApodizationSetApodization
(
	iuta_t transmitApodization,
	float *apodization
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H

