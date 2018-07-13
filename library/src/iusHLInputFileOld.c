//
// Created by Ruijzendaal on 08/03/2018.
//
#include <library_config.h>
#include <memory.h>
#include <stdlib.h>

#include <ius.h>
#include <iusNode.h>
#include <iusInput.h>
#include <iusInputFile.h>
#include <iusHLNode.h>
#include <iusHLInputInstance.h>
#include <iusHLInputFile.h>


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
    int status = iusInputWrite(fileHandle->handle, fileHandle->pIusInput, 1);
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