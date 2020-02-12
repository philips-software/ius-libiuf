#include <iuf.h>

#define SOURCE_LABEL "V2_Source"
#define PULSE_LABEL "V2_Pulse"

//for packing position and size
typedef struct TripleFloat_
{
	float x;
	float y;
	float z;
} TripleFloat;

// for packing angles
typedef struct DualFloat_
{
	float theta;
	float phi;
} DualFloat;

//helper function for reading a iu2dp_t
static iu2dp_t iufHdf5Read2DPos(hid_t elementsHandle, int index)
{
	int status;
	//hid_t dataset = H5Dopen(pInst->handle, "/Transducer/Elements/positions", H5P_DEFAULT);
	hsize_t  dims[1];
	
	hid_t positionsHandle = H5Dopen(elementsHandle, "positions", H5P_DEFAULT);
	hid_t space = H5Dget_space(positionsHandle);
	int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	if (ndims != 1)
	{
        IUF_ERROR_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE, "iufHdf5Read2DPos: Only 1D array of transducer elements expected.");
		return IU2DP_INVALID;
	}
	hid_t position_tid = H5Tcreate(H5T_COMPOUND, sizeof(TripleFloat));
	H5Tinsert(position_tid, "x", HOFFSET(TripleFloat, x), H5T_NATIVE_FLOAT);
	H5Tinsert(position_tid, "y", HOFFSET(TripleFloat, y), H5T_NATIVE_FLOAT);
	H5Tinsert(position_tid, "z", HOFFSET(TripleFloat, z), H5T_NATIVE_FLOAT);
	TripleFloat *posData = (TripleFloat *)malloc((size_t)(dims[0] * sizeof(TripleFloat)));
	status = H5Dread(positionsHandle, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, posData); //We are reading the whole thing just to return 1 item...
	if (status != IUF_E_OK)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Reading data: H5Dread returned %d.", status);
		free(posData);
		return IU2DP_INVALID;
	}
	iu2dp_t pos = iuf2DPositionCreate(posData[index].x, posData[index].z);
	free(posData);
	status = H5Dclose(positionsHandle);
	status = H5Sclose(space);
	status = H5Tclose(position_tid);

	return pos;
}

//helper function for reading a iu2ds_t
static iu2ds_t iufHdf5Read2DSize(hid_t elementsHandle, int index)
{
	int status;
	//hid_t dataset = H5Dopen(pInst->handle, "/Transducer/Elements/positions", H5P_DEFAULT);
	hsize_t  dims[1];

	hid_t sizesHandle = H5Dopen(elementsHandle, "sizes", H5P_DEFAULT);
	hid_t space = H5Dget_space(sizesHandle);
	int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	if (ndims != 1)
	{
        IUF_ERROR_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE, "Only 1D array of transducer elements expected");
		return IU2DS_INVALID;
	}
	hid_t size_tid = H5Tcreate(H5T_COMPOUND, sizeof(TripleFloat));
	H5Tinsert(size_tid, "x", HOFFSET(TripleFloat, x), H5T_NATIVE_FLOAT);
	H5Tinsert(size_tid, "y", HOFFSET(TripleFloat, y), H5T_NATIVE_FLOAT);
	H5Tinsert(size_tid, "z", HOFFSET(TripleFloat, z), H5T_NATIVE_FLOAT);
	TripleFloat *sizeData = (TripleFloat *)malloc((size_t)(dims[0] * sizeof(TripleFloat)));
	status = H5Dread(sizesHandle, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sizeData); //We are reading the whole thing just to return 1 item...
	if (status != IUF_E_OK)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Reading data: H5Dread returned %d.", status);
		free(sizeData);
		return IU2DS_INVALID;
	}
	iu2ds_t size = iuf2DSizeCreate(sizeData[index].x, sizeData[index].z);
	free(sizeData);
	status = H5Dclose(sizesHandle);
	status = H5Sclose(space);
	status = H5Tclose(size_tid);

	return size;
}

