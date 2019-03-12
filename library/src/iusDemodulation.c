//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ius.h>
#include <iusFilterPrivate.h>
#include <iusIqFileStructure.h>
#include <iusDemodulationPrivate.h>
#include <iusTGCPrivate.h>

struct IusDemodulation
{
	IusDemodulationMethod method;   /**< How the downsampling has been performed */
	int      downSampleFactor;      /**< The reduction factor between original sampling and the data rate (i.e. sampleFrequency) */
	iuff_t   preFilter;             /**< The filter that has been used before downSampling was applied */
	iutgc_t  TGC;                   /**< TimeGainControl points (time,gain) */
	float    sampleFrequency;       /**< The sampling frequency that was used */
	float    centerFrequency;       /**< The center frequency that was used */
	int      numSamplesPerLine;     /**< length of an acquisition line */
};

// ADT
static iudm_t iusDemodulationCreateWithoutTGCandFilter
(
	IusDemodulationMethod method,
	float sampleFrequency,
	float centerFrequency,
	int numSamplesPerLine
)
{
	iudm_t created = calloc(1, sizeof(IusDemodulation));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusDemodulation, IUDM_INVALID);
    created->sampleFrequency = sampleFrequency;
    created->centerFrequency = centerFrequency;
	created->numSamplesPerLine = numSamplesPerLine;
	created->method = method;
	return created;
}

iudm_t iusDemodulationCreate
(
	IusDemodulationMethod method,
	float sampleFrequency,
	float centerFrequency,
	int numSamplesPerLine,
	int numTGCentries,
	int filterKernelSize
)
{
    if( centerFrequency <= 0.0f )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "centerFrequency argument should be > 0.0, but was: '%f'", centerFrequency);
        return IUDM_INVALID;
    }

    if( sampleFrequency <= 0.0f )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "sampleFrequency argument should be > 0.0, but was: '%f'", sampleFrequency);
        return IUDM_INVALID;
    }

    if( numSamplesPerLine < 0 )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "numSamplesPerLine argument should be >= 0, but was: '%d'", numSamplesPerLine);
        return IUDM_INVALID;
    }

    if( numTGCentries <= 0 )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "numTGCentries argument should be > 0, but was: '%d'", numTGCentries);
        return IUDM_INVALID;
    }

    if( filterKernelSize <= 0 )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "filterKernelSize argument should be > 0, but was: '%d'", numTGCentries);
        return IUDM_INVALID;
    }
    iudm_t created = iusDemodulationCreateWithoutTGCandFilter(method, sampleFrequency, centerFrequency, numSamplesPerLine);
	created->TGC = iusTGCCreate(numTGCentries);
	created->preFilter = iusFirFilterCreate(filterKernelSize);

	return created;
}

int iusDemodulationDelete
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, IUS_ERR_VALUE);
    iusTGCDelete(demodulation->TGC);
    iusFirFilterDelete(demodulation->preFilter);
    free(demodulation);
	return IUS_E_OK;
}


// operations
int iusDemodulationCompare
(
	iudm_t reference,
	iudm_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (IUS_EQUAL_FLOAT(reference->sampleFrequency, actual->sampleFrequency) == IUS_FALSE) return IUS_FALSE;
	if (IUS_EQUAL_FLOAT(reference->centerFrequency, actual->centerFrequency) == IUS_FALSE) return IUS_FALSE;
	if (reference->numSamplesPerLine != actual->numSamplesPerLine) return IUS_FALSE;
	if (iusFirFilterCompare(reference->preFilter, actual->preFilter) == IUS_FALSE) return IUS_FALSE;
	return iusTGCCompare(reference->TGC, actual->TGC);
}

// Getters
iutgc_t iusDemodulationGetTGC
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, IUTGC_INVALID);
    return demodulation->TGC;
}

// Getters
iuff_t iusDemodulationGetPreFilter
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, IUFIRFILTER_INVALID);
	return demodulation->preFilter;
}

float iusDemodulationGetCenterFrequency
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, NAN);
	return demodulation->centerFrequency;
}

float iusDemodulationGetSampleFrequency
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, NAN);
	return demodulation->sampleFrequency;
}

int iusDemodulationGetNumSamplesPerLine
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, -1);
	return demodulation->numSamplesPerLine;
}

int iusDemodulationGetNumTGCentries
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, -1);
	return iusTGCGetNumValues(demodulation->TGC);
}

int iusDemodulationGetPreFilterKernelSize
(
	iudm_t demodulation
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, -1);
	return iusFirFilterGetKernelSize(demodulation->preFilter);
}

//setters
int iusDemodulationSetTGC
(
	iudm_t demodulation,
    iutgc_t tgc
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(tgc, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusTGCGetNumValues(tgc) != iusTGCGetNumValues(demodulation->TGC), IUS_ERR_VALUE);

    for (int idx=0; idx<iusTGCGetNumValues(tgc); idx++)
    {
        float gain = iusTGCGetGain(tgc, idx);
        float time = iusTGCGetTime(tgc, idx);
        iusTGCSet(demodulation->TGC, idx, time, gain);
    }

    return IUS_E_OK;
}

