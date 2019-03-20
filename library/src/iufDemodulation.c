//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iuf.h>
#include <iufFilterPrivate.h>
#include <iufIqFileStructure.h>
#include <iufDemodulationPrivate.h>
#include <iufTGCPrivate.h>

struct IufDemodulation
{
	IufDemodulationMethod method;   /**< How the downsampling has been performed */
	int      downSampleFactor;      /**< The reduction factor between original sampling and the data rate (i.e. sampleFrequency) */
	iuff_t   preFilter;             /**< The filter that has been used before downSampling was applied */
	iutgc_t  TGC;                   /**< TimeGainControl points (time,gain) */
	float    sampleFrequency;       /**< The sampling frequency that was used */
	float    centerFrequency;       /**< The center frequency that was used */
	int      numSamplesPerLine;     /**< length of an acquisition line */
};

// ADT
static iudm_t iufDemodulationCreateWithoutTGCandFilter
(
	IufDemodulationMethod method,
	float sampleFrequency,
	float centerFrequency,
	int numSamplesPerLine
)
{
	iudm_t created = calloc(1, sizeof(IufDemodulation));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufDemodulation, IUDM_INVALID);
    created->sampleFrequency = sampleFrequency;
    created->centerFrequency = centerFrequency;
	created->numSamplesPerLine = numSamplesPerLine;
	created->method = method;
	return created;
}

iudm_t iufDemodulationCreate
(
	IufDemodulationMethod method,
	float sampleFrequency,
	float centerFrequency,
	int numSamplesPerLine,
	int numTGCentries,
	int filterKernelSize
)
{
    if( centerFrequency < 0.0f )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "centerFrequency argument should be >= 0.0, but was: '%f'", centerFrequency);
        return IUDM_INVALID;
    }

    if( sampleFrequency <= 0.0f )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "sampleFrequency argument should be > 0.0, but was: '%f'", sampleFrequency);
        return IUDM_INVALID;
    }

    if( numSamplesPerLine < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "numSamplesPerLine argument should be >= 0, but was: '%d'", numSamplesPerLine);
        return IUDM_INVALID;
    }

    if( numTGCentries <= 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "numTGCentries argument should be > 0, but was: '%d'", numTGCentries);
        return IUDM_INVALID;
    }

    if( filterKernelSize <= 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "filterKernelSize argument should be > 0, but was: '%d'", numTGCentries);
        return IUDM_INVALID;
    }
    iudm_t created = iufDemodulationCreateWithoutTGCandFilter(method, sampleFrequency, centerFrequency, numSamplesPerLine);
	created->TGC = iufTGCCreate(numTGCentries);
	created->preFilter = iufFirFilterCreate(filterKernelSize);

	return created;
}

int iufDemodulationDelete
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, IUF_ERR_VALUE);
    iufTGCDelete(demodulation->TGC);
    iufFirFilterDelete(demodulation->preFilter);
    free(demodulation);
	return IUF_E_OK;
}


// operations
int iufDemodulationCompare
(
	iudm_t reference,
	iudm_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (IUF_EQUAL_FLOAT(reference->sampleFrequency, actual->sampleFrequency) == IUF_FALSE) return IUF_FALSE;
	if (IUF_EQUAL_FLOAT(reference->centerFrequency, actual->centerFrequency) == IUF_FALSE) return IUF_FALSE;
	if (reference->numSamplesPerLine != actual->numSamplesPerLine) return IUF_FALSE;
	if (iufFirFilterCompare(reference->preFilter, actual->preFilter) == IUF_FALSE) return IUF_FALSE;
	return iufTGCCompare(reference->TGC, actual->TGC);
}

// Getters
iutgc_t iufDemodulationGetTGC
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, IUTGC_INVALID);
    return demodulation->TGC;
}

// Getters
iuff_t iufDemodulationGetPreFilter
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, IUFIRFILTER_INVALID);
	return demodulation->preFilter;
}

float iufDemodulationGetCenterFrequency
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, NAN);
	return demodulation->centerFrequency;
}

float iufDemodulationGetSampleFrequency
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, NAN);
	return demodulation->sampleFrequency;
}

int iufDemodulationGetNumSamplesPerLine
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, -1);
	return demodulation->numSamplesPerLine;
}

int iufDemodulationGetNumTGCentries
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, -1);
	return iufTGCGetNumValues(demodulation->TGC);
}

int iufDemodulationGetPreFilterKernelSize
(
	iudm_t demodulation
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, -1);
	return iufFirFilterGetKernelSize(demodulation->preFilter);
}