//helper function for reading a iu2da_t
static int iufHdf5Read2DAngle(hid_t elementsHandle, int index, float *theta)
{
	int status;
	//hid_t dataset = H5Dopen(pInst->handle, "/Transducer/Elements/positions", H5P_DEFAULT);
	hsize_t  dims[1];

	hid_t anglesHandle = H5Dopen(elementsHandle, "angles", H5P_DEFAULT);
	hid_t space = H5Dget_space(anglesHandle);
	int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	if (ndims != 1)
	{
        IUF_ERROR_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE, "Only 1D array of transducer elements expected");
		return IUF_ERR_VALUE;
	}

	hid_t angle_tid = H5Tcreate(H5T_COMPOUND, sizeof(DualFloat));
	H5Tinsert(angle_tid, "theta", HOFFSET(DualFloat, theta), H5T_NATIVE_FLOAT);
	H5Tinsert(angle_tid, "phi", HOFFSET(DualFloat, phi), H5T_NATIVE_FLOAT);
	DualFloat *angleData = (DualFloat *)malloc((size_t)(dims[0] * sizeof(DualFloat)));
	status = H5Dread(anglesHandle, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, angleData); //We are reading the whole thing just to return 1 item...
	if (status != IUF_E_OK)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Reading data: H5Dread returned %d.", status);
		free(angleData);
		return status;
	}
	*theta = angleData[index].theta;
	free(angleData);
	status = H5Dclose(anglesHandle);
	status = H5Sclose(space);
	status = H5Tclose(angle_tid);

	return IUF_E_OK;
}

static int extractFrameDuration(hid_t inputFile, float *duration)
{
	int status;
	int numPulses;
	float transmitDelay;

	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);
	if (drivingSchemeHandle < 0)
	{
		return IUF_ERR_GENERAL;
	}
	hid_t transmitPatternHandle = H5Gopen(drivingSchemeHandle, "TransmitPattern", H5P_DEFAULT);
	if (transmitPatternHandle < 0)
	{
		return IUF_ERR_GENERAL;
	}
	status = iufHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numPulses);
	status |= iufHdf5ReadFloat(transmitPatternHandle, "transmitPatternDelay", &transmitDelay);

	float *transmitTimes = (float *)calloc(numPulses, sizeof(float));
	if (transmitTimes == NULL)
	{
		return IUF_ERR_MEMORY;
	}
	status |= iufHdf5ReadFloat(transmitPatternHandle, "transmitPatternTime", transmitTimes);
	status |= H5Gclose(transmitPatternHandle);
	status |= H5Gclose(drivingSchemeHandle);

	*duration = transmitDelay + transmitTimes[numPulses - 1];
	free(transmitTimes);

	return status;
}

static iusd_t extractSource(hid_t inputFile)
{
	int status;
	iusd_t sourceDict = iufSourceDictCreate();
	hid_t drivingScheme = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);

	float fNumber, deltaTheta, startTheta;
	int numLocations;

	status = iufHdf5ReadFloat(drivingScheme, "sourceFNumber", &fNumber);
	status |= iufHdf5ReadInt(drivingScheme, "numTransmitSources", &numLocations);
	status |= iufHdf5ReadFloat(drivingScheme, "sourceAngularDelta", &deltaTheta);
	status |= iufHdf5ReadFloat(drivingScheme, "sourceStartAngle", &startTheta);
	status |= H5Gclose(drivingScheme);

	if (status != IUF_E_OK)
	{
		return IUFD_INVALID;
	}

	iu2dps_t source = iuf2DParametricSourceCreate(numLocations, fNumber, deltaTheta, startTheta); // the only one implemented
	status = iufSourceDictSet(sourceDict, SOURCE_LABEL, (ius_t)source);
	if (status != IUF_E_OK)
	{
		return IUFD_INVALID;
	}

	return sourceDict;
}

