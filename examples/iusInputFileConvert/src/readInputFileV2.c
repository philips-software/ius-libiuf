//
// Created by Ruijzendaal on 16/04/2018.
//
#include <include/ius.h>

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
static iu2dp_t iusHdf5Read2DPos(hid_t elementsHandle, int index)
{
	int status;
	//hid_t dataset = H5Dopen(pInst->handle, "/Transducer/Elements/positions", H5P_DEFAULT);
	hsize_t  dims[1];
	
	hid_t positionsHandle = H5Dopen(elementsHandle, "positions", H5P_DEFAULT);
	hid_t space = H5Dget_space(positionsHandle);
	int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	if (ndims != 1)
	{
        IUS_ERROR_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE, "iusHdf5Read2DPos: Only 1D array of transducer elements expected.");
		return IU2DP_INVALID;
	}
	hid_t position_tid = H5Tcreate(H5T_COMPOUND, sizeof(TripleFloat));
	H5Tinsert(position_tid, "x", HOFFSET(TripleFloat, x), H5T_NATIVE_FLOAT);
	H5Tinsert(position_tid, "y", HOFFSET(TripleFloat, y), H5T_NATIVE_FLOAT);
	H5Tinsert(position_tid, "z", HOFFSET(TripleFloat, z), H5T_NATIVE_FLOAT);
	TripleFloat *posData = (TripleFloat *)malloc((size_t)(dims[0] * sizeof(TripleFloat)));
	status = H5Dread(positionsHandle, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, posData); //We are reading the whole thing just to return 1 item...
	if (status != IUS_E_OK)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Reading data: H5Dread returned %d.", status);
		free(posData);
		return IU2DP_INVALID;
	}
	iu2dp_t pos = ius2DPositionCreate(posData[index].x, posData[index].z);
	free(posData);
	status = H5Dclose(positionsHandle);
	status = H5Sclose(space);
	status = H5Tclose(position_tid);

	return pos;
}

//helper function for reading a iu2ds_t
static iu2ds_t iusHdf5Read2DSize(hid_t elementsHandle, int index)
{
	int status;
	//hid_t dataset = H5Dopen(pInst->handle, "/Transducer/Elements/positions", H5P_DEFAULT);
	hsize_t  dims[1];

	hid_t sizesHandle = H5Dopen(elementsHandle, "sizes", H5P_DEFAULT);
	hid_t space = H5Dget_space(sizesHandle);
	int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	if (ndims != 1)
	{
        IUS_ERROR_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE, "Only 1D array of transducer elements expected");
		return IU2DS_INVALID;
	}
	hid_t size_tid = H5Tcreate(H5T_COMPOUND, sizeof(TripleFloat));
	H5Tinsert(size_tid, "x", HOFFSET(TripleFloat, x), H5T_NATIVE_FLOAT);
	H5Tinsert(size_tid, "y", HOFFSET(TripleFloat, y), H5T_NATIVE_FLOAT);
	H5Tinsert(size_tid, "z", HOFFSET(TripleFloat, z), H5T_NATIVE_FLOAT);
	TripleFloat *sizeData = (TripleFloat *)malloc((size_t)(dims[0] * sizeof(TripleFloat)));
	status = H5Dread(sizesHandle, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sizeData); //We are reading the whole thing just to return 1 item...
	if (status != IUS_E_OK)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Reading data: H5Dread returned %d.", status);
		free(sizeData);
		return IU2DS_INVALID;
	}
	iu2ds_t size = ius2DSizeCreate(sizeData[index].x, sizeData[index].z);
	free(sizeData);
	status = H5Dclose(sizesHandle);
	status = H5Sclose(space);
	status = H5Tclose(size_tid);

	return size;
}

