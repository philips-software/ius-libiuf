// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSCOMPOUNDFILEIMP_H
#define IUSLIBRARY_IUSCOMPOUNDFILEIMP_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <iusCompoundFile.h>
#include <iusHistoryNode.h>

typedef struct IusCompoundFileInstance IusCompoundFileInstance;
typedef IusCompoundFileInstance *iucfi_t;
#define  IUCFI_INVALID (iucfi_t) NULL


iucf_t iusCompoundFileSpecificsLoad
(
	iucf_t inputFile
);


int iusCompoundFileSaveInstance
(
	hid_t handle,
	iucfi_t instanceData
);

iuhn_t iusCompoundFileLoadNode
(
	hid_t handle
);

void *iusCompoundFileInstanceLoad
(
	hid_t handle
);

int iusCompoundFileInstanceDelete
(
	iucfi_t instanceData
);

int iusCompoundFileSetFilename
(
	iucf_t inputFile,
	char * fileName
);

#endif //IUSLIBRARY_IUSCOMPOUNDFILEIMP_H
