// Created by nlv09165 on 31/08/2018.
#include "iusHistoryNode.h"

#ifndef IUSLIBRARY_IUSFILE_H
#define IUSLIBRARY_IUSFILE_H


// ADT
typedef struct IusFile IusFile;
typedef IusFile *iuf_t;
#define  IUFI_INVALID (iuf_t) NULL

iuf_t iusFileLoad
(
    char *pFilename
);

int iusFileDelete
(
    iuf_t iusFile
);


// operations
int iusFileCompare
(
    iuf_t reference,
    iuf_t actual
);

// Getters
iuhn_t iusFileGetHistoryTree
(
    iuf_t iusFile
);

const char *iusFileGetType
(
    iuf_t iusFile
);

// Setters
int iusFileSetHistoryTree
(
    iuf_t iusFile,
    iuhn_t history
);
#endif //IUSLIBRARY_IUSFILE_H
