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

iuf_t iusOpenFile(const char *filename)
{
    return IUF_INVALID;
}

iuh_t iusGetHeader(iuf_t fileHandle)
{
    return IUH_INVALID;
}