//helper function for reading a iu2da_t
static int iusHdf5Read2DAngle(hid_t elementsHandle, int index, float *theta)
{
	int status;
	//hid_t dataset = H5Dopen(pInst->handle, "/Transducer/Elements/positions", H5P_DEFAULT);
	hsize_t  dims[1];

	hid_t anglesHandle = H5Dopen(elementsHandle, "angles", H5P_DEFAULT);
	hid_t space = H5Dget_space(anglesHandle);
	int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	if (ndims != 1)
	{
        IUS_ERROR_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE, "Only 1D array of transducer elements expected");
		return IUS_ERR_VALUE;
	}

	hid_t angle_tid = H5Tcreate(H5T_COMPOUND, sizeof(DualFloat));
	H5Tinsert(angle_tid, "theta", HOFFSET(DualFloat, theta), H5T_NATIVE_FLOAT);
	H5Tinsert(angle_tid, "phi", HOFFSET(DualFloat, phi), H5T_NATIVE_FLOAT);
	DualFloat *angleData = (DualFloat *)malloc((size_t)(dims[0] * sizeof(DualFloat)));
	status = H5Dread(anglesHandle, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, angleData); //We are reading the whole thing just to return 1 item...
	if (status != IUS_E_OK)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Reading data: H5Dread returned %d.", status);
		free(angleData);
		return status;
	}
	*theta = angleData[index].theta;
	free(angleData);
	status = H5Dclose(anglesHandle);
	status = H5Sclose(space);
	status = H5Tclose(angle_tid);

	return IUS_E_OK;
}

static int extractFrameDuration(hid_t inputFile, float *duration)
{
	int status;
	int numPulses;
	float transmitDelay;

	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);
	if (drivingSchemeHandle < 0)
	{
		return IUS_ERR_GENERAL;
	}
	hid_t transmitPatternHandle = H5Gopen(drivingSchemeHandle, "TransmitPattern", H5P_DEFAULT);
	if (transmitPatternHandle < 0)
	{
		return IUS_ERR_GENERAL;
	}
	status = iusHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numPulses);
	status |= iusHdf5ReadFloat(transmitPatternHandle, "transmitPatternDelay", &transmitDelay);

	float *transmitTimes = (float *)calloc(numPulses, sizeof(float));
	if (transmitTimes == NULL)
	{
		return IUS_ERR_MEMORY;
	}
	status |= iusHdf5ReadFloat(transmitPatternHandle, "transmitPatternTime", transmitTimes);
	status |= H5Gclose(transmitPatternHandle);
	status |= H5Gclose(drivingSchemeHandle);

	*duration = transmitDelay + transmitTimes[numPulses - 1];
	free(transmitTimes);

	return status;
}

static iusd_t extractSource(hid_t inputFile)
{
	int status;
	iusd_t sourceDict = iusSourceDictCreate();
	hid_t drivingScheme = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);

	float fNumber, deltaTheta, startTheta;
	int numLocations;

	status = iusHdf5ReadFloat(drivingScheme, "sourceFNumber", &fNumber);
	status |= iusHdf5ReadInt(drivingScheme, "numTransmitSources", &numLocations);
	status |= iusHdf5ReadFloat(drivingScheme, "sourceAngularDelta", &deltaTheta);
	status |= iusHdf5ReadFloat(drivingScheme, "sourceStartAngle", &startTheta);
	status |= H5Gclose(drivingScheme);

	if (status != IUS_E_OK)
	{
		return IUSD_INVALID;
	}

	iu2dps_t source = ius2DParametricSourceCreate(numLocations, fNumber, deltaTheta, startTheta); // the only one implemented
	status = iusSourceDictSet(sourceDict, SOURCE_LABEL, (ius_t)source);
	if (status != IUS_E_OK)
	{
		return IUSD_INVALID;
	}

	return sourceDict;
}

static iupd_t extractPulse(hid_t inputFile)
{
	iupd_t pulseDict = iusPulseDictCreate();
	hid_t pulseHandle = H5Gopen(inputFile, "DrivingScheme/TransmitPulse", H5P_DEFAULT); //

	int numPulseValues, count;
	float frequency, amplitude;
	int status = iusHdf5ReadInt(pulseHandle, "numPulseValues", &numPulseValues);
	if (status != IUS_E_OK)
	{
		return IUPD_INVALID;
	}
	if (numPulseValues == 0) //paramteric pulse
	{
		status |= iusHdf5ReadInt(pulseHandle, "pulseCount", &count);
		status |= iusHdf5ReadFloat(pulseHandle, "pulseFrequency", &frequency);
		status |= iusHdf5ReadFloat(pulseHandle, "pulseAmplitude", &amplitude);
		status |= H5Gclose(pulseHandle);
		if (status != IUS_E_OK)
		{
			return IUPD_INVALID;
		}
		iupp_t ppulse = iusParametricPulseCreate(frequency, amplitude, count);
		status = iusPulseDictSet(pulseDict, PULSE_LABEL, (iup_t)ppulse);
	}
	else //nonParamteric pulse
	{
		iunpp_t nppulse = iusNonParametricPulseCreate(numPulseValues);
		float *pulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
		float *pulseTimes = (float *)calloc(numPulseValues, sizeof(float));
		status |= iusHdf5ReadFloat(pulseHandle, "pulseTimes", pulseTimes);
		status |= iusHdf5ReadFloat(pulseHandle, "pulseAmplitudes", pulseAmplitudes);
		status |= H5Gclose(pulseHandle);
		if (status != IUS_E_OK)
		{
			return IUPD_INVALID;
		}
		for (int i = 0; i < numPulseValues && status == IUS_E_OK; i++)
		{
			status |= iusNonParametricPulseSetValue(nppulse, i, pulseTimes[i], pulseAmplitudes[i]);
		}
		free(pulseAmplitudes);
		free(pulseTimes);
		if (status != IUS_E_OK)
		{
			return IUPD_INVALID;
		}
		status = iusPulseDictSet(pulseDict, PULSE_LABEL, (iup_t)nppulse);
	}
	if (status != IUS_E_OK)
	{
		return IUPD_INVALID;
	}
	return pulseDict;
}