static iupd_t extractPulse(hid_t inputFile)
{
	iupd_t pulseDict = iufPulseDictCreate();
	hid_t pulseHandle = H5Gopen(inputFile, "DrivingScheme/TransmitPulse", H5P_DEFAULT); //

	int numPulseValues, count;
	float frequency, amplitude;
	int status = iufHdf5ReadInt(pulseHandle, "numPulseValues", &numPulseValues);
	if (status != IUF_E_OK)
	{
		return IUPD_INVALID;
	}
	if (numPulseValues == 0) //paramteric pulse
	{
		status |= iufHdf5ReadInt(pulseHandle, "pulseCount", &count);
		status |= iufHdf5ReadFloat(pulseHandle, "pulseFrequency", &frequency);
		status |= iufHdf5ReadFloat(pulseHandle, "pulseAmplitude", &amplitude);
		status |= H5Gclose(pulseHandle);
		if (status != IUF_E_OK)
		{
			return IUPD_INVALID;
		}
		iupp_t ppulse = iufParametricPulseCreate(frequency, amplitude, count);
		status = iufPulseDictSet(pulseDict, PULSE_LABEL, (iup_t)ppulse);
	}
	else //nonParamteric pulse
	{
		iunpp_t nppulse = iufNonParametricPulseCreate(numPulseValues);
		float *pulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
		float *pulseTimes = (float *)calloc(numPulseValues, sizeof(float));
		status |= iufHdf5ReadFloat(pulseHandle, "pulseTimes", pulseTimes);
		status |= iufHdf5ReadFloat(pulseHandle, "pulseAmplitudes", pulseAmplitudes);
		status |= H5Gclose(pulseHandle);
		if (status != IUF_E_OK)
		{
			return IUPD_INVALID;
		}
		for (int i = 0; i < numPulseValues && status == IUF_E_OK; i++)
		{
			status |= iufNonParametricPulseSetValue(nppulse, i, pulseTimes[i], pulseAmplitudes[i]);
		}
		free(pulseAmplitudes);
		free(pulseTimes);
		if (status != IUF_E_OK)
		{
			return IUPD_INVALID;
		}
		status = iufPulseDictSet(pulseDict, PULSE_LABEL, (iup_t)nppulse);
	}
	if (status != IUF_E_OK)
	{
		return IUPD_INVALID;
	}
	return pulseDict;
}

static iursd_t extractReceiveSettings(hid_t inputFile, char *mode)
{
	int status = IUF_E_OK;
	iursd_t receiveSettingsDict = iufReceiveSettingsDictCreate();
	hid_t receiveSettingsHandle = H5Gopen(inputFile, "ReceiveSettings", H5P_DEFAULT); //
	hid_t TGCHandle = H5Gopen(receiveSettingsHandle, "TimeGainControl", H5P_DEFAULT); //
	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT); //
	if (receiveSettingsHandle < 0 || TGCHandle < 0 || drivingSchemeHandle < 0)
	{
		return IURSD_INVALID;
	}

	float sampleFrequency;
	int numSamplesPerLine;
	int numTGCEntries=0;
	status |= iufHdf5ReadFloat(receiveSettingsHandle, "sampleFrequency", &sampleFrequency);
	status |= iufHdf5ReadInt(drivingSchemeHandle, "numSamplesPerLine", &numSamplesPerLine);
	status |= iufHdf5ReadInt(TGCHandle, "numValues", &numTGCEntries);
	
	if (numTGCEntries < 1)
	{
		return IURSD_INVALID;
	}

	float *TGCTimes      = (float *)calloc(numTGCEntries, sizeof(float));
	float *TGCAmplitudes = (float *)calloc(numTGCEntries, sizeof(float));
	if (TGCTimes == NULL || TGCAmplitudes == NULL)
	{
		return IURSD_INVALID;
	}

	status |= iufHdf5ReadFloat(TGCHandle, "gains", TGCAmplitudes);
	status |= iufHdf5ReadFloat(TGCHandle, "timings", TGCTimes);
	//close the handles
	status |= H5Gclose(receiveSettingsHandle);
	status |= H5Gclose(drivingSchemeHandle);
	status |= H5Gclose(TGCHandle);
	if (status != IUF_E_OK)
	{
		free(TGCTimes);
		free(TGCAmplitudes);
		return IURSD_INVALID;
	}

	iurs_t receiveSettings = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCEntries);
	for (int i = 0; i < numTGCEntries; i++)
	{
		status |= iufTGCSet(iufReceiveSettingsGetTGC(receiveSettings), i, TGCTimes[i], TGCAmplitudes[i]);
	}
	if (status != IUF_E_OK)
	{
		free(TGCTimes);
		free(TGCAmplitudes);
		return IURSD_INVALID;
	}
	
	status = iufReceiveSettingsDictSet(receiveSettingsDict, mode, receiveSettings);
	free(TGCTimes);
	free(TGCAmplitudes);
	if (status != IUF_E_OK)
	{
		return IURSD_INVALID;
	}

	return receiveSettingsDict;
}

