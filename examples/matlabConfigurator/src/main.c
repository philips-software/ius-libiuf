#include "iuf.h"
#include <stdio.h>
#include "getopt.h"
#include <string.h>
#include <math.h>
#include <iufPositionPrivate.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define DEFAULT_IMAGING_DEPTH 0.010
#define PIXEL_DELTA (0.875f)
#define WINDOW_RESOLUTION_Y 480
#define WINDOW_POS_X 220
#define WINDOW_POS_Y 440
#define WINDOW_WIDTH (640+100)
#define WINDOW_HEIGHT (480+100)

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

// write transducer initialization function
static char *writeTransducer(iut_t transducer)
{
    char *script = (char *)calloc(500, sizeof(char)); // 500 should be large enough to hold string
    sprintf(script, "%%%% System object: transducer characteristics in the Trans object\n" \
            "Trans = iusInitTrans('%s');\n" \
            "nrTdxTx = sum(Trans.elementsTx);\n" \
            "nrTdxRx = sum(Trans.elementsRx);\n",
            iufTransducerGetName(transducer));
            //iufTransducerGetCenterFrequency(transducer), it is a Verasonics specific?
            //iufTransducerGetNumElements(transducer));
    return script;
}

// Write the Resource struct.
//   - It uses a PData object that describes the area and 'density' of the image reconstruction
//   - It contains Parameters: {numTransmits, numRcvChannels, speedOfSound, verbose }
//                 RcvBuffer: {type, rows, cols, numFrames}
//                 InterBuffer: {numFrames}
//                 ImageBuffer: {numFrames}
//                 DisplayWindow: {Title, pdelta, AxesUnits, Position, ReferencePt}
//  These values can be determined from the iufTransducer, iufAcquisition , depth and numFrames
//
static char *writeResource(iut_t transducer, iua_t acquisition, double depth, int numFrames)
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
    int numRays = (int)round(2.0*(M_PI/4.0)/rayDelta);
    double maxAqcLength = round((depth*1000.0f)/lambdaMm);
    double lineLengthRcvBuffer = 128 * ceil(maxAqcLength / 16.0);
    float pixel_delta = PIXEL_DELTA;
    int rowsPerFrame = (int)(16.0 * ceil(lineLengthRcvBuffer * numRays / 16.0));
    int numColumns = 512; //ceil(2.0f*(maxAqcLength+M_PI/4.0f)*sin(-M_PI/4.0f)/pixel_delta);
    double originX = -(numColumns/2.0f)*pixel_delta;

    //int interBufferRowsPerFrame = (int)(16 * ceil(pow(2,ceil(log2(maxAqcLength)))) / 16); // verify this formula
    //int interBufferColsPerFrame = numColumns;
    const char windowTitle[] = "Generated file:";

    sprintf(script, "PData.Coord  = 'rectangular'; \n" \
                    "PData.PDelta = [Trans.spacing 0.0 0.5];  \n" \
                    "PData.Size   = [%d %d 1];   \n" \
                    "PData.Origin = [%f 0.0 0.0];  \n" \
                    "Resource.Parameters.simulateMode          = 0;\n" \
                    "Resource.Parameters.numTransmit           = 128;\n" \
                    "Resource.Parameters.numRcvChannels        = Resource.Parameters.numTransmit;\n" \
                    "Resource.Parameters.speedOfSound          = %f;\n" \
                    "Resource.Parameters.verbose               = 2;\n" \
                    "Resource.RcvBuffer(1).datatype            = \'int16\';\n" \
                    "Resource.RcvBuffer(1).rowsPerFrame        = %d;\n" \
                    "Resource.RcvBuffer(1).colsPerFrame        = Resource.Parameters.numRcvChannels;\n" \
                    "Resource.RcvBuffer(1).numFrames           = %d; %% for cineloop\n" \
                    "Resource.InterBuffer(1).numFrames         = 1;\n"\
                    "Resource.ImageBuffer(1).numFrames         = 1;\n" \
                    "Resource.DisplayWindow(1).Title           = ['%s', ...\n" \
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
                    //iufTransducerGetNumElements(transducer), // Parameters.numTransmit
                    iufAcquisitionGetSpeedOfSound(acquisition), // Parameters.speedOfSound
                    rowsPerFrame,            //RcvBuffer(1).rowsPerFrame
                    numFrames,              //RcvBuffer.numFrames
                    windowTitle, numElements, DEFAULT_IMAGING_DEPTH,              // window info
                    iufTransducerGetCenterFrequency(transducer), numRays, // window info
                    maxAqcLength / (double)WINDOW_RESOLUTION_Y,          // displayWindow.pdelta
                    WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
    return script;
}

