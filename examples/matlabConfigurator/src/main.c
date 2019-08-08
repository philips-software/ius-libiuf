#include "iuf.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define DEFAULT_IMAGING_DEPTH 0.010
#define PIXEL_DELTA 0.875

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


static char *writeResource(iupal_t patternList, iut_t transducer, iua_t acquisition, double depth)
{
    char* script = (char *)calloc(5000, sizeof(char)); // 5000 characters should be enough

    // get all keys from the patternList dictionary and for each key go though its list and accumulate the transmits
    int numElements = iufTransducerGetNumElements(transducer);
    int i=0;
    int numWaves = iufPatternListGetSize(patternList);

    // determine rowsPerFrame
    double lambdaMm = 0.001 * iufAcquisitionGetSpeedOfSound(acquisition) / iufTransducerGetCenterFrequency(transducer);
    IufTransducerShape shape = iufTransducerGetShape(transducer);
    double elementPitch=0.0;
    if (shape == IUF_2D_SHAPE)
    {
        iu2dte_t element0 = iuf2DTransducerGetElement((iu2dt_t)transducer, 0);
        iu2dte_t element1 = iuf2DTransducerGetElement((iu2dt_t)transducer, 1);
        elementPitch = IUF_ABS(iuf2DTransducerElementGetPosition(element1).x -
                               iuf2DTransducerElementGetPosition(element0).x);
    }
    double D = (iufTransducerGetNumElements(transducer) - 1) * elementPitch * 1000.0; // in mm
    double rayDelta = (0.25) * asin(1.22*lambdaMm/D);
    int numRays = round(2*(M_PI/4)/rayDelta);
    double maxAqcLength = round(depth/lambdaMm);
    double lineLengthRcvBuffer = 128 * ceil(maxAqcLength / 16.0);
    int rowsPerFrame = 16 * ceil(lineLengthRcvBuffer * numRays / 16.0);

    int interBufferRowsPerFrame = (int)(16 * ceil(2^ceil(log2(maxAqcLength))) / 16); // verify this formula
    int interBufferColsPerFrame = PData.Size(2);

    double sizeX = ceil((depth));
    sprintf(script, "PData.Coord = 'rectangular';\n" \
                    "PData.PDelta = [%f, 1.0, 1.0]; \n" \
                    "PData.Size = []; \n" \
                    "PData.Origin = [] \n" \
                    "PData.Region.Shape.Name = 'Rectangle' \n" \              LEFT OF HER!!!!
                    "PData.Region.Shape.Position = [%f, %f, %f] "\
                    "PData.Region.Shape.Position  'width', ); \n "\
                    "Resource.Parameters.numTransmit           = %d;\n" \
                    "Resource.Parameters.numRcvChannels        = Resource.Parameters.numTransmit;\n" \
                    "Resource.Parameters.speedOfSound          = %f;\n" \
                    "Resource.Parameters.verbose               = 2;\n" \
                    "Resource.RcvBuffer(1).datatype            = \'int16\';\n" \
                    "Resource.RcvBuffer(1).rowsPerFrame        = %d;\n" \
                    "Resource.RcvBuffer(1).colsPerFrame        = Resource.Parameters.numRcvChannels;\n" \
                    "Resource.RcvBuffer(1).numFrames           = 20; %% for cineloop\n" \
                    "Resource.InterBuffer(1).numFrames         = 1;\n"\
                    "Resource.ImageBuffer(1).numFrames         = 1;\n" \
                    "Resource.DisplayWindow(1).Title           = [%s, ...\n" \
                    " ' (', %d,' active elements)', ...\n"\
                    " ', max ', %f,' cm', ...\n" \
                    " ', Fc = ', %f, ' MHz', ...\n" \
                    " ', numRays = ', %d];\n" \
                    " Resource.DisplayWindow(1).pdelta         = %f;\n" \
                    " Resource.DisplayWindow(1).AxesUnits      = 'mm';\n" \
                    " Resource.DisplayWindow(1).Position       = [%d,%d,%d,%d]\n" \
                    " Resource.DisplayWindow(1).ReferencePt    = [%d,%d];\n", \
                    PIXEL_DELTA,
                    iufTransducerGetNumElements(transducer), // Parameters.numTransmit
                    iufAcquisitionGetSpeedOfSound(acquisition), // Parameters.speedOfSound
                    rowsPerFrame,            //RcvBuffer(1).rowsPerFrame
                    interBufferRowsPerFrame, //InterBuffer(1).rowsPerFrame
                    interBufferColsPerFrame, //InterBuffer(1).rowsPerFrame
                    windowTitle, numElements, IMAGING_DEPTH_MM, numRays,
                    IMAGING_DEPTH_MM / (double)WINDOW_RESOLUTION_Y,
                    WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT,
                    refX, refY);
}