static iursd_t extractReceiveSettings(hid_t inputFile, char *mode)
{
	int status = IUS_E_OK;
	iursd_t receiveSettingsDict = iusReceiveSettingsDictCreate();
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
	status |= iusHdf5ReadFloat(receiveSettingsHandle, "sampleFrequency", &sampleFrequency);
	status |= iusHdf5ReadInt(drivingSchemeHandle, "numSamplesPerLine", &numSamplesPerLine);
	status |= iusHdf5ReadInt(TGCHandle, "numValues", &numTGCEntries);
	
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

	status |= iusHdf5ReadFloat(TGCHandle, "gains", TGCAmplitudes);
	status |= iusHdf5ReadFloat(TGCHandle, "timings", TGCTimes);
	//close the handles
	status |= H5Gclose(receiveSettingsHandle);
	status |= H5Gclose(drivingSchemeHandle);
	status |= H5Gclose(TGCHandle);
	if (status != IUS_E_OK)
	{
		free(TGCTimes);
		free(TGCAmplitudes);
		return IURSD_INVALID;
	}

	iurs_t receiveSettings = iusReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCEntries);
	for (int i = 0; i < numTGCEntries; i++)
	{
		status |= iusTGCSet(iusReceiveSettingsGetTGC(receiveSettings), i, TGCTimes[i], TGCAmplitudes[i]);
	}
	if (status != IUS_E_OK)
	{
		free(TGCTimes);
		free(TGCAmplitudes);
		return IURSD_INVALID;
	}
	
	status = iusReceiveSettingsDictSet(receiveSettingsDict, mode, receiveSettings);
	free(TGCTimes);
	free(TGCAmplitudes);
	if (status != IUS_E_OK)
	{
		return IURSD_INVALID;
	}

	return receiveSettingsDict;
}

static 	iurcmd_t extractReceiveChannelMapDict(hid_t inputFile, char *mode)
{
	int numChannels;
	int status;
	int numDelays=0; // equal to numtransmitPulses

	hid_t receiveSettingsHandle = H5Gopen(inputFile, "ReceiveSettings", H5P_DEFAULT); //
	hid_t transducerHandle = H5Gopen(inputFile, "Transducer", H5P_DEFAULT);
	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT); //
	if (receiveSettingsHandle < 0 || transducerHandle < 0 || drivingSchemeHandle < 0)
	{
		return IURCMD_INVALID;
	}

	//receiveChannelMap has not been introduced in V2 so always full one-to-one mapping siz of num transducer elements
	iurcmd_t receiveChannelMapDict = iusReceiveChannelMapDictCreate();
	status = iusHdf5ReadInt(transducerHandle, "numElements", &numChannels);
	status |= H5Gclose(transducerHandle);
	if (status != IUS_E_OK)
	{
		return IURCMD_INVALID;
	}

	status = iusHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numDelays);
	status |= H5Gclose(drivingSchemeHandle);
	if (status != IUS_E_OK)
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

	status = iusHdf5ReadFloat(receiveSettingsHandle, "startDepth", startDelays);
	status |= H5Gclose(receiveSettingsHandle);
	if (status != IUS_E_OK)
	{
		free(startDelays);
		return IURCMD_INVALID;
	}

	iurcm_t receiveChannelMap = iusReceiveChannelMapCreate(numChannels);
	for (int i = 0; i < numChannels; i++)
	{
		iusReceiveChannelMapSetChannel(receiveChannelMap, i, i); //strainght forward 1 to 1 mapping
	}

	for (int i = 0; i < numDelays; i++)
	{
		status |= iusReceiveChannelMapSetStartDelay(receiveChannelMap, i, startDelays[i]);
	}

	status = iusReceiveChannelMapDictSet(receiveChannelMapDict, mode, receiveChannelMap);
	if (status != IUS_E_OK)
	{
		return IURCMD_INVALID;
	}
	return receiveChannelMapDict;
}

