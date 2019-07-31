#include "iuf.h"
#include <stdio.h>  
#include <unistd.h>
#include <string.h>
#include <math.h>

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

static char *writeResource(iupal_t patternList, iut_t transducer, iua_t acquisition, iurs_t receiveSettings)
{
    char* script = (char *)calloc(500, sizeof(char)); // 500 characters should be enough
    int numWaves;
    sprintf(script, "Resource.Parameters.connector             = 1;\n" \
                    "Resource.Parameters.numTransmit           = %d;\n" \
                    "Resource.Parameters.numRcvChannels        = %d;\n" \
                    "Resource.Parameters.speedOfSound          = %f;\n" \
                    "Resource.Parameters.speedCorrectionFactor = 1.0;\n" \
                    "Resource.Parameters.startEvent            = 1;\n" \
                    "Resource.Parameters.simulateMode          = 0;\n " \
                    "Resource.Parameters.fakeScanhead          = 0;\n" \
                    "Resource.Parameters.verbose               = 2;\n" \
                    "Resource.RcvBuffer(1).datatype            = \'int16\';\n" \
                    "Resource.RcvBuffer(1).rowsPerFrame = (lineLengthRcvBuffer) * numWaves;\n" \
                    "Resource.RcvBuffer(1).colsPerFrame =  Resource.Parameters.numRcvChannels;\n" \
                    "Resource.RcvBuffer(1).numFrames    = par.nrDMAs;\n",
                    iufTransducerGetNumElements(transducer),
                    iufTransducerGetNumElements(transducer),
                    iufAcquisitionGetSpeedOfSound(acquisition));

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

    sprintf(script, "%%%% Specify TX structure array.\n" \
                    "scaleToWvl = %f;\n,", unitScale);
    for (int t=0; t < numTransmits; t++)
    {
        iupa_t pattern = iufPatternListGet(patternList, t);
        iuta_t apodization = iufTransmitApodizationDictGet(apodizationDict,
                                                           (char *)iufPatternGetApodizationLabel(pattern));
        int numElements = iufTransmitApodizationGetNumElements(apodization);
        for (int i = 0; i < numElements; i++)
        {
            strcat(apodizationString, "%f%c", iufTransmitApodizationGetElement(apodization,i),i==numElements-1?' ':',');
        }
        ius_t source = iufSourceDictGet(sourceDict, (char *)iufPatternGetSourceLabel(pattern));
        IufSourceType sourceType = iufSourceGetType(source);

        iu2dp_t pos2d;
        iu3dp_t pos3d;
        double focus = 50; // focus value is 50 mm

        double theta=0.0, fNumber=0.0;

        switch (sourceType)
        {
            case IUF_2D_NON_PARAMETRIC_SOURCE:
                pos2d = iuf2DNonParametricSourceGetPosition((iu2dnps_t)source, 0);
                break;
            case IUF_2D_PARAMETRIC_SOURCE:
                theta = iuf2DParametricSourceGetStartTheta((iu2dps_t)source);
                fNumber = iuf2DParametricSourceGetFNumber((iu2dps_t)source);
                return "%% writeTX error: paramatric 2D source currently not implemented\n";
            case IUF_3D_NON_PARAMETRIC_SOURCE:
                pos3d = iuf3DNonParametricSourceGetPosition((iu3dnps_t)source, 0);
                break;
            case IUF_3D_PARAMETRIC_SOURCE:
                theta = iuf3DParametricSourceGetStartTheta((iu3dps_t)source);
                theta = iuf3DParametricSourceGetStartPhi((iu3dps_t)source);
                fNumber = iuf3DParametricSourceGetFNumber((iu3dps_t)source);
                return "%% writeTX error: parametric 3D source currently not implemented\n";
            default:
                return "%% writeTX error: unknown sourceType.\n";
        }
        IufTransducerShape transShape = iufTransducerGetShape(transducer);
        if (transShape == IUF_LINE)
        {
        transElemFirst =
        P.radius      = (P.aperture/2);
        TXorgs = P.radius * tan(theta);

        strcat(script, "TX(%d).Origin = [%f 0.0 0.0] * scaleToWvl;\n " \
                       "TX(%d).focus  = %f * scaleToWvl;\n " \
                       "TX(%d).Steer  = [%f, 0];\n" \
                       "TX(%d).Delay  = computeTXDelays(TX(%d));\n;" \
                       "TX(%d).delayOffsetSec = %f\n;",
                        t,


    }

    return script;
}

static char *parseIuf(iuif_t iuf, char *label)
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
        iurs_t receiveSettings = iufReceiveSettingsDictGet(iufInputFileGetReceiveSettingsDict(iuf), label);
        iup_t pulse = iufPulseDictGet(iufInputFileGetPulseDict(iuf), label);
        iuta_t apodization = iufTransmitApodizationDictGet(iufInputFileGetTransmitApodizationDict(iuf), label);
        ius_t sources = iufSourceDictGet(iufInputFileGetSourceDict(iuf), label);

        transducerScript = writeTransducer(transducer);
        resourceScript = writeResource(patternList, transducer, acquisition, receiveSettings);
        txScript = writeTx(patternList, transducer, apodization, pulse, sources, receiveSettings);

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
                label = label;
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

    veraScript = parseIuf(iuf);
    fprintf(outputFile, "%s", veraScript);

    fclose(outputFile);

    return 0;
}
