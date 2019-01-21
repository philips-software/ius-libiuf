//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ius.h>
#include <iusFilterPrivate.h>
#include <iusDemodulationPrivate.h>
#include <iusTGCPrivate.h>

struct IusDemodulation
{
	IusDemodulationMethod method;   /**< How the downsampling has been performed */
	int      downSampleFactor;      /**< The reduction factor between original sampling and the data rate (i.e. sampleFrequency) */
	iuff_t   preFilter;             /**< The filter that has been used before downSampling was applied */
	iutgc_t  TGC;                   /**< TimeGainControl points (time,gain) */
	float    sampleFrequency;       /**< The sampling frequency that was used */
	int      numSamplesPerLine;     /**< length of an acquisition line */
};

// ADT
iudm_t iusDemodulationCreateWithoutTGCandFilter
(
	IusDemodulationMethod method,
	float sampleFrequency,
	int numSamplesPerLine
)
{
	iudm_t created = calloc(1, sizeof(IusDemodulation));
	created->sampleFrequency = sampleFrequency;
	created->numSamplesPerLine = numSamplesPerLine;
	created->method = method;
	return created;
}

iudm_t iusDemodulationCreate
(
	IusDemodulationMethod method,
	float sampleFrequency,
	int numSamplesPerLine,
	int numTGCentries,
	int filterKernelSize
)
{
	if (sampleFrequency <= 0.0f) return IUDM_INVALID;
	if (numSamplesPerLine < 0) return IUDM_INVALID;
	if (numTGCentries <= 0) return IUDM_INVALID;
	if (filterKernelSize <= 0) return IUDM_INVALID;

	iudm_t created = iusDemodulationCreateWithoutTGCandFilter(method, sampleFrequency, numSamplesPerLine);
	created->TGC = iusTGCCreate(numTGCentries);
	created->preFilter = iusFirFilterCreate(filterKernelSize);
	
	return created;
}

int iusDemodulationDelete
(
	iudm_t iusDemodulation
)
{
	int status = IUS_ERR_VALUE;
	if (iusDemodulation != NULL)
	{
		iusTGCDelete(iusDemodulation->TGC);
		iusFirFilterDelete(iusDemodulation->preFilter);
		free(iusDemodulation);
		status = IUS_E_OK;
	}
	return status;
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
	if (reference->numSamplesPerLine != actual->numSamplesPerLine) return IUS_FALSE;
	if (iusFirFilterCompare(reference->preFilter, actual->preFilter) == IUS_FALSE) return IUS_FALSE;
	return iusTGCCompare(reference->TGC, actual->TGC);
}

// Getters
iutgc_t iusDemodulationGetTGC
(
	iudm_t iusDemodulation
)
{
	if (iusDemodulation == NULL) return IUTGC_INVALID;
	return iusDemodulation->TGC;
}

// Getters
iuff_t iusDemodulationGetPreFilter
(
	iudm_t iusDemodulation
)
{
	if (iusDemodulation == NULL) return IUFIRFILTER_INVALID;
	return iusDemodulation->preFilter;
}

float iusDemodulationGetSampleFrequency
(
	iudm_t iusDemodulation
)
{
	if (iusDemodulation == NULL) return NAN;
	return iusDemodulation->sampleFrequency;
}

int iusDemodulationGetNumSamplesPerLine
(
	iudm_t iusDemodulation
)
{
	if (iusDemodulation == NULL) return -1;
	return iusDemodulation->numSamplesPerLine;
}

int iusDemodulationGetNumTGCentries
(
	iudm_t iusDemodulation
)
{
	if (iusDemodulation == NULL) return -1;
	return iusTGCGetNumValues(iusDemodulation->TGC);
}

int iusDemodulationGetPreFilterKernelSize
(
	iudm_t iusDemodulation
)
{
	if (iusDemodulation == NULL) return -1;
	return iusFirFilterGetKernelSize(iusDemodulation->preFilter);
}


int iusDemodulationSave
(
	iudm_t iusDemodulation,
	hid_t handle
)
{
	int status = 0;
	int method = (int)iusDemodulation->method;

	status |= iusHdf5WriteFloat(handle, IUS_IQFILE_PATH_DEMODULATION_SAMPLEFREQUENCY, &(iusDemodulation->sampleFrequency), 1);
	status |= iusHdf5WriteInt(handle, IUS_IQFILE_PATH_DEMODULATION_NUMSAMPLESPERLINE, &(iusDemodulation->numSamplesPerLine), 1);
	status |= iusHdf5WriteInt(handle, IUS_IQFILE_PATH_DEMODULATION_METHOD, &method, 1);

	hid_t tgc_id = H5Gcreate(handle, IUS_IQFILE_PATH_DEMODULATION_TGC, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusTGCSave(iusDemodulation->TGC, tgc_id);
	H5Gclose(tgc_id);

	hid_t filter_id = H5Gcreate(handle, IUS_IQFILE_PATH_DEMODULATION_PREFILTER, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusFirFilterSave(iusDemodulation->preFilter, filter_id);
	H5Gclose(filter_id);

	return status;
}

iudm_t iusDemodulationLoad
(
	hid_t handle
)
{
	float sampleFrequency;
	int numSamplesPerLine;
	int status = 0;
	int method = -1;

	iutgc_t tgc;
	iuff_t filter;
	iudm_t iusDemodulation;

	status |= iusHdf5ReadFloat(handle, IUS_IQFILE_PATH_DEMODULATION_SAMPLEFREQUENCY, &sampleFrequency);
	status |= iusHdf5ReadInt(handle, IUS_IQFILE_PATH_DEMODULATION_NUMSAMPLESPERLINE, &numSamplesPerLine);
	status |= iusHdf5ReadInt(handle, IUS_IQFILE_PATH_DEMODULATION_METHOD, &method);
	if (status != 0) return IUDM_INVALID;

	hid_t tgc_id = H5Gopen(handle, IUS_IQFILE_PATH_DEMODULATION_TGC, H5P_DEFAULT);
	tgc = iusTGCLoad(tgc_id);
	H5Gclose(tgc_id);
	if (tgc == IUTGC_INVALID) return IUDM_INVALID;

	hid_t filter_id = H5Gopen(handle, IUS_IQFILE_PATH_DEMODULATION_PREFILTER, H5P_DEFAULT);
	filter = iusFirFilterLoad(filter_id);
	H5Gclose(filter_id);
	if (filter == IUFIRFILTER_INVALID) return IUDM_INVALID;

	iusDemodulation = iusDemodulationCreateWithoutTGCandFilter((IusDemodulationMethod)method, sampleFrequency, numSamplesPerLine);
	iusDemodulation->TGC = tgc;
	iusDemodulation->preFilter = filter;

	return iusDemodulation;
}