static iutad_t extractTransmitApodizationDict(hid_t inputFile)
{
	int status;
	int numChannels, numPulses;
	iutad_t transmitApodizationDict = iusTransmitApodizationDictCreate();

	hid_t transducerHandle = H5Gopen(inputFile, "Transducer", H5P_DEFAULT);
	hid_t drivingSchemeHandle = H5Gopen(inputFile, "DrivingScheme", H5P_DEFAULT);
	if (transducerHandle < 0 || drivingSchemeHandle < 0)
	{
		return IUTAD_INVALID;
	}
	//in V2 there was always 1 transmit apodization per pulse, so here we make a dictionary referring to each pulse with an automated label
	status = iusHdf5ReadInt(transducerHandle, "numElements", &numChannels);
	status |= iusHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numPulses);
	if (status != IUS_E_OK)
	{
		return IUTAD_INVALID;
	}

	float *apodizations = (float *)calloc(numPulses*numChannels, sizeof(float));
	status = iusHdf5ReadFloat(drivingSchemeHandle, "transmitApodization", apodizations);
	status |= H5Gclose(transducerHandle);
	status |= H5Gclose(drivingSchemeHandle);
	if (status != IUS_E_OK)
	{
		free(apodizations);
		return IUTAD_INVALID;
	}
	for (int i=0; i<numPulses && status == IUS_E_OK; i++)
	{
		char pulseLabel[32];
		float *apod = apodizations + i*numChannels;
		iuta_t transmitApodization = iusTransmitApodizationCreate(numChannels);
		status |= iusTransmitApodizationSetApodization(transmitApodization, apod);
		sprintf(pulseLabel, "pulse[%d]", i);
		status |= iusTransmitApodizationDictSet(transmitApodizationDict, pulseLabel, transmitApodization);
	}
	if (status != IUS_E_OK)
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
	char description[IUS_MAX_STRING_LENGTH];
	float speedOfSound;

	if (experimentHandle < 0)
	{
		return IUA_INVALID;
	}
	status  = iusHdf5ReadInt(experimentHandle, "date", &date);
	status |= iusHdf5ReadString(experimentHandle, "description", description);
	status |= iusHdf5ReadFloat(experimentHandle, "speedOfSound", &speedOfSound);
	status |= H5Gclose(experimentHandle);
	if (status != IUS_E_OK)
	{
		return IUA_INVALID;  //todo rename IUE_INVLAID to IUA_INVALID
	}

	return iusAcquisitionCreate(speedOfSound, date, description);
}