// Writes the TX structure from the file info:
// a TX structure sets the transmit delays of the transducer array
// This is done by setting TX(n).delay using the Verasonic's helper function computeTXDelays
// that takes TX.focus, TX.steer and TX.origin as inputs.
// In addition a transmit apodization is set.
// For each pattern in the patternList a TX is written, using the specific source and apodization.

//TODO: FIX data generator to create a Verasonics pattern: for each parametric source item a pattern
static char *writeTx(iupal_t patternList,
                     iut_t transducer,
                     iutad_t apodizationDict,
                     iusd_t sourceDict)
{
    char *script = (char *) calloc(500000, sizeof(char));
    char *elementString = (char *) calloc(50, sizeof(char));
    char *apodizationString;
    char *txString = (char *) calloc(9000, sizeof(char));

    double unitScale = iufTransducerGetCenterFrequency(transducer)/1540.0;
    int numTransmits = iufPatternListGetSize(patternList);
    double transducerWidth = 0.0;

    IufTransducerShape transShape = iufTransducerGetShape(transducer);
    int numElements = iufTransducerGetNumElements(transducer); 
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
        apodizationString= (char *) calloc(5000, sizeof(char));
        for (int i = 0; i < numElements; i++)
        {
            sprintf(elementString, "%f%c", iufTransmitApodizationGetElement(apodization,i),i==numElements-1?' ':',');
            strcat(apodizationString, elementString);
        }
        fprintf(stderr, "%s\n\n", apodizationString);

        ius_t source = iufSourceDictGet(sourceDict, (char *)iufPatternGetSourceLabel(pattern));
        IufSourceType sourceType = iufSourceGetType(source);
        // iu2dp_t pos2d;
        // iu3dp_t pos3d;
        double focus = 50; // focus value is 50 mm
        double theta=0.0, deltaTheta=0.0, fNumber=0.0;
        int numSources;

        fprintf(stderr, "sourceType = %d\n", (int)sourceType);
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
                fprintf(stderr, "numSources=%d\n", numSources);
                fprintf(stderr, "fNumber=%f\n", fNumber);
                for (int i=0; i< numSources; i++)
                {
                    // we assume transmit t matches source t%numSources
                    focus = IUF_ABS(fNumber) < IUF_FLT_EPS ? 0.0 : transducerWidth / fNumber;
                    int txCount = t*numSources+i+1;
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
                    fprintf(stderr, "%s", txString);
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
        free(apodizationString);
    }
    free(elementString);
    free(txString);
    return script;
}

