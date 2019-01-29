// Created by Frank van Heesch on 28/01/2019.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include "iusReceiveApodizationPrivate.h"

iura_t iusReceiveApodizationCreate
(
	int numElements
)
{
	int idx;
	iura_t receiveApodization;

	if (numElements <= 0) return IURA_INVALID;

	receiveApodization = calloc(1, sizeof(struct IusReceiveApodization));
	receiveApodization->numElements = numElements;
	receiveApodization->apodization = calloc(numElements, sizeof(float));
	for (idx = 0; idx < numElements; idx++)
	{
		receiveApodization->apodization[idx] = -1;
	}
	return receiveApodization;
}

int iusReceiveApodizationDelete
(
	iura_t receiveApodization
)
{
	if (receiveApodization == NULL) return IUS_ERR_VALUE;
	if (receiveApodization->apodization != NULL)
	{
		free(receiveApodization->apodization);
	}
	free(receiveApodization);
	return IUS_E_OK;
}

IUS_BOOL iusReceiveApodizationCompare
(
	iura_t reference,
	iura_t actual
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

float iusReceiveApodizationGetElement
(
	iura_t receiveApodization,
	int idx
)
{
	if (receiveApodization == NULL) return  -1;
	if (idx >= receiveApodization->numElements || idx < 0) return -1;
	return receiveApodization->apodization[idx];
}

int iusReceiveApodizationSetElement
(
	iura_t receiveApodization,
	int idx,
	float attenuation
)
{
	int status = 0;
	if (receiveApodization == NULL) return  -1;
	if (idx >= receiveApodization->numElements || idx < 0) return -1;
	if (attenuation < 0.0f || attenuation > 1.0f) return -1;

	receiveApodization->apodization[idx] = attenuation;

	return status;
}

int iusReceiveApodizationSetApodization
(
	iura_t receiveApodization,
	float *apodization
)
{
	if (receiveApodization == NULL || apodization == NULL) return  -1;
	if (receiveApodization->numElements <= 0) return -1;

	memcpy(receiveApodization->apodization, apodization, receiveApodization->numElements * sizeof(int));

	return (receiveApodization->apodization == NULL);
}

int iusReceiveApodizationSave
(
	iura_t receiveApodization,
	hid_t   group_id
)
{
	/* write the /Transducer data */
	herr_t  status;

	status = iusHdf5WriteInt(group_id, 
                             IUS_COMPOUNDFILE_PATH_RECEIVEAPODIZATION_NUMELEMENTS, 
                             &(receiveApodization->numElements), 1);
	status |= iusHdf5WriteFloat(group_id, 
                                IUS_COMPOUNDFILE_PATH_RECEIVEAPODIZATION_APODIZATION, 
                                receiveApodization->apodization, 
                                receiveApodization->numElements);

	return status;
}

iura_t iusReceiveApodizationLoad
(
	hid_t group_id
)
{
	herr_t status;
	iura_t receiveApodization;
	int idx = 0;
	int numElements = 0;
	float *apodization;

	status = iusHdf5ReadInt(group_id, IUS_COMPOUNDFILE_PATH_RECEIVEAPODIZATION_NUMELEMENTS, &numElements);
	if (status != 0 || numElements <= 0) return NULL;

	apodization = (float *)calloc((size_t)numElements, sizeof(int));
	status |= iusHdf5ReadFloat(group_id, IUS_COMPOUNDFILE_PATH_RECEIVEAPODIZATION_APODIZATION, apodization);
	if (status != 0) return NULL;

	receiveApodization = iusReceiveApodizationCreate(numElements);
	for (idx = 0; idx < numElements; idx++)
	{
		status |= iusReceiveApodizationSetElement(receiveApodization, idx, apodization[idx]);
	}
	if (status != 0) return NULL;

	if (apodization != NULL)
	{
		free(apodization);
	}

	return receiveApodization;
}

