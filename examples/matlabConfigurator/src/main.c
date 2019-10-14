#include "iuf.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iufPositionPrivate.h>

#define DEFAULT_IMAGING_DEPTH 0.010
#define PIXEL_DELTA (0.875f)
#define WINDOW_RESOLUTION_Y 480
#define WINDOW_POS_X 220
#define WINDOW_POS_Y 440
#define WINDOW_WIDTH (640+100)
#define WINDOW_HEIGHT (480+100)
#define NUM_FRAMES 20

#define SEQ_CONTROL_JUMP 1
#define SEQ_CONTROL_RETURN_TO_MATLAB 2
#define SEQ_CONTROL_TRIGGER 3
#define SEQ_CONTROL_DELAYS 4

static void printUsage(char **argv)
{
    printf("Usage: for %s:\n", argv[0]);
    printf("%s -i [inputfile.iuf] -o [outputfile.m] -l [label]\n\n", argv[0]);

    printf("[outputfile.m] is the name of the .m script to run on a verasonics Vantage. The script will create\n" \
         "a iuf file with the same header, and merges the corresponding data. [label] is the label for the dictionaries in "
           "[inputfile.iuf] that is used. Currently only 1 label is supported\n");
}

static char *writeTransducer(iut_t transducer)
{
    char *script = (char *)calloc(500, sizeof(char)); // 500 should be large enough to hold string
    sprintf(script, "%%%% System object: transducer characteristics in the Trans object\n" \
            "Trans = iusInitTrans('%s', %6.8f, %d);\n" \
            "nrTdxTx = sum(Trans.elementsTx);\n" \
            "nrTdxRx = sum(Trans.elementsRx););\n",
            iufTransducerGetName(transducer),
            iufTransducerGetCenterFrequency(transducer),
            iufTransducerGetNumElements(transducer));
    return script;
}