// The Receive structure can not be directly related to a iufReceiveSettings.
// For each frame and each transmit in the frame a Receive structure is written.
// Receive.Apod is not defined in an inputFile... Can be an input parameter to this exec.
// Receive.mode ???
// Receive.startDepth : currently assumed 0
// Receive.endDepth: in Wavelengths?
// Receive.TGC: 1 //currently not used
// Receive.bufnum = 1; //simple bmode only
// Receive.framenum: frame number
// Receive.acqnum: pattern index in frame
// Receive.sampleMode = 1; //? keep at 1
// Receive.lowPassCoef and Receive.inputFilter are considered constant
static char *writeRx(iupal_t patternList,
                     int numElements,
                     double depth,
                     int numFrames)
{
    int numReceives = iufPatternListGetSize(patternList);
    char *receive = (char *)calloc(5000, sizeof(char));
    char *script = (char *)calloc(50000, sizeof(char));

    int t=1;

    strcat(script, "lowPassCoeff = [0 0 0 0 0 0 0 0 0 0 0 1];\n" \
        "inputFilter = firpm(40,[0 0.02 0.22 0.78 0.98 1],[0 0 1 1 0 0]);\n" \
        "inputFilter = inputFilter(1:21);\n");

    for (int f=0; f<numFrames; f++)
    {
        for (int i=0; i<numReceives ; i++)
        {
            sprintf(receive, "Receive(%d).Apod =  ones(1,%d);\n" \
                             "Receive(%d).mode = 0;\n"           \
                             "Receive(%d).startDepth = 0;\n"     \
                             "Receive(%d).endDepth = %f;\n"      \
                             "Receive(%d).TGC = %d;\n"           \
                             "Receive(%d).bufnum = 1;\n"         \
                             "Receive(%d).framenum = %d;\n"      \
                             "Receive(%d).acqNum = %d;\n"        \
                             "Receive(%d).sampleMode = 1;\n"     \
                             "Receive(%d).LowPassCoef = lowPassCoeff;\n" \
                             "Receive(%d).InputFilter = inputFilter;\n",
                             t, numElements,  // Apodization length
                             t, t, t, depth,  // endDepth
                             t, f+1,          // +1 is Matlab offset, tgc is the receivesettings/tgc key index
                             t,               // bufnum
                             t, f+1,          // framenum
                             t, i+1,          // item in the patternList
                             t,               // lowpass filter
                             t, t);           // input filter
            t++;
            strcat(script, receive);
        }
    }
    return script;
}

// The TW is derived from pulse
static char *writeTw(iup_t pulse)
{
    char *script = (char *) calloc(5000, sizeof(char));

    sprintf(script, "%%%% Specify TW structure array.\n");

    IufPulseType pulseType = iufPulseGetType(pulse);
    if (pulseType == IUF_PARAMETRIC_PULSETYPE)
    {
      sprintf(script, "TW(1) = iusComputeTW(%d,%d);\n",
              (int)(iufParametricPulseGetFrequency((iupp_t)pulse)/1000000.0f), //in MHz instead of Hz
                    iufParametricPulseGetNumPulses((iupp_t)pulse));
    }
    else
    {
       sprintf(script, "%% Non-parametric pulse currently not supported;\n error=1;\n");
    }
    return script;
}

// TGC is not worked out in detail yet (TGC(1) = 1)
static char *writeTGC(iupal_t patternList, iursd_t receiveSettingsDict, double maxDepthWL)
{
    char *script = (char *) calloc(5000, sizeof(char));
    char *tgcScript = (char *) calloc(500, sizeof(char));
    int numPatterns = iufPatternListGetSize(patternList);

    sprintf(script, "%%%% Specify TGC structure array.\n");
    for (int p=0; p<numPatterns; p++) //for all patterns
    {
        iupa_t pattern = iufPatternListGet(patternList, p);
        char *receiveSettingsLabel = (char *)iufPatternGetReceivesettingsLabel(pattern);
        iutgc_t tgc = iufReceiveSettingsGetTGC(iufReceiveSettingsDictGet(receiveSettingsDict, receiveSettingsLabel));
        if (iufTGCGetNumValues(tgc) > 1)
        {
            sprintf(tgcScript, "%%%% Currently only a default TGC is supported.\n");
        }
        sprintf(tgcScript, "iusgTGCSetPoints = 512 * ones(1,8);\n" \
                           "TGC(%d).CntrlPts = iusgTGCSetPoints;\n" \
                           "TGC(%d).rangeMax = %f ;\n" \
                           "TGC(%d).Waveform = computeTGCWaveform(TGC(%d));\n",
                           (int)p+1,(int)p+1, maxDepthWL, (int)p+1, (int)p+1);
        strcat(script, tgcScript);
    }
    return script;
}

