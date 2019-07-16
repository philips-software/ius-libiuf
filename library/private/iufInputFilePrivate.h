#ifndef IUFLIBRARY_IUFHLINPUTFILEIMP_H
#define IUFLIBRARY_IUFHLINPUTFILEIMP_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <iufInputFile.h>
#include <iufHistoryNode.h>

typedef struct IufInputFileInstance IufInputFileInstance;
typedef IufInputFileInstance *iuifi_t;
#define  IUIFI_INVALID (iuifi_t) NULL


iuif_t iufInputFileSpecificsLoad
(
    iuif_t inputFile
);


int iufInputFileSaveInstance
(
    hid_t handle,
    iuifi_t instanceData
);

iuhn_t iufInputFileLoadNode
(
    hid_t handle
);

void *iufInputFileInstanceLoad
(
    hid_t handle
);

int iufInputFileInstanceDelete
(
    iuifi_t instanceData
);

int iufInputFileSetFilename
(
    iuif_t inputFile,
    char * fileName
);

#endif //IUFLIBRARY_IUFHLINPUTFILEIMP_H