static char *writeResource(iut_t transducer, iua_t acquisition, double depth)
{
    char* script = (char *)calloc(5000, sizeof(char)); // 5000 characters should be enough

    // get all keys from the patternList dictionary and for each key go though its list and accumulate the transmits
    int numElements = iufTransducerGetNumElements(transducer);

    // determine rowsPerFrame
    double lambdaMm = 1000.0f * iufAcquisitionGetSpeedOfSound(acquisition) / iufTransducerGetCenterFrequency(transducer);
    IufTransducerShape shape = iufTransducerGetShape(transducer);
    double elementPitch=0.0;

    if (shape == IUF_LINE)
    {
        iu2dte_t element0 = iuf2DTransducerGetElement((iu2dt_t)transducer, 0);
        iu2dte_t element1 = iuf2DTransducerGetElement((iu2dt_t)transducer, 1);
        elementPitch = IUF_ABS(iuf2DTransducerElementGetPosition(element1)->x -
                               iuf2DTransducerElementGetPosition(element0)->x);
    }
    double D = (iufTransducerGetNumElements(transducer) - 1) * elementPitch * 1000.0; // in mm
    double rayDelta = (0.25) * asin(1.22*lambdaMm/D);
    int numRays = round(2*(M_PI/4)/rayDelta);
    double maxAqcLength = round((depth*1000.0f)/lambdaMm);
    double lineLengthRcvBuffer = 128 * ceil(maxAqcLength / 16.0);
    float pixel_delta = PIXEL_DELTA;
    int rowsPerFrame = 16 * ceil(lineLengthRcvBuffer * numRays / 16.0);
    int numColumns = 512; //ceil(2.0f*(maxAqcLength+M_PI/4.0f)*sin(-M_PI/4.0f)/pixel_delta);
    double originX = -(numColumns/2.0f)*pixel_delta;

    //int interBufferRowsPerFrame = (int)(16 * ceil(pow(2,ceil(log2(maxAqcLength)))) / 16); // verify this formula
    //int interBufferColsPerFrame = numColumns;
    const char windowTitle[] = "Generated file:";

    sprintf(script, "PData.Coord  = 'rectangular'; \n" \
                    "PData.PDelta = [Trans.spacing 0.0 0.5];  \n" \
                    "PData.Size   = [%d %d 1];   \n" \
                    "PData.Origin = [%f 0.0 0.0];  \n" \
                    "Resource.Parameters.numTransmit           = %d;\n" \
                    "Resource.Parameters.numRcvChannels        = Resource.Parameters.numTransmit;\n" \
                    "Resource.Parameters.speedOfSound          = %f;\n" \
                    "Resource.Parameters.verbose               = 2;\n" \
                    "Resource.RcvBuffer(1).datatype            = \'int16\';\n" \
                    "Resource.RcvBuffer(1).rowsPerFrame        = %d;\n" \
                    "Resource.RcvBuffer(1).colsPerFrame        = Resource.Parameters.numRcvChannels;\n" \
                    "Resource.RcvBuffer(1).numFrames           = %d; %% for cineloop\n" \
                    "Resource.InterBuffer(1).numFrames         = 1;\n"\
                    "Resource.ImageBuffer(1).numFrames         = 1;\n" \
                    "Resource.DisplayWindow(1).Title           = [%s, ...\n" \
                    " ' (', %d,' active elements)', ...\n"\
                    " ', max ', %f,' cm', ...\n" \
                    " ', Fc = ', %f, ' MHz', ...\n" \
                    " ', numRays = ', %d];\n" \
                    " Resource.DisplayWindow(1).pdelta         = %f;\n" \
                    " Resource.DisplayWindow(1).AxesUnits      = 'mm';\n" \
                    " Resource.DisplayWindow(1).Position       = [%d,%d,%d,%d];\n" \
                    " Resource.DisplayWindow(1).ReferencePt    = [PData.Origin(1),PData.Origin(3)];\n",
                    rowsPerFrame, iufTransducerGetNumElements(transducer), // PData size z and x
                    originX, //PData origin
                    iufTransducerGetNumElements(transducer), // Parameters.numTransmit
                    iufAcquisitionGetSpeedOfSound(acquisition), // Parameters.speedOfSound
                    rowsPerFrame,            //RcvBuffer(1).rowsPerFrame
                    NUM_FRAMES,              //RcvBuffer.numFrames
                    windowTitle, numElements, DEFAULT_IMAGING_DEPTH,              // window info
                    iufTransducerGetCenterFrequency(transducer), numRays, // window info
                    maxAqcLength / (double)WINDOW_RESOLUTION_Y,          // displayWindow.pdelta
                    WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
    return script;
}

static char *writeTx(iupal_t patternList,
                     iut_t transducer,
                     iutad_t apodizationDict,
                     iusd_t sourceDict)
{
    char *script = (char *) calloc(50000, sizeof(char));
    char *elementString = (char *) calloc(50, sizeof(char));
    char *apodizationString = (char *) calloc(5000, sizeof(char));
    char *txString = (char *) calloc(500, sizeof(char));

    double unitScale = iufTransducerGetCenterFrequency(transducer)/1540.0;
    int numTransmits = iufPatternListGetSize(patternList);
    double transducerWidth = 0.0;
    int numElements = iufTransducerGetNumElements(transducer);

    IufTransducerShape transShape = iufTransducerGetShape(transducer);
    if (transShape == IUF_LINE)
    {
        iu2dp_t posFirst, posLast;
        iu2dte_t elem2DFirst = iuf2DTransducerGetElement((iu2dt_t) transducer, 0);
        iu2dte_t elem2DLast = iuf2DTransducerGetElement((iu2dt_t) transducer, numElements - 1);

        posFirst = iuf2DTransducerElementGetPosition(elem2DFirst);
        posLast = iuf2DTransducerElementGetPosition(elem2DLast);
        transducerWidth = fabsf(posFirst->x - posLast->x);
    }
    else
    {
        return "%% writeTX error: only 2D linear transducers are currently supported.\n error=1;\n";
    }

    sprintf(script, "%%%% Specify TX structure array.\n" \
                    "scaleToWvl = %f;\n", unitScale);

    for (int t=0; t < numTransmits; t++)
    {
        iupa_t pattern = iufPatternListGet(patternList, t);
        iuta_t apodization = iufTransmitApodizationDictGet(apodizationDict,
                                                           (char *)iufPatternGetApodizationLabel(pattern));
        for (int i = 0; i < numElements; i++)
        {
            sprintf(elementString, "%f%c", iufTransmitApodizationGetElement(apodization,i),i==numElements-1?' ':',');
            strcat(apodizationString, elementString);
        }
        ius_t source = iufSourceDictGet(sourceDict, (char *)iufPatternGetSourceLabel(pattern));
        IufSourceType sourceType = iufSourceGetType(source);

        // iu2dp_t pos2d;
        // iu3dp_t pos3d;
        double focus = 50; // focus value is 50 mm
        double theta=0.0, deltaTheta=0.0, fNumber=0.0;
        int numSources;

        switch (sourceType)
        {
            case IUF_2D_NON_PARAMETRIC_SOURCE:
                // pos2d = iuf2DNonParametricSourceGetPosition((iu2dnps_t)source, 0);
                return "%% writeTX error: non-parametric 2D source currently not implemented.\n error=1;\n";
                // break;
            case IUF_2D_PARAMETRIC_SOURCE:
                numSources = iuf2DParametricSourceGetNumLocations((iu2dps_t)source);
                theta = iuf2DParametricSourceGetStartTheta((iu2dps_t)source);
                fNumber = iuf2DParametricSourceGetFNumber((iu2dps_t)source);
                deltaTheta = iuf2DParametricSourceGetDeltaTheta((iu2dps_t)source);
                for (int i=0; i< numSources; i++)
                {
                    // we assume transmit t matches source t%numSources
                    focus = IUF_ABS(fNumber) < IUF_FLT_EPS ? 0.0 : transducerWidth / fNumber;
                    int txCount = t*numSources+i;
                    sprintf(txString, "TX(%d).Apod   = [%s];\n" \
                                      "TX(%d).Origin = [%f 0.0 0.0] * scaleToWvl;\n" \
                                      "TX(%d).focus  = %f * scaleToWvl;\n" \
                                      "TX(%d).Steer  = [%f, 0];\n" \
                                      "TX(%d).Delay  = computeTXDelays(TX(%d));\n",
                                      txCount, apodizationString,
                                      txCount, transducerWidth * tan(theta),
                                      txCount, focus,
                                      txCount, theta+i*deltaTheta,
                                      txCount, txCount);
                    strcat(script, txString);
                }
                break;
            case IUF_3D_NON_PARAMETRIC_SOURCE:
                // pos3d = iuf3DNonParametricSourceGetPosition((iu3dnps_t)source, 0);
                return "%% writeTX error: non-parametric 3D source currently not implemented.\n error=1;\n";
            case IUF_3D_PARAMETRIC_SOURCE:
                //theta = iuf3DParametricSourceGetStartTheta((iu3dps_t)source);
                //phi = iuf3DParametricSourceGetStartPhi((iu3dps_t)source);
                //fNumber = iuf3DParametricSourceGetFNumber((iu3dps_t)source);
                return "%% writeTX error: parametric 3D source currently not implemented.\n error=1;\n";
            default:
                return "%% writeTX error: unknown sourceType.\nerror=1; \n";
        }
#if 0
        focus = IUF_ABS(fNumber) < IUF_FLT_EPS ? 0.0 : transducerWidth / fNumber;
        sprintf(txString, "TX(%d).Apod = [%s];\n" \
                       "TX(%d).Origin = [%f 0.0 0.0] * scaleToWvl;\n " \
                       "TX(%d).focus  = %f * scaleToWvl;\n " \
                       "TX(%d).Steer  = [%f, 0];\n" \
                       "TX(%d).Delay  = computeTXDelays(TX(%d));\n;",
                       t, apodizationString,
                       t, transducerWidth * tan(theta),
                       t, focus,
                       t, theta,
                       t, t);
        strcat(script, txString);
#endif
    }
    free(elementString);
    free(apodizationString);
    free(txString);
    return script;
}

static char *writeRx(iupal_t patternList,
                     iursd_t receiveSettingsDict,
                     int numElements,
                     double depth)
{
    int numReceives = iufPatternListGetSize(patternList);
    char *receive = (char *)calloc(5000, sizeof(char));
    char *script = (char *)calloc(50000, sizeof(char));

    char **receiveSettingsKeys = iufReceiveSettingsDictGetKeys(receiveSettingsDict);
    int numKeys = (int)iufReceiveSettingsDictGetSize(receiveSettingsDict);
    int t=0;
    for (int f=0; f<NUM_FRAMES; f++)
    {
        for (int i=0; i<numReceives ; i++)
        {
            int k=0;
            iupa_t pattern = iufPatternListGet(patternList, i);
            const char *receiveSettingsLabel = iufPatternGetReceivesettingsLabel(pattern);
            for (k=0; k <numKeys; k++)
            {
                if (!strcmp(receiveSettingsKeys[k], receiveSettingsLabel)) break; //breaks if k is the key index
            }

            sprintf(receive, "Receive(%d).Apod =  ones(1,%d);\n"       \
                           "Receive(%d).mode = 0;\n"       \
                           "Receive(%d).startDepth = 0;\n" \
                           "Receive(%d).endDepth = %f;\n"    \
                           "Receive(%d).TGC = %d;\n"         \
                           "Receive(%d).bufnum = 1;\n"      \
                           "Receive(%d).framenum = %d;\n"    \
                           "Receive(%d).acqNum = %d;\n"      \
                           "Receive(%d).sampleMode = 1;\n"  \
                           "Receive(%d).LowPassCoef = lowPassCoeff;\n" \
                           "Receive(%d).InputFilter = inputFilter;\n",
                           t, numElements,  // Apodization length
                           t, t, t, depth,  // endDepth
                           t, k+1,          // +1 is Matlab offset, tgc is the receivesettings/tgc key index
                           t,               // bufnum
                           t, f+1,          // framenum
                           t, i,             // item in the patternList
                           t,               // lowpass filter
                           t, t);           // input filter
            t++;
            strcat(script, receive);
        }
    }
    return script;
}

static char *writeTw(iupd_t pulseDict)
{
    char *script = (char *) calloc(5000, sizeof(char));

    sprintf(script, "%%%% Specify TW structure array.\n");
    char **keys = iufPulseDictGetKeys(pulseDict);
    for (size_t i=0; i < iufPulseDictGetSize(pulseDict); i++)
    {
        iup_t pulse = iufPulseDictGet(pulseDict, keys[i]);
        IufPulseType pulseType = iufPulseGetType(pulse);
        if (pulseType == IUF_PARAMETRIC_PULSETYPE)
        {
            sprintf(script, "TW(%d) = iusComputeTW(%f,%d);\n",
                    (int)i,
                    iufParametricPulseGetFrequency((iupp_t)pulse),
                    iufParametricPulseGetNumPulses((iupp_t)pulse));
        }
        else
        {
            sprintf(script, "%% Non-parametric pulse currently not supported;\n error=1;\n");
        }
    }
    return script;
}

static char *writeTGC(iursd_t receiveSettingsDict, double maxDepthWL)
{
    char *script = (char *) calloc(5000, sizeof(char));

    sprintf(script, "%%%% Specify TGC structure array.\n");
    char **keys = iufReceiveSettingsDictGetKeys(receiveSettingsDict);
    for (size_t i=0; i < iufReceiveSettingsDictGetSize(receiveSettingsDict); i++)
    {
        iutgc_t tgc = iufReceiveSettingsGetTGC(iufReceiveSettingsDictGet(receiveSettingsDict,keys[i]));
        if (iufTGCGetNumValues(tgc) > 1)
        {
            sprintf(script, "%%%% Currently only a default TGC is supported.\n");
        }
        sprintf(script, "TGC(%d).CntrlPts = iusgTGCSetPoints;\n" \
                        "TGC(%d).rangeMax = %f ;\n" \
                        "TGC(%d).Waveform = computeTGCWaveform(TGC);\n",
                        (int)i,(int)i, maxDepthWL, (int)i);
    }
    return script;
}

static char *writeRecon(iupal_t patternList)
{
    int numPatterns = iufPatternListGetSize(patternList);
    char *script = (char *) calloc(8000, sizeof(char));
    char *reconInfoScript = (char *) calloc(800, sizeof(char));

    sprintf(script, "%% Specify Recon structure array \n" \
                   "Recon = struct('senscutoff',  0.0, ...\n" \
                   "'pdatanum',    1, ...\n" \
                   "'rcvBufFrame', -1, ... \n" \
                   "'IntBufDest',  [1,1], ... \n" \
                   "'ImgBufDest',  [1,-1], ... \n" \
                   "'RINums',      1:%d);\n\n" \
                   "%% Define ReconInfo structures.\n", numPatterns);
    for (int i=0; i<numPatterns; i++)
    {
        sprintf(reconInfoScript, "ReconInfo(%d).mode = 0;\n" \
                               "ReconInfo(%d).txnum = %d;\n" \
                               "ReconInfo(%d).rcvnum = %d;\n" \
                               "ReconInfo(%d).regionnum = %d;\n",
                i,
                i, i + 1,  // txnum
                i, i + 1,  // rcvnum
                i, i + 1); // regionnum
    }
    strcat(script, reconInfoScript);
    return script;
}

static char *writeProcess(void)
{
    char *script = (char *) calloc(8000, sizeof(char));
    sprintf(script, "%% Specify Process structure array \n" \
                    "Process(1).classname = 'Image';\n" \
                    "Process(1).method = 'imageDisplay';\n" \
                    "Process(1).Parameters = {'imgbufnum', 1, ...    %% number of buffer to process.\n" \
                    "    'framenum', -1, ...   %% (-1 => lastFrame)\n" \
                    "    'pdatanum', 1, ...    %% number of PData structure to use\n" \
                    "    'norm', 1, ...        %% normalization method(1 means fixed)\n" \
                    "    'pgain', 10.0, ...    %% pgain is image processing gain\n" \
                    "    'persistMethod', 'none', ...\n" \
                    "    'persistLevel', 0, ...\n" \
                    "    'interp', 1, ...        %% method of interpolation (1=4pt interp)\n" \
                    "    'compression', 0.5, ... %% X^0.5 normalized to output word size\n" \
                    "    'reject', 0, ...\n" \
                    "    'mappingMode', 'full', ...\n" \
                    "    'display', 1, ...     %% display image after processing\n" \
                    "    'displayWindow', 1};\n"
                    "Process(2).classname = 'External';\n" \
                    "Process(2).method = 'ManagePlot';\n" \
                    "Process(2).Parameters = {'srcbuffer', 'receive', ...\n" \
                    "    'srcbufnum', 1, ... %% num buffers to process.\n" \
                    "    'srcframenum', -1, ...  %% process the most recent frame.\n" \
                    "    'dstbuffer', 'none'};\n" \
                    "EF(1).Function = text2cell('%%EF#1');  %% Call an external function defined below\n");
    return script;
}

static char *writeEvents(iufl_t frameList, iupal_t patternList)
{
    char *script = (char *) calloc(80000, sizeof(char));
    char *scriptLine = (char *) calloc(800, sizeof(char));
    int numFrames = iufFrameListGetSize(frameList);
    int numEventsPerFrame = iufPatternListGetSize(patternList);
    int numEvents=0;
    int n = SEQ_CONTROL_DELAYS; // the index to start the TimeToNextAcq
    double dt = 0.0;
    double prevRelativeTimeInFrame = 0.0;

    /* Fixed sequence control */
    sprintf(scriptLine, "seqControl(%d).command = 'jump'\n", SEQ_CONTROL_JUMP);
    strcat(script, scriptLine);
    sprintf(scriptLine, "seqControl(%d).argument = 1\n", SEQ_CONTROL_JUMP);
    strcat(script, scriptLine);
    sprintf(scriptLine, "seqControl(%d).command = 'returnToMatlab'\n", SEQ_CONTROL_RETURN_TO_MATLAB);
    strcat(script, scriptLine);
    sprintf(scriptLine, "seqControl(%d).command = 'triggerOut'\n", SEQ_CONTROL_TRIGGER);
    strcat(script, scriptLine);

    /* write all delays between pulses as individual sequence controls */
    for (int f=0; f < numFrames; f++)
    {
        iufr_t frame = iufFrameListGet(frameList, f);
        double frameTime = iufFrameGetTime(frame);
        prevRelativeTimeInFrame = frameTime;
        for (int e = 0; e < numEventsPerFrame; e++, n++) {
            iupa_t pattern = iufPatternListGet(patternList, e);
            double curRelativeTimeInFrame = iufPatternGetTimeInFrame(pattern);
            dt = curRelativeTimeInFrame - prevRelativeTimeInFrame;
            sprintf(scriptLine, "seqControl(%d).command = timeToNextAcq\n", n + 1);
            strcat(script, scriptLine);
            sprintf(scriptLine, "seqControl(%d).argument = %f\n", n + 1, dt);
            strcat(script, scriptLine);
            sprintf(scriptLine, "seqControl(%d).condition = 'ignore'\n", n + 1);
            strcat(script, scriptLine);
            prevRelativeTimeInFrame = curRelativeTimeInFrame;
            n++;
        }
    }

    strcat(script, "for i = 1 : Resource.RcvBuffer(1).numFrames\n");
    for (int e = 0; e < numEventsPerFrame; e++)
    {
        sprintf(scriptLine, "\tfor j = 1 : %d\n", numEvents); strcat(script, scriptLine);
        strcat(script, "\t\tEvent(n).info       = 'Acquire B-mode ray line';\n");
        strcat(script, "\t\tEvent(n).tx         = j;\n");
        sprintf(scriptLine, "\t\tEvent(n).rcv        = %d*(i-1) + j;\n", numEvents); strcat(script, scriptLine);
        strcat(script, "\t\tEvent(n).recon      = 0;\n");
        strcat(script, "\t\tEvent(n).process    = 0;\n");
        sprintf(script, "\t\tEvent(n).seqControl = [%d,%d];\n", e*(numFrames-1)+e-1, SEQ_CONTROL_TRIGGER);
        strcat(script, "\tend\n");
    }
    strcat(script, "end\n");
    return script;
}

static char *parseIuf(iuif_t iuf, char *label, double depth)
{
    char *script = (char *) calloc(80000, sizeof(char));
    sprintf(script, "%% Script generated from file:\n");

    if (iuf)
    {
        iut_t transducer = iufInputFileGetTransducer(iuf);
        iupal_t patternList = iufPatternListDictGet(iufInputFileGetPatternListDict(iuf), label);
        iua_t acquisition = iufInputFileGetAcquisition(iuf);
        iursd_t receiveSettingsDict = iufInputFileGetReceiveSettingsDict(iuf);
        iupd_t pulseDict = iufInputFileGetPulseDict(iuf);
        iutad_t apodizationDict = iufInputFileGetTransmitApodizationDict(iuf);
        iusd_t sourcesDict = iufInputFileGetSourceDict(iuf);
        iufl_t frameList = iufInputFileGetFrameList(iuf);

        double lambdaMm = iufAcquisitionGetSpeedOfSound(acquisition) / iufTransducerGetCenterFrequency(transducer);
        double depthWL = depth/lambdaMm;
        strcat(script, writeTransducer(transducer));
        strcat(script, writeResource(transducer, acquisition, depth));
        strcat(script, writeTx(patternList, transducer, apodizationDict, sourcesDict));
        strcat(script, writeTw(pulseDict));
        strcat(script, writeTGC(receiveSettingsDict, depthWL));
        strcat(script, writeRx(patternList, receiveSettingsDict, iufTransducerGetNumElements(transducer), depthWL));
        strcat(script, writeRecon(patternList));
        strcat(script, writeProcess()); //processing of data is currently a fixed 2 item array
        strcat(script, writeEvents(frameList, patternList));
    }
    else
    {
        return "";
    }

    return script;
}

int main(int argc, char *argv[])
{
    int version = iufGetVersionMajor();
    int opt;
    char *veraScript;
    FILE *outputFile;
    iuif_t iuf;
    char *label;
    double imaging_depth = DEFAULT_IMAGING_DEPTH;

    while((opt = getopt(argc, argv, ":i:o:l:d")) != -1)
    {
        switch(opt)
        {
            case 'i':
                printf("inputFile (%s): %d\n", optarg, version);
                iuf = iufInputFileNodeLoad(optarg);
                break;
            case 'o':
                printf("output: %s\n", optarg);
                outputFile = fopen(optarg, "w");
                break;
            case 'l':
                printf("label: %s\n", optarg);
                label = optarg;
                break;
            case 'd':
                printf("depth: %s\n", optarg);
                imaging_depth = atoi(optarg);
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    if (!iuf)
    {
        printUsage(argv);
        exit(1);
    }

    veraScript = parseIuf(iuf, label, imaging_depth);
    fprintf(outputFile, "%s", veraScript);
    fclose(outputFile);

    return 0;
}
