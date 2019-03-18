// Created by Frank van Heesch on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufTransmitApodizationPrivate.h>
#include <math.h>

iuta_t iufTransmitApodizationCreate
(
    float *apodization,
	int numElements
)
{
	int idx;
	iuta_t transmitApodization;
	int status = IUF_E_OK;

    IUF_ERR_CHECK_NULL_N_RETURN(apodization, IUTA_INVALID);
	if (numElements <= 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "numElements argument should be > 0 but was '%d'", numElements);
		return IUTA_INVALID;
	}

	transmitApodization = calloc(1, sizeof(struct IufTransmitApodization));
	IUF_ERR_ALLOC_NULL_N_RETURN(transmitApodization, IufTransmitApodization, IUTA_INVALID);
	transmitApodization->numElements = numElements;
	transmitApodization->apodization = calloc(numElements, sizeof(float));

    for (idx = 0; idx < transmitApodization->numElements; idx++)
    {
        status = iufTransmitApodizationSetElement(transmitApodization,idx,apodization[idx]);
        if ( status == IUF_ERR_VALUE )
        {
            iufTransmitApodizationDelete(transmitApodization);
            return IUTA_INVALID;
        }
    }

	return transmitApodization;
}

int iufTransmitApodizationDelete
(
	iuta_t transmitApodization
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUF_ERR_VALUE);
	if (transmitApodization->apodization != NULL)
	{
		free(transmitApodization->apodization);
	}
	free(transmitApodization);
	return IUF_E_OK;
}

IUF_BOOL iufTransmitApodizationCompare
(
	iuta_t reference,
	iuta_t actual
)
{
	int idx;
	if (reference == actual)
		return IUF_TRUE;
	if (reference == NULL || actual == NULL)
		return IUF_FALSE;
	if (reference->numElements != actual->numElements)
		return IUF_FALSE;

	for (idx = 0; idx < reference->numElements; idx++)
	{
		if (reference->apodization[idx] != actual->apodization[idx])
			return IUF_FALSE;
	}
	return IUF_TRUE;
}

float iufTransmitApodizationGetElement
(
	iuta_t transmitApodization,
	int idx
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transmitApodization, NAN);
	if (idx >= transmitApodization->numElements || idx < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "idx >= 0 && idx < %d but was '%d'", transmitApodization->numElements, idx);
		return NAN;
	}
	return transmitApodization->apodization[idx];
}

int iufTransmitApodizationGetNumElements
(
    iuta_t transmitApodization
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transmitApodization, -1);
    return transmitApodization->numElements;
}

int iufTransmitApodizationSetElement
(
	iuta_t transmitApodization,
	int idx,
	float attenuation
)
{
	int status = 0;
	IUF_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUF_ERR_VALUE);
    if (idx >= transmitApodization->numElements || idx < 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "idx >= 0 && idx < %d but was '%d'", transmitApodization->numElements, idx);
        return IUF_ERR_VALUE;
    }

	if (attenuation < -1.0f || attenuation > 1.0f)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "attentuation >= -1.0 && attentuation <= 1.0 but was '%.1f'", attenuation);
        return IUF_ERR_VALUE;
    }

	transmitApodization->apodization[idx] = attenuation;
	return status;
}

int iufTransmitApodizationSetApodization
(
	iuta_t transmitApodization,
	float *apodization
)
{
    int idx;
    int status=0;
	IUF_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUF_ERR_VALUE);
    if (apodization == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "argument apodization was NULL");
        return IUF_ERR_VALUE;
    }

	if (transmitApodization->numElements <= 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "argument transmitApodization contains an invalid number of elements: '%d'", transmitApodization->numElements);
        return IUF_ERR_VALUE;
    }

    for (idx = 0; idx < transmitApodization->numElements; idx++)
    {
        status |= iufTransmitApodizationSetElement(transmitApodization,idx,apodization[idx]);
    }

	return status;
}

int iufTransmitApodizationSave
(
	iuta_t transmitApodization,
	hid_t   group_id
)
{
	/* write the /Transducer data */
	herr_t  status=0;
	IUF_ERR_CHECK_NULL_N_RETURN(transmitApodization, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(group_id == H5I_INVALID_HID, IUF_ERR_VALUE);
	status |= iufHdf5WriteInt(group_id, IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS, &(transmitApodization->numElements), 1);
	status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION, transmitApodization->apodization, transmitApodization->numElements);
    if (status != 0)
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "write failed for %s and/or %s",
        IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS,
        IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION);
	return status;
}

iuta_t iufTransmitApodizationLoad
(
	hid_t group_id
)
{
	herr_t status;
	iuta_t transmitApodization;
	int numElements = 0;
	float *apodization;

	IUF_ERR_EVAL_N_RETURN(group_id == H5I_INVALID_HID, IUTA_INVALID);
	status = iufHdf5ReadInt(group_id, IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS, &numElements);
	if (status != 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "read failed for %s",
        IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS);
        return IUTA_INVALID;
    }

    if (numElements <= 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_ARG_VALUE, "numElements in file should be > 0, but was %d",
        numElements);
        return IUTA_INVALID;
    }

	apodization = (float *)calloc((size_t)numElements, sizeof(int));
    if (apodization == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for apodization");
        return IUTA_INVALID;
    }

	status = iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION, apodization);
    if (status != 0)
    {
        free(apodization);
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "read failed for %s",
                           IUF_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION);
        return IUTA_INVALID;
    }

	transmitApodization = iufTransmitApodizationCreate(apodization, numElements);
    free(apodization);
	return transmitApodization;
}

