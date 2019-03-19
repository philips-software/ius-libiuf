// Created by nlv09165 on 11/07/2018.
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


int iufIqFileSaveInstance
(
    hid_t handle,
    iuiqfi_t instanceData
);

iuhn_t iufIqFileLoadNode
(
    hid_t handle
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
