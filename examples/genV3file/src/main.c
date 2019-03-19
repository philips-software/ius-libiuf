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
#include "iuf.h"
#include "nodeConfig.h"



void dgFillData
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

int saveFrames
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    iud_t frame = iufInputFileFrameCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(frame, 1 + i * 1.0f);
        offset->t = i;
        status |= iufInputFileFrameSave(inputFile, label, frame, offset);
    }

    iufDataDelete(frame);
    return status;
}

int main
(
    int argc,
    char *argv[]
)
{
    int version = iufGetVersionMajor();
    int numFrames = 10;
    char *ultrasoundMode = "bmode";
    iufErrorLog(IUF_TRUE);

    if (argc != 2)
    {
        printf("Usage: ./example [outputfile]\n");
        return -1;
    }
    printf("Creating file %s, for version %d\n", argv[1], version);
    iuif_t ifh = iufInputFileCreate(argv[1]);
    if (ifh == IUIF_INVALID)
    {
        IUF_ERROR_FMT_PRINT(IUF_ERR_MAJ_GENERAL, IUF_ERR_MIN_ARG_VALUE, "Unable to create file %s", argv[1]);
        return -1;
    }

    int status = ncInputFileNodeConfig(ifh, numFrames, "S5-1", ultrasoundMode);
    status |= iufInputFileNodeSave(ifh);
    if( status != 0 )
    {
        IUF_ERROR_FMT_PRINT(IUF_ERR_MAJ_GENERAL, IUF_ERR_MIN_ARG_VALUE, "Configuring node for file  %s", argv[1]);
        return -1;
    }

    status = saveFrames(ifh,ultrasoundMode,numFrames);
    if( status != 0 )
    {
        IUF_ERROR_FMT_PRINT(IUF_ERR_MAJ_GENERAL, IUF_ERR_MIN_ARG_VALUE, "Saving frames for file  %s", argv[1]);
        return -1;
    }

    // Create Meta data in Node
    iufInputFileClose(ifh);
    iufInputFileDelete(ifh);
    return 0;
}