static 	iurcmd_t extractReceiveChannelMapDict(hid_t inputFile, char *mode)
{
	int numChannels;
	int status;
	int numDelays=0; // equal to numtransmitPulses, while we use only the first one

	hid_t receiveSettingsHandle = H5Gopen(inputFile, "ReceiveSettings", H5P_DEFAULT); //
	hid_t transducerHandle = H5Gopen(inputFile, "Transducer", H5P_DEFAULT);
	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT); //
	if (receiveSettingsHandle < 0 || transducerHandle < 0 || drivingSchemeHandle < 0)
	{
		return IURCMD_INVALID;
	}

	//receiveChannelMap has not been introduced in V2 so always full one-to-one mapping siz of num transducer elements
	iurcmd_t receiveChannelMapDict = iufReceiveChannelMapDictCreate();
	status = iufHdf5ReadInt(transducerHandle, "numElements", &numChannels);
	status |= H5Gclose(transducerHandle);
	if (status != IUF_E_OK)
	{
		return IURCMD_INVALID;
	}

	status = iufHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numDelays);
	status |= H5Gclose(drivingSchemeHandle);
	if (status != IUF_E_OK)
	{
		return IURCMD_INVALID;
	}

	if (numDelays < 1)
	{
		return IURCMD_INVALID;
	}

	float *startDelays = (float *)calloc(numDelays, sizeof(float));
	if (startDelays == NULL)
	{
		return IURCMD_INVALID;
	}

	status = iufHdf5ReadFloat(receiveSettingsHandle, "startDepth", startDelays);
	status |= H5Gclose(receiveSettingsHandle);
	if (status != IUF_E_OK)
	{
		free(startDelays);
		return IURCMD_INVALID;
	}

	iurcm_t receiveChannelMap = iufReceiveChannelMapCreate(numChannels);
	for (int i = 0; i < numChannels; i++)
	{
		status |= iufReceiveChannelMapSetChannel(receiveChannelMap, i, i); //strainght forward 1 to 1 mapping
        status |= iufReceiveChannelMapSetStartDelay(receiveChannelMap, i, startDelays[ 0 ]); // Only use the first one
	}

	status = iufReceiveChannelMapDictSet(receiveChannelMapDict, mode, receiveChannelMap);
	if (status != IUF_E_OK)
	{
		return IURCMD_INVALID;
	}
	return receiveChannelMapDict;
}

static iutad_t extractTransmitApodizationDict(hid_t inputFile)
{
	int status;
	int numChannels, numPulses;
	iutad_t transmitApodizationDict = iufTransmitApodizationDictCreate();

	hid_t transducerHandle = H5Gopen(inputFile, "Transducer", H5P_DEFAULT);
	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);
	if (transducerHandle < 0 || drivingSchemeHandle < 0)
	{
		return IUTAD_INVALID;
	}
	//in V2 there was always 1 transmit apodization per pulse, so here we make a dictionary referring to each pulse with an automated label
	status = iufHdf5ReadInt(transducerHandle, "numElements", &numChannels);
	status |= iufHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numPulses);
	if (status != IUF_E_OK)
	{
		return IUTAD_INVALID;
	}

	float *apodizations = (float *)calloc(numPulses*numChannels, sizeof(float));
	status = iufHdf5ReadFloat(drivingSchemeHandle, "transmitApodization", apodizations);
	status |= H5Gclose(transducerHandle);
	status |= H5Gclose(drivingSchemeHandle);
	if (status != IUF_E_OK)
	{
		free(apodizations);
		return IUTAD_INVALID;
	}
	for (int i=0; i<numPulses && status == IUF_E_OK; i++)
	{
		char pulseLabel[32];
		float *apod = apodizations + i*numChannels;
		iuta_t transmitApodization = iufTransmitApodizationCreate(apod, numChannels);
		snprintf(pulseLabel, sizeof(pulseLabel), "pulse[%d]", i);
		status |= iufTransmitApodizationDictSet(transmitApodizationDict, pulseLabel, transmitApodization);
	}
	if (status != IUF_E_OK)
	{
		free(apodizations);
		return IUTAD_INVALID;
	}
	return transmitApodizationDict;
}

