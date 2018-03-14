//
// Created by Ruijzendaal on 08/03/2018.
//
#include <library_config.h>

#define IUSLIBRARY_IMPLEMENTATION
#include "ius.h"
#include "iusNode.h"
#include <iusInputFile.h>
#include <iusInput.h>
#include <iusHLNode.h>
#include <iusHLInput.h>
#include <iusHLInputFile.h>
#include <include/iusInputFile.h>
#include <stdlib.h>
#include <include/iusInput.h>
#include <memory.h>


iue_t iusHLCreateExperiment
(
        float speedOfSound, /**< speed of sound in m/s */
        int date,           /**< interger concatenation of year-month-day  */
        char *pDescription  /**< Experiment notes */
)
{
    IusExperiment *pExperiment = (IusExperiment *) calloc(1, sizeof(IusExperiment));
    pExperiment->speedOfSound = speedOfSound;
    pExperiment->date = date;
    if( pDescription == NULL )
        pExperiment->pDescription = "";
    else
        pExperiment->pDescription = strdup(pDescription);
    return pExperiment;
}

float iusHLExperimentGetSpeedOfSound
(
    iue_t experiment
)
{
    return experiment->speedOfSound;
}

int iusHLExperimentGetDate
(
    iue_t experiment
)
{
    return experiment->date;
}

char * iusHLExperimentGetDescription
(
    iue_t experiment
)
{
    return experiment->pDescription;
}

iuf_t iusHLCreateFile(const char *filename)
{
    IusInputFileInstance *pIFI = iusInputFileCreate(filename);
    return pIFI;
}

int iusHLFileSetHeader(iuf_t fileHandle, iuh_t header)
{
    fileHandle->pIusInput = header;
    return IUS_E_OK;
}

int iusHLFileSave(iuf_t fileHandle)
{
    int status = iusInputWrite(fileHandle->handle, fileHandle->pIusInput);
    return status;
}

int iusHLCloseFile(iuf_t fileHandle)
{
    return iusInputFileClose(fileHandle);
}

iuf_t iusHLOpenFile(const char *filename)
{
    IusInputFileInstance* pIFI;
    pIFI = iusInputFileOpen(filename, 0);
    return pIFI;
}

iuh_t iusHLFileGetHeader(iuf_t fileHandle)
{
    return fileHandle->pIusInput;
}