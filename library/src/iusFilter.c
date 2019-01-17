//
// Created by nlv12901 on 16/01/2019.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusFilterPrivate.h>

struct IusFirFilter
{
	int     kernelSize;
	float * pCoefficients;
};

// ADT
iuff_t iusFirFilterCreate
(
	int kernelSize
)
{
	IusFirFilter *firFilter;

	if (kernelSize < 0) return IUFIRFILTER_INVALID;

	firFilter = (IusFirFilter *)calloc(1, sizeof(IusFirFilter));
	if (firFilter == NULL) return NULL;

	firFilter->pCoefficients = (float *)calloc(kernelSize, sizeof(float));
	if (firFilter->pCoefficients == NULL) return IUFIRFILTER_INVALID;

	firFilter->kernelSize = kernelSize;
	return firFilter;
}

int iusFirFilterDelete
(
	iuff_t iusFirFilter
)
{
	int status = IUS_ERR_VALUE;
	if (iusFirFilter != NULL)
	{
		free(iusFirFilter->pCoefficients);
		free(iusFirFilter);
		status = IUS_E_OK;
	}
	return status;
}


// operations
int iusFirFilterCompare
(
	iuff_t reference,
	iuff_t actual
)
{
	int i;
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (reference->kernelSize != actual->kernelSize) return IUS_FALSE;
	for (i = 0; i< reference->kernelSize; i++)
	{
		if (IUS_EQUAL_FLOAT(reference->pCoefficients[i], actual->pCoefficients[i]) == IUS_FALSE) return IUS_FALSE;
	}
	return IUS_TRUE;
}

// Getters
int iusFirFilterGetKernelSize
(
	iuff_t iusFirFilter
)
{
	if (iusFirFilter == NULL)
		return -1;
	return iusFirFilter->kernelSize;
}


float iusFirFilterGetCoefficient
(
	iuff_t iusFirFilter,
	int index
)
{
	if (iusFirFilter == NULL) return NAN;
	if (index < 0 || index >= iusFirFilter->kernelSize)
		return NAN;
	return iusFirFilter->pCoefficients[index];
}

// Setters
int iusFirFilterSetCoefficient
(
	iuff_t iusFirFilter,
	int index,
	float coefficient
)
{
	if (iusFirFilter == NULL) return IUS_ERR_VALUE;
	if (index < 0 || index >= iusFirFilter->kernelSize)
		return IUS_ERR_VALUE;
	iusFirFilter->pCoefficients[index] = coefficient;
	return IUS_E_OK;
}

int iusFirFilterSave
(
	iuff_t iusFirFilter,
	hid_t handle
)
{
	int status = 0;
	if (iusFirFilter == NULL)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

	status |= iusHdf5WriteInt(handle, IUS_PATH_FIRFILTER_KERNELSIZE, &(iusFirFilter->kernelSize), 1);

	hsize_t dims[1] = { 1 };
	dims[0] = iusFirFilter->kernelSize;
	status |= H5LTmake_dataset_float(handle, IUS_PATH_FIRFILTER_COEFFICIENTS, 1, dims, iusFirFilter->pCoefficients);
	
	return status;
}

iuff_t iusFirFilterLoad
(
	hid_t handle
)
{
	int status = 0;
	int  kernelSize;
	iuff_t firFilter;

	if (handle == H5I_INVALID_HID)
		return IUFIRFILTER_INVALID;

	status |= iusHdf5ReadInt(handle, IUS_PATH_FIRFILTER_KERNELSIZE, &(kernelSize));
	if (status < 0)
		return IUFIRFILTER_INVALID;

	firFilter = iusFirFilterCreate(kernelSize);
	status |= H5LTread_dataset_float(handle, IUS_PATH_FIRFILTER_COEFFICIENTS, firFilter->pCoefficients);
	if (status < 0)
		return IUFIRFILTER_INVALID;
	return firFilter;
}


