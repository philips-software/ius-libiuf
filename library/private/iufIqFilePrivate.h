#ifndef IUFLIBRARY_IUFHLIQFILEIMP_H
#define IUFLIBRARY_IUFHLIQFILEIMP_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <iufIqFile.h>
#include <iufHistoryNode.h>

typedef struct IufIqFileInstance IufIqFileInstance;
typedef IufIqFileInstance *iuiqfi_t;
#define  IUIQFI_INVALID (iuiqfi_t) NULL


iuiqf_t iufIqFileSpecificsLoad
(
    iuiqf_t iqFile
);

int iufIqFileCompareInstance
(
    iuiqfi_t reference,
    iuiqfi_t actual
);

int iufIqFileSaveInstance
(
    hid_t handle,
    iuiqfi_t instanceData
);

void *iufIqFileInstanceLoad
(
    hid_t handle
);

int iufIqFileInstanceDelete
(
    iuiqfi_t instanceData
);

int iufIqFileSetFilename
(
    iuiqf_t iqFile,
    char * fileName
);

#endif //IUFLIBRARY_IUFHLIQFILEIMP_H
