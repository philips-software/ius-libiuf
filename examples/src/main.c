/** ==============================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ==============================================================================
 *
 * \file main.c
 * \brief Main source file containing example code of how to construct a version 3
 * input file type.
 *
 *  ==============================================================================
 */
#include "ius.h"
#include "nodeConfig.h"



void dgFillData
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

int saveFrames
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    iud_t frame = iusInputFileFrameCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(frame, 1 + i * 1.0f);
        offset->t = i;
        status |= iusInputFileFrameSave(inputFile, label, frame, offset);
    }

    iusDataDelete(frame);
    return status;
}

int main
(
    int argc,
    char *argv[]
)
{
    int version = iusGetVersionMajor();
    int numFrames = 10;
    char *ultrasoundMode = "bmode";
    iusDiagDisable();

    if (argc != 2)
    {
        printf("Usage: ./example [outputfile]\n");
        return -1;
    }
    printf("Creating file %s, for version %d\n", argv[1], version);
    iuif_t ifh = iusInputFileCreate(argv[1]);
    if (ifh == IUIF_INVALID)
    {
        fprintf(stderr, "ERROR: Unable to create file %s", argv[1]);
        return -1;
    }

    int status = ncInputFileNodeConfig(ifh, numFrames, "S5-1", ultrasoundMode);
    status |= iusInputFileNodeSave(ifh);
    if( status != 0 )
    {
        fprintf(stderr, "ERROR: configuring node for file  %s", argv[1]);
    }

    status = saveFrames(ifh,ultrasoundMode,numFrames);
    if( status != 0 )
    {
        fprintf(stderr, "ERROR: saving frames for file  %s", argv[1]);
    }

    // Create Meta data in Node
    iusInputFileClose(ifh);
    iusInputFileDelete(ifh);
    return 0;
}
