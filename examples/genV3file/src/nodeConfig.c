/** ==============================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ==============================================================================
 *
 * \file nodeConfig.c
 * \brief File containing nodeConfig convenience routine definitions.
 *
 *  ==============================================================================
 */
#include <iuf.h>



static const char *pChannelMapLabel = "channelMapLabel";



void ncFillData
(
    iud_t data,
    float value
)
{
    int i;
    float *pData = iufDataGetPointer(data);
    int numSamples = iufDataGetSize(data);

    for (i=0; i<numSamples; i++)
    {
        pData[i] = value;
    }
}

iurs_t ncFillReceiveSettings
(
    void
)
{
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;

    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);

    return obj;
}

iursd_t ncFillReceiveSettingsDict
(
    char *label
)
{
    // Create
    iursd_t  dict = iufReceiveSettingsDictCreate();

    // Fill
    iurs_t obj = ncFillReceiveSettings();
    iufReceiveSettingsDictSet(dict,label,obj);
    return dict;
}

iupad_t ncFillParameterDict
(
    int numElements
)
{
    int elementID;
    iupad_t dict = iufParameterDictCreate();

    char generatedKey[1024];
    char generatedValue[1024];
    for (elementID=0;elementID<numElements;elementID++)
    {
        sprintf(generatedKey,"key_%d", elementID);
        sprintf(generatedValue,"value_%d", elementID);
        iufParameterDictSet(dict, generatedKey, generatedValue);
    }

    return dict;
}

iuhn_t ncFillHistoryNode
(

)
{
    char *type =  "TYPE3";
    iuhn_t node;

    node = iufHistoryNodeCreate(type);

    iupad_t parameterDict = ncFillParameterDict(10);
    iufHistoryNodeSetParameters(node, parameterDict);
    return node;
}



iufl_t ncFillFrameList
(
    int numFrames,
    char *label
)
{
    iufr_t obj;
    int i;
    iufl_t frameList = iufFrameListCreate(numFrames);

    for (i=0;i<numFrames;i++)
    {
        obj = iufFrameCreate(label,i+2,i*0.01f);
        iufFrameListSet(frameList, obj, i);
    }
    return frameList;
}

iupa_t ncFillPattern
(
    float timeInterval,
    char *pPulseLabel,
    char *pSourceLabel,
    char *pApodizationLabel,
    char *pReceivesettingsLabel
)
{
    iupa_t pattern = iufPatternCreate( timeInterval,
                                      pPulseLabel,
                                      pSourceLabel,
                                      pChannelMapLabel,
                                      pApodizationLabel,
                                      pReceivesettingsLabel);
    return pattern;
}

iupald_t ncFillPatternListDict
(
    char *patternListLabel,
    char *pPulseLabel,
    char *pSourceLabel,
    char *pApodizationLabel,
    char *pReceivesettingsLabel
)
{
    // fill list
    iupald_t patternListDict = iufPatternListDictCreate();
    iupal_t patternList = iufPatternListCreate(1,NULL,NULL);

    iupa_t pattern = ncFillPattern( 0.01f, pPulseLabel,
                                    pSourceLabel,
                                    pApodizationLabel,
                                    pReceivesettingsLabel);

    iufPatternListSet(patternList, pattern, 0);
    iufPatternListDictSet(patternListDict, patternListLabel, patternList);
    return patternListDict;
}

iupd_t ncFillPulseDict
(
    char *pulseLabel
)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */

    // create
    iupd_t dict = iufPulseDictCreate();

    // fill
    iupp_t parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, pulseCount);
    iufPulseDictSet(dict,pulseLabel, (iup_t) parametricPulse);
    return dict;
}

iusd_t ncFillSourceDict
(
    char *sourceLabel
)
{
    int locationCountTheta = 5; /**< number of locations */
	int locationCountPhi = 3; /**< number of locations */
    float deltaTheta = 0.13f;
	float deltaPhi = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
	float startTheta = startAngle;
    iu3dps_t parametricSource = iuf3DParametricSourceCreate(locationCountTheta, locationCountPhi, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);

    // create
    iusd_t dict = iufSourceDictCreate();

    // fill
    iufSourceDictSet(dict, sourceLabel,(ius_t) parametricSource);
    return dict;
}


