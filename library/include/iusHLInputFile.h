// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H

#include <iusHLPulseDict.h>
#include <iusHLReceiveChannelMapDict.h>

// ADT
typedef struct IusInputFile IusInputFile;
typedef IusInputFile *iuif_t;
#define IUIF_INVALID (iuif_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

iuif_t iusHLInputFileCreate
(
    const char *filename
);

int iusHLInputFileDelete
(
    iuif_t iusInputFile
);

// operations
iuif_t iusHLInputFileLoad
(
    const char *filename
);

int iusHLInputFileSave
(
    iuif_t fileHandle
);

int iusHLInputFileClose
(
    iuif_t fileHandle
);

int iusHLInputFileCompare
(
    iuif_t reference,
    iuif_t actual
);

// Getters
iupd_t iusHLInputFileGetPulseDict
(
    iuif_t fileHandle
);

iurcmd_t iusHLInputFileGetReceiveChannelMapDict
(
	iuif_t fileHandle
);

// Setters

int iusHLInputFileSetPulseDict
(
	iuif_t inputFile,
	iupd_t pulseDict
);

int iusHLInputFileSetReceiveChannelMapDict
(
    iuif_t inputFile,
    iurcmd_t receiveChannelMapDict
);

#endif //IUSLIBRARY_IUSHLINPUTFILE_H