static 	iut_t extractTransducer(hid_t inputFile)
{
	int status;
	char name[IUS_MAX_STRING_LENGTH];
	float centerFrequency;
	int numElements;

	hid_t transducerHandle = H5Gopen(inputFile, "Transducer", H5P_DEFAULT);
	hid_t elementsHandle = H5Gopen(transducerHandle, "Elements", H5P_DEFAULT);
	if (transducerHandle < 0 || elementsHandle < 0)
	{
		return IUT_INVALID;
	}
	status = iusHdf5ReadInt(transducerHandle, "numElements", &numElements);
	status |= iusHdf5ReadFloat(transducerHandle, "centerFrequency", &centerFrequency);
	status |= iusHdf5ReadString(transducerHandle, "transducerName", name);
	if (status != IUS_E_OK)
	{
		return IUT_INVALID;
	}
	iu2dt_t transducer = ius2DTransducerCreate(name, IUS_LINE, centerFrequency, numElements); //for V2 we assume elements are on a 2D line
	
	for (int i = 0; i < numElements && status == IUS_E_OK; i++)
	{
		float theta=0.0;
		status |= iusHdf5Read2DAngle(elementsHandle, i, &theta); //not very efficient but at least readable code
		iu2dp_t pos = iusHdf5Read2DPos(elementsHandle, i); 
		iu2ds_t size = iusHdf5Read2DSize(elementsHandle, i);
		
		iu2dte_t transducerElement = ius2DTransducerElementCreate(pos, theta, size);
		status |= ius2DTransducerSetElement(transducer, i, transducerElement);
	}
	if (status != IUS_E_OK)
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
	iupald_t paternListDict = iusPatternListDictCreate();
	int status;
	float transmitDelay;
	int numPulses;
	char apodizationLabel[IUS_MAX_STRING_LENGTH];

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
	status = iusHdf5ReadInt(drivingSchemeHandle, "numTransmitPulses", &numPulses);
	status |= iusHdf5ReadFloat(transmitPatternHandle, "transmitPatternDelay", &transmitDelay);
	
	int *transmitIndices  = (int *)calloc(numPulses, sizeof(int));
	float *transmitTimes  = (float *)calloc(numPulses, sizeof(float));
	if (transmitIndices == NULL || transmitTimes == NULL)
	{
		return IUPALD_INVALID;
	}
	status |= iusHdf5ReadInt(transmitPatternHandle, "transmitPatternIndex", transmitIndices);
	status |= iusHdf5ReadFloat(transmitPatternHandle, "transmitPatternTime",  transmitTimes);
	status |= H5Gclose(transmitPatternHandle);
	status |= H5Gclose(drivingSchemeHandle);

	iupal_t patternList = iusPatternListCreate(numPulses, receiveSettingsDict, receiveChannelMapDict);
	for (int i = 0; i < numPulses && status == IUS_E_OK; i++)
	{
		sprintf(apodizationLabel, "pulse[%d]", i); // for each pulse an apodizaion is defined in V2
		iupa_t pattern = iusPatternCreate(transmitTimes[i], PULSE_LABEL, SOURCE_LABEL, label, apodizationLabel, label);
		if (pattern == IUPA_INVALID)
		{
			return IUPALD_INVALID;
		}
		status |= iusPatternListSet(patternList, pattern, i);
	}
	free(transmitIndices);
	free(transmitTimes);

	iusPatternListDictSet(paternListDict, label, patternList);
	return paternListDict;
}

static 	iufl_t extractFrameList(hid_t inputFile, char *label)
{
	int status;
	int numFrames;
	float frameDuration=0.0;

	status = iusHdf5ReadInt(inputFile, "numFrames", &numFrames);
	status |= extractFrameDuration(inputFile, &frameDuration);
	if (status != IUS_E_OK)
	{
		return IUFL_INVALID;
	}
	iufl_t frameList = iusFrameListCreate(numFrames);
	for (int i = 0; i < numFrames; i++)
	{
		iufr_t frame = iusFrameCreate(label, i, frameDuration*i);
		status |= iusFrameListSet(frameList, frame, i);
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
	int status = iusInputFileSetSourceDict(outputFile, sourceDict);

	iupd_t pulseDict = extractPulse(inputFile);
	status |= iusInputFileSetPulseDict(outputFile, pulseDict);

	iursd_t receiveSettingsDict = extractReceiveSettings(inputFile, mode);   //actually check the patternlist label what label to use
	status |= iusInputFileSetReceiveSettingsDict(outputFile, receiveSettingsDict);

	iurcmd_t receiveChannelMapDict = extractReceiveChannelMapDict(inputFile, mode);  //actually check the patternlist label what label to use
	status |= iusInputFileSetReceiveChannelMapDict(outputFile, receiveChannelMapDict);

	iupald_t patternListDict = extractPatternListDict(inputFile, mode, receiveSettingsDict, receiveChannelMapDict);
	status |= iusInputFileSetPatternListDict(outputFile, patternListDict);

	iutad_t transmitApodizationDict = extractTransmitApodizationDict(inputFile);
	status |= iusInputFileSetTransmitApodizationDict(outputFile, transmitApodizationDict);

	 iua_t acquisition = extractAcquisition(inputFile);
	status |= iusInputFileSetAcquisition(outputFile, acquisition);

	iut_t transducer = extractTransducer(inputFile);
	status |= iusInputFileSetTransducer(outputFile, transducer);


	 iufl_t frameList = extractFrameList(inputFile, mode);
	status |= iusInputFileSetFrameList(outputFile, frameList);

	return status;
}


