// Created by Frank van Heesch on 18/07/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusHDF5.h>
#include <iusUtil.h>
#include <iusError.h>
#include <iusTransmitApodization.h>
#include <iusTransmitApodizationImp.h>


iuta_t iusTransmitApodizationCreate
(
	int numElements
)
{
	int idx;
	iuta_t transmitApodization = IUTA_INVALID;

	if (numElements <= 0) return IUTA_INVALID;

	transmitApodization = calloc(1, sizeof(struct IusTransmitApodization));
	transmitApodization->numElements = numElements;
	transmitApodization->apodization = calloc(numElements, sizeof(float));
	for (idx = 0; idx < numElements; idx++)
	{
		transmitApodization->apodization[idx] = -1;
	}
	return transmitApodization;
}

int iusTransmitApodizationDelete
(
	iuta_t transmitApodization
)
{
	if (transmitApodization == NULL) return IUS_ERR_VALUE;
	if (transmitApodization->apodization != NULL)
	{
		free(transmitApodization->apodization);
	}
	free(transmitApodization);
	return IUS_E_OK;
}

IUS_BOOL iusTransmitApodizationCompare
(
	iuta_t reference,
	iuta_t actual
)
{
	int idx;
	if (reference == actual)
		return IUS_TRUE;
	if (reference == NULL || actual == NULL)
		return IUS_FALSE;
	if (reference->numElements != actual->numElements)
		return IUS_FALSE;

	for (idx = 0; idx < reference->numElements; idx++)
	{
		if (reference->apodization[idx] != actual->apodization[idx])
			return IUS_FALSE;
	}
	return IUS_TRUE;
}

float iusTransmitApodizationGetElement
(
	iuta_t transmitApodization,
	int idx
)
{
	if (transmitApodization == NULL) return  -1;
	if (idx >= transmitApodization->numElements || idx < 0) return -1;
	return transmitApodization->apodization[idx];
}

int iusTransmitApodizationSetElement
(
	iuta_t transmitApodization,
	int idx,
	float attenuation
)
{
	int status = 0;
	if (transmitApodization == NULL) return  -1;
	if (idx >= transmitApodization->numElements || idx < 0) return -1;
	if (attenuation < 0.0f || attenuation > 1.0f) return -1;

	transmitApodization->apodization[idx] = attenuation;

	return status;
}

int iusTransmitApodizationSetApodization
(
	iuta_t transmitApodization,
	float *apodization
)
{
	if (transmitApodization == NULL || apodization == NULL) return  -1;
	if (transmitApodization->numElements <= 0) return -1;

	memcpy(transmitApodization->apodization, apodization, transmitApodization->numElements * sizeof(int));

	return (transmitApodization->apodization == NULL);
}

int iusTransmitApodizationSave
(
	iuta_t transmitApodization,
	hid_t   group_id
)
{
	/* write the /Transducer data */
	herr_t  status;

	status = iusHdf5WriteInt(group_id, "numElements", &(transmitApodization->numElements), 1);
	status |= iusHdf5WriteFloat(group_id, "apodization", transmitApodization->apodization, transmitApodization->numElements, 1);

	return status;
}

iuta_t iusTransmitApodizationLoad
(
	hid_t group_id
)
{
	herr_t status;
	iuta_t transmitApodization;
	int idx = 0;
	int numElements = 0;
	float *apodization;

	status = iusHdf5ReadInt(group_id, "numElements", &numElements);
	if (status != 0 || numElements <= 0) return NULL;

	apodization = (float *)calloc(numElements, sizeof(int));
	status |= iusHdf5ReadFloat(group_id, "apodization", apodization);
	if (status != 0) return NULL;

	transmitApodization = iusTransmitApodizationCreate(numElements);
	for (idx = 0; idx < numElements; idx++)
	{
		status |= iusTransmitApodizationSetElement(transmitApodization, idx, apodization[idx]);
	}
	if (status != 0) return NULL;

	if (apodization != NULL)
	{
		free(apodization);
	}

	return transmitApodization;
}