//setters
int iufDemodulationSetTGC
(
	iudm_t demodulation,
    iutgc_t tgc
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufTGCGetNumValues(tgc) != iufTGCGetNumValues(demodulation->TGC), IUF_ERR_VALUE);

    for (int idx=0; idx<iufTGCGetNumValues(tgc); idx++)
    {
        float gain = iufTGCGetGain(tgc, idx);
        float time = iufTGCGetTime(tgc, idx);
        iufTGCSet(demodulation->TGC, idx, time, gain);
    }

    return IUF_E_OK;
}

//setters
int iufDemodulationSetPreFilter
(
	iudm_t demodulation,
    iuff_t preFilter
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(preFilter, IUF_ERR_VALUE);
    
    int kernelSize = iufFirFilterGetKernelSize(preFilter);
    iuff_t dmPreFilter = iufDemodulationGetPreFilter(demodulation);
    IUF_ERR_EVAL_N_RETURN(kernelSize <= 0, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(kernelSize != iufFirFilterGetKernelSize(dmPreFilter), IUF_ERR_VALUE);

    for (int idx=0; idx<kernelSize; idx++)
    {
        float coefficient = iufFirFilterGetCoefficient(preFilter, idx);
        iufFirFilterSetCoefficient(dmPreFilter, idx, coefficient);
    }

    return IUF_E_OK;
}
int iufDemodulationSave
(
	iudm_t demodulation,
	hid_t handle
)
{
	int status = 0;
	int method = (int)demodulation->method;

    IUF_ERR_CHECK_NULL_N_RETURN(demodulation, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	status |= iufHdf5WriteFloat(handle, IUF_PATH_DEMODULATION_SAMPLEFREQUENCY, &(demodulation->sampleFrequency), 1);
	status |= iufHdf5WriteFloat(handle, IUF_PATH_DEMODULATION_CENTERFREQUENCY, &(demodulation->centerFrequency), 1);
	status |= iufHdf5WriteInt(handle, IUF_PATH_DEMODULATION_NUMSAMPLESPERLINE, &(demodulation->numSamplesPerLine), 1);
	status |= iufHdf5WriteInt(handle, IUF_PATH_DEMODULATION_METHOD, &method, 1);
    if (status != 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "write failed for %s, %s, %s, and/or %s",
							IUF_PATH_DEMODULATION_SAMPLEFREQUENCY,
							IUF_PATH_DEMODULATION_CENTERFREQUENCY,
							IUF_PATH_DEMODULATION_NUMSAMPLESPERLINE,
                           IUF_PATH_DEMODULATION_METHOD);
        return IUF_ERR_VALUE;
    }

	hid_t tgc_id = H5Gcreate(handle, IUF_PATH_DEMODULATION_TGC, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iufTGCSave(demodulation->TGC, tgc_id);
	H5Gclose(tgc_id);

	hid_t filter_id = H5Gcreate(handle, IUF_PATH_DEMODULATION_PREFILTER, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iufFirFilterSave(demodulation->preFilter, filter_id);
	H5Gclose(filter_id);

	return status;
}

iudm_t iufDemodulationLoad
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
	iudm_t iufDemodulation;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDM_INVALID);
	status |= iufHdf5ReadFloat(handle, IUF_PATH_DEMODULATION_SAMPLEFREQUENCY, &sampleFrequency);
	status |= iufHdf5ReadFloat(handle, IUF_PATH_DEMODULATION_CENTERFREQUENCY, &centerFrequency);
	status |= iufHdf5ReadInt(handle, IUF_PATH_DEMODULATION_NUMSAMPLESPERLINE, &numSamplesPerLine);
	status |= iufHdf5ReadInt(handle, IUF_PATH_DEMODULATION_METHOD, &method);
    if (status != 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "read failed for %s, %s %s, or %s",
                           IUF_PATH_DEMODULATION_SAMPLEFREQUENCY,
                           IUF_PATH_DEMODULATION_CENTERFREQUENCY,
                           IUF_PATH_DEMODULATION_NUMSAMPLESPERLINE,
                           IUF_PATH_DEMODULATION_METHOD);
        return IUDM_INVALID;
    }

	hid_t tgc_id = H5Gopen(handle, IUF_PATH_DEMODULATION_TGC, H5P_DEFAULT);
	tgc = iufTGCLoad(tgc_id);
	H5Gclose(tgc_id);
	if (tgc == IUTGC_INVALID) return IUDM_INVALID;

	hid_t filter_id = H5Gopen(handle, IUF_PATH_DEMODULATION_PREFILTER, H5P_DEFAULT);
	filter = iufFirFilterLoad(filter_id);
	H5Gclose(filter_id);
	if (filter == IUFIRFILTER_INVALID) return IUDM_INVALID;

	iufDemodulation = iufDemodulationCreateWithoutTGCandFilter((IufDemodulationMethod)method, sampleFrequency, centerFrequency, numSamplesPerLine);
	iufDemodulation->TGC = tgc;
	iufDemodulation->preFilter = filter;

	return iufDemodulation;
}
