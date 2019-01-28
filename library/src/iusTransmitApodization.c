// Created by Frank van Heesch on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusTransmitApodizationPrivate.h>
#include <math.h>

iuta_t iusTransmitApodizationCreate
(
	int numElements
)
{
	int idx;
	iuta_t transmitApodization;

	if (numElements <= 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "numElements argument should be > 0 but was '%d'", numElements);
		return IUTA_INVALID;
	}

	transmitApodization = calloc(1, sizeof(struct IusTransmitApodization));
	IUS_ERR_ALLOC_NULL_N_RETURN(transmitApodization, IusTransmitApodization, IUTA_INVALID);
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
	IUS_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUS_ERR_VALUE);
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
	IUS_ERR_CHECK_NULL_N_RETURN(transmitApodization, NAN);
	if (idx >= transmitApodization->numElements || idx < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "idx >= 0 && idx < %d but was '%d'", transmitApodization->numElements, idx);
		return NAN;
	}
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
	IUS_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUS_ERR_VALUE);
    if (idx >= transmitApodization->numElements || idx < 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "idx >= 0 && idx < %d but was '%d'", transmitApodization->numElements, idx);
        return IUS_ERR_VALUE;
    }

	if (attenuation < -1.0f || attenuation > 1.0f)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "attentuation >= -1.0 && attentuation <= 1.0 but was '%.1f'", attenuation);
        return IUS_ERR_VALUE;
    }

	transmitApodization->apodization[idx] = attenuation;
	return status;
}

int iusTransmitApodizationSetApodization
(
	iuta_t transmitApodization,
	float *apodization
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUS_ERR_VALUE);
    if (apodization == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument apodization was NULL");
        return IUS_ERR_VALUE;
    }

	if (transmitApodization->numElements <= 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument transmitApodization contains an invalid number of elements: '%d'", transmitApodization->numElements);
        return IUS_ERR_VALUE;
    }

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
	herr_t  status=0;
	IUS_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(group_id == H5I_INVALID_HID, IUS_ERR_VALUE);
	status |= iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS, &(transmitApodization->numElements), 1);
	status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION, transmitApodization->apodization, transmitApodization->numElements);
    if (status != 0)
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "write failed for %s and/or %s",
        IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS,
        IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION);
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

	IUS_ERR_EVAL_N_RETURN(group_id == H5I_INVALID_HID, IUTA_INVALID);
	status = iusHdf5ReadInt(group_id, IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS, &numElements);
	if (status != 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "read failed for %s",
        IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS);
        return IUTA_INVALID;
    }

    if (numElements <= 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_ARG_VALUE, "numElements in file should be > 0, but was %d",
        numElements);
        return IUTA_INVALID;
    }

	apodization = (float *)calloc((size_t)numElements, sizeof(int));
    if (apodization == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for apodization");
        return IUTA_INVALID;
    }

	status = iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION, apodization);
    if (status != 0)
    {
        free(apodization);
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "read failed for %s",
                           IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION);
        return IUTA_INVALID;
    }

    status = 0;
	transmitApodization = iusTransmitApodizationCreate(numElements);
    if (transmitApodization != IUTA_INVALID)
    {
        for (idx = 0; idx < numElements; idx++)
        {
            status |= iusTransmitApodizationSetElement(transmitApodization, idx, apodization[idx]);
        }
    }

	if (status != 0)
    {
        iusTransmitApodizationDelete(transmitApodization);
        transmitApodization = IUTA_INVALID;
    }

    free(apodization);
	return transmitApodization;
}

