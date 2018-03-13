//
// Created by Ruijzendaal on 08/03/2018.
//
#include <library_config.h>
#include "library.h"

#define IUSLIBRARY_IMPLEMENTATION
#include "ius.h"
#include "iusNode.h"
#include <iusInputFile.h>
#include <iusInput.h>
#include <iusHLNode.h>
#include <iusHLInput.h>
#include <iusHLInputFile.h>
#include <include/iusInputFile.h>

iuf_t iusHLCreateFile(const char *filename, iuh_t inputInstance)
{
    IusInputFileInstance *pIFI = iusInputFileCreate(filename, inputInstance, iusGetVersionMajor());
    return pIFI;
}

int iusSaveHeader(iuf_t fileHandle,iuh_t header)
{
    return !IUS_E_OK;
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

iuh_t iusHLGetHeader(iuf_t fileHandle)
{
    return fileHandle->pIusInput;
}