static iua_t extractAcquisition(hid_t inputFile)
{
	int status;
	hid_t experimentHandle = H5Gopen(inputFile, "Experiment", H5P_DEFAULT);
	
	int date;
	char description[IUF_MAX_STRING_LENGTH];
	float speedOfSound;

	if (experimentHandle < 0)
	{
		return IUA_INVALID;
	}
	status  = iufHdf5ReadInt(experimentHandle, "date", &date);
	status |= iufHdf5ReadString(experimentHandle, "description", description);
	status |= iufHdf5ReadFloat(experimentHandle, "speedOfSound", &speedOfSound);
	status |= H5Gclose(experimentHandle);
	if (status != IUF_E_OK)
	{
		return IUA_INVALID;  //todo rename IUE_INVLAID to IUA_INVALID
	}

	return iufAcquisitionCreate(speedOfSound, date, description);
}

static 	iut_t extractTransducer(hid_t inputFile)
{
	int status;
	char name[IUF_MAX_STRING_LENGTH];
	float centerFrequency;
	int numElements;

	hid_t transducerHandle = H5Gopen(inputFile, "Transducer", H5P_DEFAULT);
	hid_t elementsHandle = H5Gopen(transducerHandle, "Elements", H5P_DEFAULT);
	if (transducerHandle < 0 || elementsHandle < 0)
	{
		return IUT_INVALID;
	}
	status = iufHdf5ReadInt(transducerHandle, "numElements", &numElements);
	status |= iufHdf5ReadFloat(transducerHandle, "centerFrequency", &centerFrequency);
	status |= iufHdf5ReadString(transducerHandle, "transducerName", name);
	if (status != IUF_E_OK)
	{
		return IUT_INVALID;
	}
	iu2dt_t transducer = iuf2DTransducerCreate(name, IUF_LINE, centerFrequency, numElements); //for V2 we assume elements are on a 2D line
	
	for (int i = 0; i < numElements && status == IUF_E_OK; i++)
	{
		float theta=0.0;
		status |= iufHdf5Read2DAngle(elementsHandle, i, &theta); //not very efficient but at least readable code
		iu2dp_t pos = iufHdf5Read2DPos(elementsHandle, i);
		iu2ds_t size = iufHdf5Read2DSize(elementsHandle, i);
		
		iu2dte_t transducerElement = iuf2DTransducerElementCreate(pos, theta, size);
		status |= iuf2DTransducerSetElement(transducer, i, transducerElement);
	}
	if (status != IUF_E_OK)
	{
		return IUT_INVALID;
	}

	return (iut_t)transducer;
}

