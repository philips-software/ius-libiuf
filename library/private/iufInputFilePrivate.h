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

int iufInputFileCompareInstance
(
    iuifi_t reference,
    iuifi_t actual
);

int iufInputFileSaveInstance
(
    hid_t handle,
    iuifi_t instanceData
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