iurcm_t ncFillReceiveChannelMap
(
    void
)
{
    int numChannels = 8;
    int channelMap[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int status=0,i;

    // fill
    iurcm_t receiveChannelMap = iufReceiveChannelMapCreate(numChannels);

    iufReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    // Delays
    for(i=0;i<numChannels;i++)
    {
        float delay = i*2.0f;
        status |= iufReceiveChannelMapSetStartDelay(receiveChannelMap, i, delay);
    }

    if (status != 0)
    {
        iufReceiveChannelMapDelete(receiveChannelMap);
        return IURCM_INVALID;
    }
    return receiveChannelMap;
}

iurcmd_t ncFillReceiveChannelMapDict
(
    char *label
)
{
    iurcmd_t dict = iufReceiveChannelMapDictCreate();

    // fill
    iurcm_t receiveChannelMap = ncFillReceiveChannelMap();
    iufReceiveChannelMapDictSet(dict, label, receiveChannelMap);
    return dict;
}

iua_t ncFillAcquisition()
{
    int date = 20180416;
    char *pDescription = "a nice acquisition that almost won me the nobel prize";
    float speedOfSound = 1540.0f;

    iua_t acquisition = iufAcquisitionCreate(speedOfSound,date,pDescription);
    return acquisition;
}


iut_t ncFillTransducer
(
    char *transducerName
)
{
    const int numTransducerElements = 128;
    int i = 0;

    // create and fill
    const float transducerPitch = 0.000005f;

    iu3dt_t transducer = iuf3DTransducerCreate(transducerName, IUF_PLANE, 2500000.0f, numTransducerElements);
    for (i = 0; i < numTransducerElements; i++)
    {
        iu3dp_t elemPos = iuf3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
        iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
        iuf3DTransducerSetElement(transducer, i, element);
    }

    return  (iut_t)transducer;
}

iutad_t ncFillTransmitApodizationDict
(
    char *label
)
{
    int numElements = 8;
    float apodizaton[8] = { 0.5f, 1.0f, 1.0f, 1.0f, .0f, 1.0f, .0f, 0.5f };

    iutad_t dict = iufTransmitApodizationDictCreate();

    // fill
    iuta_t transmitApodization = iufTransmitApodizationCreate(apodizaton, numElements);
    iufTransmitApodizationDictSet(dict, label, transmitApodization);

    return dict;
}





int ncInputFileNodeConfig
(
    iuif_t inputFile,
    int numFrames,
    char *transducerName,
    char *label
)
{
    char *sourceLabel = "mysource";
    char *pulseLabel = "mypulse";

    iusd_t sourceDict = ncFillSourceDict(sourceLabel);
    int status = iufInputFileSetSourceDict(inputFile, sourceDict);

    iupd_t pulseDict = ncFillPulseDict(pulseLabel);
    status |= iufInputFileSetPulseDict(inputFile, pulseDict);

    iursd_t receiveSettingsDict = ncFillReceiveSettingsDict(label);
    status |= iufInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);

    iurcmd_t receiveChannelMapDict = ncFillReceiveChannelMapDict(label);
    status |= iufInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);

    iutad_t transmitApodizationDict = ncFillTransmitApodizationDict(label);
    status |= iufInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);

    iufl_t frameList = ncFillFrameList(numFrames, label);
    status |= iufInputFileSetFrameList(inputFile,frameList);

    iupald_t patternListDict = ncFillPatternListDict(label, pulseLabel, sourceLabel, label, label);
    status |= iufInputFileSetPatternListDict(inputFile,patternListDict);

    iua_t acquisition = ncFillAcquisition();
    status |= iufInputFileSetAcquisition(inputFile, acquisition);


    iut_t transducer = ncFillTransducer(transducerName);
    status |= iufInputFileSetTransducer(inputFile, transducer);

    return status;
}
