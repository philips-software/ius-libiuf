// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLIQFILEIMP_H
#define IUSLIBRARY_IUSHLIQFILEIMP_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <iusIqFile.h>
#include <iusHistoryNode.h>

typedef struct IusIqFileInstance IusIqFileInstance;
typedef IusIqFileInstance *iuiqfi_t;
#define  IUIQFI_INVALID (iuiqfi_t) NULL


iuiqf_t iusIqFileSpecificsLoad
(
    iuiqf_t iqFile
);


int iusIqFileSaveInstance
(
    hid_t handle,
    iuiqfi_t instanceData
);

iuhn_t iusIqFileLoadNode
(
    hid_t handle
);

void *iusIqFileInstanceLoad
(
    hid_t handle
);

int iusIqFileInstanceDelete
(
    iuiqfi_t instanceData
);

int iusIqFileSetFilename
(
    iuiqf_t iqFile,
    char * fileName
);

#endif //IUSLIBRARY_IUSHLIQFILEIMP_H
