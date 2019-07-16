#include <stdlib.h>
#include <math.h>

#include <iuf.h>
#include <iufFilterPrivate.h>

struct IufFirFilter
{
	int     kernelSize;
	float * pCoefficients;
};

// ADT
iuff_t iufFirFilterCreate
(
	int kernelSize
)
{
	IufFirFilter *firFilter;
    IUF_ERR_EVAL_N_RETURN(kernelSize <0, IUFIRFILTER_INVALID);
	firFilter = (IufFirFilter *)calloc(1, sizeof(IufFirFilter));
    IUF_ERR_ALLOC_NULL_N_RETURN(firFilter, IufFirFilter, IUFIRFILTER_INVALID);

	firFilter->pCoefficients = (float *)calloc(kernelSize, sizeof(float));
	if (firFilter->pCoefficients == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pCoefficients member");
        return IUFIRFILTER_INVALID;
    }

	firFilter->kernelSize = kernelSize;
	return firFilter;
}

int iufFirFilterDelete
(
	iuff_t filter
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(filter, IUF_ERR_VALUE);
    free(filter->pCoefficients);
    free(filter);
	return IUF_E_OK;
}


// operations
int iufFirFilterCompare
(
	iuff_t reference,
	iuff_t actual
)
{
	int i;
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (reference->kernelSize != actual->kernelSize) return IUF_FALSE;
	for (i = 0; i< reference->kernelSize; i++)
	{
		if (IUF_EQUAL_FLOAT(reference->pCoefficients[i], actual->pCoefficients[i]) == IUF_FALSE) return IUF_FALSE;
	}
	return IUF_TRUE;
}

// Getters
int iufFirFilterGetKernelSize
(
	iuff_t filter
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(filter, -1);
	return filter->kernelSize;
}


float iufFirFilterGetCoefficient
(
	iuff_t filter,
	int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(filter, NAN);
    if (index >= filter->kernelSize || index < 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d (kernelSize) but was '%d'", filter->kernelSize, index);
        return NAN;
    }

	return filter->pCoefficients[index];
}

// Setters
int iufFirFilterSetCoefficient
(
	iuff_t filter,
	int index,
	float coefficient
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(filter, IUF_ERR_VALUE);
    if (index >= filter->kernelSize || index < 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d (kernelSize) but was '%d'", filter->kernelSize, index);
        return IUF_ERR_VALUE;
    }
	filter->pCoefficients[index] = coefficient;
	return IUF_E_OK;
}

int iufFirFilterSave
(
	iuff_t filter,
	hid_t handle
)
{
	int status = 0;
    IUF_ERR_CHECK_NULL_N_RETURN(filter, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	status |= iufHdf5WriteInt(handle, IUF_PATH_FIRFILTER_KERNELSIZE, &(filter->kernelSize), 1);

	hsize_t dims[1] = { 1 };
	dims[0] = filter->kernelSize;
	status |= H5LTmake_dataset_float(handle, IUF_PATH_FIRFILTER_COEFFICIENTS, 1, dims, filter->pCoefficients);
	
	return status;
}

iuff_t iufFirFilterLoad
(
	hid_t handle
)
{
	int status = 0;
	int  kernelSize;
	iuff_t firFilter;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUFIRFILTER_INVALID);

	status |= iufHdf5ReadInt(handle, IUF_PATH_FIRFILTER_KERNELSIZE, &(kernelSize));
	if (status < 0)
		return IUFIRFILTER_INVALID;

	firFilter = iufFirFilterCreate(kernelSize);
	status |= H5LTread_dataset_float(handle, IUF_PATH_FIRFILTER_COEFFICIENTS, firFilter->pCoefficients);
	if (status < 0)
		return IUFIRFILTER_INVALID;
	return firFilter;
}


