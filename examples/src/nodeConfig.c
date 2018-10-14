//
// Created by Ruijzendaal on 16/04/2018.
//
#include <include/ius.h>



static const char *pChannelMapLabel = "channelMapLabel";



void ncFillData
(
    iud_t data,
    float value
)
{
    int i;
    float *pData = iusDataGetPointer(data);
    int numSamples = iusDataGetSize(data);

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
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status=0,i;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);

    // Delays
    for(i=0;i<numDelays;i++)
    {
        float delay = i*2.0f;
        status |= iusReceiveSettingsSetStartDelay(obj, i, delay);
    }

    return obj;
}

iursd_t ncFillReceiveSettingsDict
(
    char *label
)
{
    // Create
    iursd_t  dict = iusReceiveSettingsDictCreate();

    // Fill
    iurs_t obj = ncFillReceiveSettings();
    iusReceiveSettingsDictSet(dict,label,obj);
    return dict;
}

iupad_t ncFillParameterDict
(
    int numElements
)
{
    int elementID;
    iupad_t dict = iusParameterDictCreate();

    char generatedKey[1024];
    char generatedValue[1024];
    for (elementID=0;elementID<numElements;elementID++)
    {
        sprintf(generatedKey,"key_%d", elementID);
        sprintf(generatedValue,"value_%d", elementID);
        iusParameterDictSet(dict, generatedKey, generatedValue);
    }

    return dict;
}

iuhn_t ncFillHistoryNode
(

)
{
    char *type =  "TYPE3";
    int numParents = 0;
    iuhn_t node;

    node = iusHistoryNodeCreate(type,numParents);

    iupad_t parameterDict = ncFillParameterDict(10);
    iusHistoryNodeSetParameters(node, parameterDict);
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
    iufl_t frameList = iusFrameListCreate(numFrames);

    for (i=0;i<numFrames;i++)
    {
        obj = iusFrameCreate(label,i+2,i*0.01f);
        iusFrameListSet(frameList, obj, i);
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
    iupa_t pattern = iusPatternCreate( timeInterval,
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
    iupald_t patternListDict = iusPatternListDictCreate();
    iupal_t patternList = iusPatternListCreate(1);

    iupa_t pattern = ncFillPattern( 0.01f, pPulseLabel,
                                    pSourceLabel,
                                    pApodizationLabel,
                                    pReceivesettingsLabel);

    iusPatternListSet(patternList, pattern, 0);
    iusPatternListDictSet(patternListDict, patternListLabel, patternList);
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
    iupd_t dict = iusPulseDictCreate();

    // fill
    iupp_t parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, pulseCount);
    iusPulseDictSet(dict,pulseLabel, (iup_t) parametricPulse);
    return dict;
}

iusd_t ncFillSourceDict
(
    char *sourceLabel
)
{
    int locationCount = 5; /**< number of locations */
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    iu3dps_t parametricSource = ius3DParametricSourceCreate(locationCount, FNumber,
                                                            angularDelta, startAngle, deltaPhi, startPhi);

    // create
    iusd_t dict = iusSourceDictCreate();

    // fill
    iusSourceDictSet(dict, sourceLabel,(ius_t) parametricSource);
    return dict;
}


iurcm_t ncFillReceiveChannelMap
(
void
)
{
    int numChannels = 8;
    int channelMap[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    // fill
    iurcm_t receiveChannelMap = iusReceiveChannelMapCreate(numChannels);

    iusReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    return receiveChannelMap;
}

iurcmd_t ncFillReceiveChannelMapDict
(
    char *label
)
{
    iurcmd_t dict = iusReceiveChannelMapDictCreate();

    // fill
    iurcm_t receiveChannelMap = ncFillReceiveChannelMap();
    iusReceiveChannelMapDictSet(dict, label, receiveChannelMap);
    return dict;
}

iua_t ncFillAcquisition()
{
    int date = 20180416;
    char *pDescription = "a nice acquisition that almost won me the nobel prize";
    float speedOfSound = 1540.0f;

    iua_t acquisition = iusAcquisitionCreate(speedOfSound,date,pDescription);
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

    iu3dt_t transducer = ius3DTransducerCreate(transducerName, IUS_PLANE, 2500000.0f, numTransducerElements);
    for (i = 0; i < numTransducerElements; i++)
    {
        iu3dp_t elemPos = ius3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
        iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
        ius3DTransducerSetElement(transducer, i, element);
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

    iutad_t dict = iusTransmitApodizationDictCreate();

    // fill
    iuta_t transmitApodization = iusTransmitApodizationCreate(numElements);

    iusTransmitApodizationSetApodization(transmitApodization, apodizaton);
    iusTransmitApodizationDictSet(dict, label, transmitApodization);

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
    int status = iusInputFileSetSourceDict(inputFile, sourceDict);

    iupd_t pulseDict = ncFillPulseDict(pulseLabel);
    status |= iusInputFileSetPulseDict(inputFile, pulseDict);

    iursd_t receiveSettingsDict = ncFillReceiveSettingsDict(label);
    status |= iusInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);

    iurcmd_t receiveChannelMapDict = ncFillReceiveChannelMapDict(label);
    status |= iusInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);

    iutad_t transmitApodizationDict = ncFillTransmitApodizationDict(label);
    status |= iusInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);

    iufl_t frameList = ncFillFrameList(numFrames, label);
    status |= iusInputFileSetFrameList(inputFile,frameList);

    iupald_t patternListDict = ncFillPatternListDict(label, pulseLabel, sourceLabel, label, label);
    status |= iusInputFileSetPatternListDict(inputFile,patternListDict);

    iua_t acquisition = ncFillAcquisition();
    status |= iusInputFileSetAcquisition(inputFile, acquisition);


    iut_t transducer = ncFillTransducer(transducerName);
    status |= iusInputFileSetTransducer(inputFile, transducer);

    return status;
}
