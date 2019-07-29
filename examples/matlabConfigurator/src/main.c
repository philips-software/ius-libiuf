#include "iuf.h"
#include <stdio.h>  
#include <unistd.h>
#include <string.h>

static void printUsage(char **argv)
{
  printf("Usage: for %s:\n", argv[0]);
  printf("%s -i [inputfile.iuf] -o [outputfile.m]\n\n", argv[0]);
  
  printf("[outputfile.m] is the name of the .m script to run on a verasonics vantage. The script will create \
 a iuf file with the same header, and merges the corresponding data\n");
}

static char *parseTransducer(iut_t transducer)
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

static char *parseResource(iupald_t patternListDict)
{
    char* script = (cahr *)calloc(500, sizeof(char)); // 500 characters should be enough
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

static char *parseIuf(iuif_t iuf)
{
    char *script = (char *) calloc(8000, sizeof(char));
    sprintf(script, "%% Script generated from file:\n");
    char *transducerScript;
/*    char *resourceScript;
    char *txScript;
    char *rxScript;
    char *tgcScript;
    char *receiveScript;
    char *processScript;
    char *seqControlScript;
    char *eventScript;
    char *uiScript;
*/
    if (iuf)
    {
        transducerScript = parseTransducer(iufInputFileGetTransducer(iuf));
        resourcesScript = parseResource(iufInputFileGetPatternListDict(iuf));
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
