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
    IUS_ERR_EVAL_N_RETURN(kernelSize <0, IUFIRFILTER_INVALID);
	firFilter = (IusFirFilter *)calloc(1, sizeof(IusFirFilter));
    IUS_ERR_ALLOC_NULL_N_RETURN(firFilter, IusFirFilter, IUFIRFILTER_INVALID);

	firFilter->pCoefficients = (float *)calloc(kernelSize, sizeof(float));
	if (firFilter->pCoefficients == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pCoefficients member");
        return IUFIRFILTER_INVALID;
    }

	firFilter->kernelSize = kernelSize;
	return firFilter;
}

int iusFirFilterDelete
(
	iuff_t filter
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(filter, IUS_ERR_VALUE);
    free(filter->pCoefficients);
    free(filter);
	return IUS_E_OK;
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
	iuff_t filter
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(filter, -1);
	return filter->kernelSize;
}


float iusFirFilterGetCoefficient
(
	iuff_t filter,
	int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(filter, NAN);
    if (index >= filter->kernelSize || index < 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d (kernelSize) but was '%d'", filter->kernelSize, index);
        return NAN;
    }

	return filter->pCoefficients[index];
}

// Setters
int iusFirFilterSetCoefficient
(
	iuff_t filter,
	int index,
	float coefficient
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(filter, IUS_ERR_VALUE);
    if (index >= filter->kernelSize || index < 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d (kernelSize) but was '%d'", filter->kernelSize, index);
        return IUS_ERR_VALUE;
    }
	filter->pCoefficients[index] = coefficient;
	return IUS_E_OK;
}

int iusFirFilterSave
(
	iuff_t filter,
	hid_t handle
)
{
	int status = 0;
    IUS_ERR_CHECK_NULL_N_RETURN(filter, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	status |= iusHdf5WriteInt(handle, IUS_PATH_FIRFILTER_KERNELSIZE, &(filter->kernelSize), 1);

	hsize_t dims[1] = { 1 };
	dims[0] = filter->kernelSize;
	status |= H5LTmake_dataset_float(handle, IUS_PATH_FIRFILTER_COEFFICIENTS, 1, dims, filter->pCoefficients);
	
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

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUFIRFILTER_INVALID);

	status |= iusHdf5ReadInt(handle, IUS_PATH_FIRFILTER_KERNELSIZE, &(kernelSize));
	if (status < 0)
		return IUFIRFILTER_INVALID;

	firFilter = iusFirFilterCreate(kernelSize);
	status |= H5LTread_dataset_float(handle, IUS_PATH_FIRFILTER_COEFFICIENTS, firFilter->pCoefficients);
	if (status < 0)
		return IUFIRFILTER_INVALID;
	return firFilter;
}