//setters
int iusDemodulationSetPreFilter
(
	iudm_t demodulation,
    iuff_t preFilter
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(preFilter, IUS_ERR_VALUE);
    
    int kernelSize = iusFirFilterGetKernelSize(preFilter);
    iuff_t dmPreFilter = iusDemodulationGetPreFilter(demodulation);
    IUS_ERR_EVAL_N_RETURN(kernelSize <= 0, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(kernelSize != iusFirFilterGetKernelSize(dmPreFilter), IUS_ERR_VALUE);

    for (int idx=0; idx<kernelSize; idx++)
    {
        float coefficient = iusFirFilterGetCoefficient(preFilter, idx);
        iusFirFilterSetCoefficient(dmPreFilter, idx, coefficient); 
    }

    return IUS_E_OK;
}
int iusDemodulationSave
(
	iudm_t demodulation,
	hid_t handle
)
{
	int status = 0;
	int method = (int)demodulation->method;

    IUS_ERR_CHECK_NULL_N_RETURN(demodulation, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	status |= iusHdf5WriteFloat(handle, IUS_PATH_DEMODULATION_SAMPLEFREQUENCY, &(demodulation->sampleFrequency), 1);
	status |= iusHdf5WriteFloat(handle, IUS_PATH_DEMODULATION_CENTERFREQUENCY, &(demodulation->centerFrequency), 1);
	status |= iusHdf5WriteInt(handle, IUS_PATH_DEMODULATION_NUMSAMPLESPERLINE, &(demodulation->numSamplesPerLine), 1);
	status |= iusHdf5WriteInt(handle, IUS_PATH_DEMODULATION_METHOD, &method, 1);
    if (status != 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "write failed for %s, %s, %s, and/or %s",
							IUS_PATH_DEMODULATION_SAMPLEFREQUENCY,
							IUS_PATH_DEMODULATION_CENTERFREQUENCY,
							IUS_PATH_DEMODULATION_NUMSAMPLESPERLINE,
                           IUS_PATH_DEMODULATION_METHOD);
        return IUS_ERR_VALUE;
    }

	hid_t tgc_id = H5Gcreate(handle, IUS_PATH_DEMODULATION_TGC, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusTGCSave(demodulation->TGC, tgc_id);
	H5Gclose(tgc_id);

	hid_t filter_id = H5Gcreate(handle, IUS_PATH_DEMODULATION_PREFILTER, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusFirFilterSave(demodulation->preFilter, filter_id);
	H5Gclose(filter_id);

	return status;
}

iudm_t iusDemodulationLoad
(
	hid_t handle
)
{
	float sampleFrequency;
	float centerFrequency;
	int numSamplesPerLine;
	int status = 0;
	int method = -1;

	iutgc_t tgc;
	iuff_t filter;
	iudm_t iusDemodulation;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDM_INVALID);
	status |= iusHdf5ReadFloat(handle, IUS_PATH_DEMODULATION_SAMPLEFREQUENCY, &sampleFrequency);
	status |= iusHdf5ReadFloat(handle, IUS_PATH_DEMODULATION_CENTERFREQUENCY, &centerFrequency);
	status |= iusHdf5ReadInt(handle, IUS_PATH_DEMODULATION_NUMSAMPLESPERLINE, &numSamplesPerLine);
	status |= iusHdf5ReadInt(handle, IUS_PATH_DEMODULATION_METHOD, &method);
    if (status != 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "read failed for %s, %s %s, or %s",
                           IUS_PATH_DEMODULATION_SAMPLEFREQUENCY,
                           IUS_PATH_DEMODULATION_CENTERFREQUENCY,
                           IUS_PATH_DEMODULATION_NUMSAMPLESPERLINE,
                           IUS_PATH_DEMODULATION_METHOD);
        return IUDM_INVALID;
    }

	hid_t tgc_id = H5Gopen(handle, IUS_PATH_DEMODULATION_TGC, H5P_DEFAULT);
	tgc = iusTGCLoad(tgc_id);
	H5Gclose(tgc_id);
	if (tgc == IUTGC_INVALID) return IUDM_INVALID;

	hid_t filter_id = H5Gopen(handle, IUS_PATH_DEMODULATION_PREFILTER, H5P_DEFAULT);
	filter = iusFirFilterLoad(filter_id);
	H5Gclose(filter_id);
	if (filter == IUFIRFILTER_INVALID) return IUDM_INVALID;

	iusDemodulation = iusDemodulationCreateWithoutTGCandFilter((IusDemodulationMethod)method, sampleFrequency, centerFrequency, numSamplesPerLine);
	iusDemodulation->TGC = tgc;
	iusDemodulation->preFilter = filter;

	return iusDemodulation;
}
