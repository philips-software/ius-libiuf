// Created by nlv09165 on 31/08/2018.
#include "iusHistoryNode.h"

#ifndef IUSLIBRARY_IUSFILE_H
#define IUSLIBRARY_IUSFILE_H


// ADT
typedef struct IusFile IusFile;
typedef IusFile *iufi_t;
#define  IUFI_INVALID (iufi_t) NULL

iufi_t iusFileOpen
(
    char *pFilename
);

int iusFileDelete
(
    iufi_t iusFile
);


// operations
int iusFileCompare
(
    iufi_t reference,
    iufi_t actual
);

// Getters
iuhn_t iusFileGetHistoryTree
(
    iufi_t iusFile
);

const char *iusFileGetType
(
    iufi_t iusFile
);

#endif //IUSLIBRARY_IUSFILE_H