static char *writeRecon(iupal_t patternList)
{
    int numPatterns = iufPatternListGetSize(patternList);
    char *script = (char *) calloc(8000, sizeof(char));
    char *reconInfoScript = (char *) calloc(800, sizeof(char));
    int i;

    sprintf(script, "%% Specify Recon structure array \n" \
                   "Recon = struct('senscutoff',  0.0, ...\n" \
                   "'pdatanum',    1, ...\n" \
                   "'rcvBufFrame', -1, ... \n" \
                   "'IntBufDest',  [1,1], ... \n" \
                   "'ImgBufDest',  [1,-1], ... \n" \
                   "'RINums',      1:%d);\n\n" \
                   "%% Define ReconInfo structures.\n", numPatterns);
    for (i=0; i<numPatterns; i++)
    {
        sprintf(reconInfoScript, "ReconInfo(%d).mode = 0;\n" \
                               "ReconInfo(%d).txnum = %d;\n" \
                               "ReconInfo(%d).rcvnum = %d;\n" \
                               "ReconInfo(%d).regionnum = %d;\n",
                i + 1,
                i + 1, i + 1,  // txnum
                i + 1, i + 1,  // rcvnum
                i + 1, i + 1); // regionnum
        strcat(script, reconInfoScript);
    }
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
                    "EF(1).Function = text2cell('%%EF#1');  %% Call an external function defined below\n" \
                    "%%EF#1\n" \
                    "aap=0;\n" \
                    "%%EF#1\n");
    return script;
}

static char *writeEvents(iufl_t frameList, iupal_t patternList, char * label)
{
    char *script = (char *) calloc(80000, sizeof(char));
    char *scriptLine = (char *) calloc(800, sizeof(char));
    int numFrames = iufFrameListGetSize(frameList);

    //int numEvents=0;
    int n = SEQ_CONTROL_DELAYS; // the index to start the TimeToNextAcq
    int eventNum = 1;
    double dt = 0.0;
    double prevRelativeTimeInFrame = 0.0;

    /* Fixed sequence control */
    sprintf(scriptLine, "seqControl(%d).command = 'jump';\n", SEQ_CONTROL_JUMP);
    strcat(script, scriptLine);
    sprintf(scriptLine, "seqControl(%d).argument = 1;\n", SEQ_CONTROL_JUMP);
    strcat(script, scriptLine);
    sprintf(scriptLine, "seqControl(%d).command = 'returnToMatlab';\n", SEQ_CONTROL_RETURN_TO_MATLAB);
    strcat(script, scriptLine);
    sprintf(scriptLine, "seqControl(%d).command = 'triggerOut';\n", SEQ_CONTROL_TRIGGER);
    strcat(script, scriptLine);

    /* write all delays between pulses as individual sequence controls */
    for (int f=0; f < numFrames; f++)
    {
        iufr_t frame = iufFrameListGet(frameList, f);
        if (!strcmp(iufFrameGetPatternListLabel(frame), label))  //it is frame with the label matching the patternList
        {
            int numEventsPerFrame = iufPatternListGetSize(patternList);
            prevRelativeTimeInFrame = iufFrameGetTime(frame);
            for (int e = 0; e < numEventsPerFrame; e++, n++, eventNum++) {
                iupa_t pattern = iufPatternListGet(patternList, e);
                double curRelativeTimeInFrame = iufPatternGetTimeInFrame(pattern);
                dt = curRelativeTimeInFrame - prevRelativeTimeInFrame;
                sprintf(scriptLine, "seqControl(%d).command = 'timeToNextAcq';\n", n);
                strcat(script, scriptLine);
                sprintf(scriptLine, "seqControl(%d).argument = %f;\n", n, dt);
                strcat(script, scriptLine);
                sprintf(scriptLine, "seqControl(%d).condition = 'ignore';\n", n);
                strcat(script, scriptLine);
                sprintf(scriptLine, "Event(%d).info       = 'Acquire B-mode ray line';\n", eventNum);
                strcat(script, scriptLine);
                sprintf(scriptLine, "Event(%d).tx         = %d;\n", eventNum, e);
                strcat(script, scriptLine);
                sprintf(scriptLine, "Event(%d).rcv        = %d;\n", eventNum, e);
                strcat(script, scriptLine);
                sprintf(scriptLine, "Event(%d).recon      = %d;\n", eventNum, f+1);
                strcat(script, scriptLine);
                sprintf(scriptLine, "Event(%d).process    = 1;\n", eventNum);
                strcat(script, scriptLine);
                sprintf(scriptLine, "Event(%d).seqControl = [%d,%d];\n", eventNum, n, SEQ_CONTROL_TRIGGER);
                strcat(script, scriptLine);
                prevRelativeTimeInFrame = curRelativeTimeInFrame;
            }
        }
    }
    sprintf(scriptLine, "Event(%d).info       = 'Jump back to first event';\n", eventNum);
    strcat(script, scriptLine);
    sprintf(scriptLine, "Event(%d).tx         = 0;\n", eventNum);
    strcat(script, scriptLine);
    sprintf(scriptLine, "Event(%d).rcv        = 0;\n", eventNum);
    strcat(script, scriptLine);
    sprintf(scriptLine, "Event(%d).recon      = 0;\n", eventNum);
    strcat(script, scriptLine);
    sprintf(scriptLine, "Event(%d).process    = 0;\n", eventNum);
    strcat(script, scriptLine);
    sprintf(scriptLine, "Event(%d).seqControl = [%d, %d];\n", eventNum, SEQ_CONTROL_JUMP, SEQ_CONTROL_RETURN_TO_MATLAB);
    strcat(script, scriptLine);

    return script;
}