static 	iupald_t extractPatternListDict
(
	hid_t inputFile,
	char *label,
	iursd_t receiveSettingsDict,
	iurcmd_t receiveChannelMapDict
)
{
	// Since V2 format defines a transmitApodization per Pulse, we will have a pattern for each pulse in a frame
	iupald_t paternListDict = iufPatternListDictCreate();
	int status;
	float transmitDelay;
	int numPulses;
	char apodizationLabel[IUF_MAX_STRING_LENGTH];

	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);
	if (drivingSchemeHandle < 0)
	{
		return IUPALD_INVALID;
	}
	hid_t transmitPatternHandle = H5Gopen(drivingSchemeHandle, "TransmitPattern", H5P_DEFAULT);
	if (transmitPatternHandle < 0)
	{
		return IUPALD_INVALID;
	}
	status = iufHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numPulses);
	status |= iufHdf5ReadFloat(transmitPatternHandle, "transmitPatternDelay", &transmitDelay);
	
	int *transmitIndices  = (int *)calloc(numPulses, sizeof(int));
	float *transmitTimes  = (float *)calloc(numPulses, sizeof(float));
	if (transmitIndices == NULL || transmitTimes == NULL)
	{
		return IUPALD_INVALID;
	}
	status |= iufHdf5ReadInt(transmitPatternHandle, "transmitPatternIndex", transmitIndices);
	status |= iufHdf5ReadFloat(transmitPatternHandle, "transmitPatternTime",  transmitTimes);
	status |= H5Gclose(transmitPatternHandle);
	status |= H5Gclose(drivingSchemeHandle);

	iupal_t patternList = iufPatternListCreate(numPulses, receiveSettingsDict, receiveChannelMapDict);
	for (int i = 0; i < numPulses && status == IUF_E_OK; i++)
	{
        snprintf(apodizationLabel, sizeof(apodizationLabel), "pulse[%d]", i); // for each pulse an apodizaion is defined in V2
		iupa_t pattern = iufPatternCreate(transmitTimes[i], PULSE_LABEL, SOURCE_LABEL, label, apodizationLabel, label);
		if (pattern == IUPA_INVALID)
		{
			return IUPALD_INVALID;
		}
		status |= iufPatternListSet(patternList, pattern, i);
	}
	free(transmitIndices);
	free(transmitTimes);

	iufPatternListDictSet(paternListDict, label, patternList);
	return paternListDict;
}

static 	iufl_t extractFrameList(hid_t inputFile, char *label)
{
	int status;
	int numFrames;
	float frameDuration=0.0;

	status = iufHdf5ReadInt(inputFile, "numFrames", &numFrames);
	status |= extractFrameDuration(inputFile, &frameDuration);
	if (status != IUF_E_OK)
	{
		return IUFL_INVALID;
	}
	iufl_t frameList = iufFrameListCreate(numFrames);
	for (int i = 0; i < numFrames; i++)
	{
		iufr_t frame = iufFrameCreate(label, i, frameDuration*i);
		status |= iufFrameListSet(frameList, frame, i);
	}

	return frameList;
}


int readInputFileV2
(
	iuif_t outputFile,
	hid_t inputFile,
	char *mode
)
{
	iusd_t sourceDict = extractSource(inputFile);
	int status = iufInputFileSetSourceDict(outputFile, sourceDict);

	iupd_t pulseDict = extractPulse(inputFile);
	status |= iufInputFileSetPulseDict(outputFile, pulseDict);

	iursd_t receiveSettingsDict = extractReceiveSettings(inputFile, mode);   //actually check the patternlist label what label to use
	status |= iufInputFileSetReceiveSettingsDict(outputFile, receiveSettingsDict);

	iurcmd_t receiveChannelMapDict = extractReceiveChannelMapDict(inputFile, mode);  //actually check the patternlist label what label to use
	status |= iufInputFileSetReceiveChannelMapDict(outputFile, receiveChannelMapDict);

	iupald_t patternListDict = extractPatternListDict(inputFile, mode, receiveSettingsDict, receiveChannelMapDict);
	status |= iufInputFileSetPatternListDict(outputFile, patternListDict);

	iutad_t transmitApodizationDict = extractTransmitApodizationDict(inputFile);
	status |= iufInputFileSetTransmitApodizationDict(outputFile, transmitApodizationDict);

	 iua_t acquisition = extractAcquisition(inputFile);
	status |= iufInputFileSetAcquisition(outputFile, acquisition);

	iut_t transducer = extractTransducer(inputFile);
	status |= iufInputFileSetTransducer(outputFile, transducer);


	 iufl_t frameList = extractFrameList(inputFile, mode);
	status |= iufInputFileSetFrameList(outputFile, frameList);

	return status;
}