static char *writeTx(iupal_t patternList,
                     iut_t transducer,
                     iutad_t apodizationDict,
                     iupd_t pulseDict,
                     iusd_t sourceDict,
                     iursd_t receiveSettingsDict)
{
    char *script = (char *) calloc(5000, sizeof(char));
    char *apodizationString = (char *) calloc(500, sizeof(char));

    double unitScale = iufTransducerGetCenterFrequency(transducer)/1.540;
    int numTransmits = iufPatternListGetSize(patternList);
    double transducerWidth = 0.0;
    int numElements = iufTransducerGetNumElements(transducer);

    IufTransducerShape transShape = iufTransducerGetShape(transducer);
    if (transShape == IUF_LINE)
    {
        iu2dte_t elem2DFirst = iuf2DTransducerGetElement((iu2dt_t) transducer, 0);
        iu2dte_t elem2DLast = iuf2DTransducerGetElement((iu2dt_t) transducer, numElements - 1);
        transducerWidth = abs(elem2DFirst->position.x - elem2DLast->position.x);
    }
    else
    {
        return "%% writeTX error: only 2D linear transducers are currently supported.\n error=1;\n";
    }

    sprintf(script, "%%%% Specify TX structure array.\n" \
                    "scaleToWvl = %f;\n,", unitScale);

    for (int t=0; t < numTransmits; t++)
    {
        iupa_t pattern = iufPatternListGet(patternList, t);
        iuta_t apodization = iufTransmitApodizationDictGet(apodizationDict,
                                                           (char *)iufPatternGetApodizationLabel(pattern));
        for (int i = 0; i < numElements; i++)
        {
            strcat(apodizationString, "%f%c", iufTransmitApodizationGetElement(apodization,i),i==numElements-1?' ':',');
        }
        ius_t source = iufSourceDictGet(sourceDict, (char *)iufPatternGetSourceLabel(pattern));
        IufSourceType sourceType = iufSourceGetType(source);

        // iu2dp_t pos2d;
        // iu3dp_t pos3d;
        double focus = 50; // focus value is 50 mm
        double theta=0.0, fNumber=0.0;

        switch (sourceType)
        {
            case IUF_2D_NON_PARAMETRIC_SOURCE:
                // pos2d = iuf2DNonParametricSourceGetPosition((iu2dnps_t)source, 0);
                return "%% writeTX error: non-parametric 2D source currently not implemented.\n error=1;\n";
                // break;
            case IUF_2D_PARAMETRIC_SOURCE:
                theta = iuf2DParametricSourceGetStartTheta((iu2dps_t)source);
                fNumber = iuf2DParametricSourceGetFNumber((iu2dps_t)source);
                break;
            case IUF_3D_NON_PARAMETRIC_SOURCE:
                // pos3d = iuf3DNonParametricSourceGetPosition((iu3dnps_t)source, 0);
                return "%% writeTX error: non-parametric 3D source currently not implemented.\n error=1;\n";
                break;
            case IUF_3D_PARAMETRIC_SOURCE:
                //theta = iuf3DParametricSourceGetStartTheta((iu3dps_t)source);
                //phi = iuf3DParametricSourceGetStartPhi((iu3dps_t)source);
                //fNumber = iuf3DParametricSourceGetFNumber((iu3dps_t)source);
                return "%% writeTX error: parametric 3D source currently not implemented.\n error=1;\n";
            default:
                return "%% writeTX error: unknown sourceType.\nerror=1; \n";
        }
        focus = IUF_ABS(fNumber) < IUF_FLT_EPS ? 0.0 : transducerWidth / fNumber;
        strcat(script, "TX(%d).Origin = [%f 0.0 0.0] * scaleToWvl;\n " \
                       "TX(%d).focus  = %f * scaleToWvl;\n " \
                       "TX(%d).Steer  = [%f, 0];\n" \
                       "TX(%d).Delay  = computeTXDelays(TX(%d));\n;" \
                        t, transducerWidth * tan(theta),
               t, focus,
               t, theta,
               t, t);
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
                        "TGC(%d).Waveform = computeTGCWaveform(TGC);",
                        (int)i,(int)i, maxDepthWL, (int)i);
    }
    return script;
}


static char *parseIuf(iuif_t iuf, char *label, double depth)
{
    char *script = (char *) calloc(8000, sizeof(char));
    sprintf(script, "%% Script generated from file:\n");
    char *transducerScript;
    char *resourceScript;
    char *txScript;
    char *twScript;
    char *rxScript;
    char *tgcScript;
    char *receiveScript;
    char *processScript;
    char *seqControlScript;
    char *eventScript;
    char *uiScript;

    if (iuf)
    {
        iut_t transducer = iufInputFileGetTransducer(iuf);
        iupal_t patternList = iufPatternListDictGet(iufInputFileGetPatternListDict(iuf), label);
        iua_t acquisition = iufInputFileGetAcquisition(iuf);
        iursd_t receiveSettingsDict = iufInputFileGetReceiveSettingsDict(iuf);
        iupd_t pulseDict = iufInputFileGetPulseDict(iuf);
        iutad_t apodizationDict = iufInputFileGetTransmitApodizationDict(iuf);
        iusd_t sourcesDict = iufInputFileGetSourceDict(iuf);

        transducerScript = writeTransducer(transducer);
        double lambdaMm = iufAcquisitionGetSpeedOfSound(acquisition) / iufTransducerGetCenterFrequency(transducer);
        double depthWL = depth/lambdaMm;
        resourceScript = writeResource(patternList, transducer, acquisition, depth);
        txScript = writeTx(patternList, transducer,
                           apodizationDict, pulseDict, sourcesDict, receiveSettingsDict);
        twScript = writeTw(pulseDict);
        tgcScript = writeTGC(receiveSettingsDict, depthWL);


    }
    else
    {
        return "";
    }

    strcat(script, transducerScript);

    return script;
}

int main
        (
                int argc,
                char *argv[]
        )
{
    int version = iufGetVersionMajor();
    int opt;
    char *veraScript;
    FILE *outputFile;
    iuif_t iuf;
    char *label;
    double imaging_depth = DEFAULT_IMAGING_DEPTH;

    while((opt = getopt(argc, argv, ":i:o:")) != -1)
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