static char *writeFooter(char *fileName)
{
    char *script = (char *) calloc(12000, sizeof(char));
    sprintf(script, "filename = %s;\n" \
                    "VSX\n",
                    fileName);
    return script;
}

static char *parseIuf(iuif_t iuf, char *label, double depth)
{
    char *script = (char *) calloc(120000, sizeof(char));
    sprintf(script, "%% Script generated from file:\n");

    if (iuf)
    {
        iut_t transducer = iufInputFileGetTransducer(iuf);
        iupal_t patternList = iufPatternListDictGet(iufInputFileGetPatternListDict(iuf), label);
        iua_t acquisition = iufInputFileGetAcquisition(iuf);
        iursd_t receiveSettingsDict = iufInputFileGetReceiveSettingsDict(iuf);
        iup_t pulse = iufPulseDictGet(iufInputFileGetPulseDict(iuf), label);
        iutad_t apodizationDict = iufInputFileGetTransmitApodizationDict(iuf);
        iusd_t sourcesDict = iufInputFileGetSourceDict(iuf);
        iufl_t frameList = iufInputFileGetFrameList(iuf);
        int numFrames = iufFrameListGetSize(frameList);

        double lambdaMm = iufAcquisitionGetSpeedOfSound(acquisition) / iufTransducerGetCenterFrequency(transducer);
        double depthWL = depth/lambdaMm;
        fprintf(stderr, "1...\n");
        strcat(script, writeTransducer(transducer));
        fprintf(stderr, "2...\n");
        strcat(script, writeResource(transducer, acquisition, depth, numFrames));
        fprintf(stderr, "3...\n");
        strcat(script, writeTx(patternList, transducer, apodizationDict, sourcesDict));
        strcat(script, writeTw(pulse)); // Verasonics only supports 1 TW structure, not an array for each pattern?
        strcat(script, writeTGC(patternList, receiveSettingsDict, depthWL));
        fprintf(stderr, "6...\n");
        strcat(script, writeRx(patternList, iufTransducerGetNumElements(transducer), depthWL, numFrames));
        fprintf(stderr, "7...\n");
        strcat(script, writeRecon(patternList));
        fprintf(stderr, "8...\n");
        strcat(script, writeProcess()); //processing of data is currently a fixed 2 item array
        fprintf(stderr, "9...\n");
        strcat(script, writeEvents(frameList, patternList, label)); //writes the patterns of each frame with certain label
        strcat(script, writeFooter("veragen.mat"));
    }
    else
    {
        return "";
    }

    return script;
}

int main(int argc, char *argv[])
{
    int version; 
    int opt;
    char *veraScript;
    FILE *outputFile;
    iuif_t iuf = NULL;
    char *label;
    double imaging_depth = DEFAULT_IMAGING_DEPTH;
    int optionCount=0;
    
    version = iufGetVersionMajor();

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
		break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
        optionCount++;
    }

    if (!iuf)
    {
        fprintf(stderr, "illegal options.\n\n");
        printUsage(argv);
        exit(1);
    }

    veraScript = parseIuf(iuf, label, imaging_depth);
    fprintf(outputFile, "%s", veraScript);
    fclose(outputFile);

    return 0;
}
