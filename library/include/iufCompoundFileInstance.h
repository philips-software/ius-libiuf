#ifndef IUFLIBRARY_IUFHLCOMPOUNDFILEIMP_H
#define IUFLIBRARY_IUFHLCOMPOUNDFILEIMP_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <iufCompoundFile.h>
#include <iufHistoryNode.h>

typedef struct IufCompoundFileInstance IufCompoundFileInstance;
typedef IufCompoundFileInstance *iucfi_t;
#define  IUCFI_INVALID (iucfi_t) NULL


iucf_t iufCompoundFileSpecificsLoad
(
	iucf_t inputFile
);


int iufCompoundFileSaveInstance
(
	hid_t handle,
	iucfi_t instanceData
);

iuhn_t iufCompoundFileLoadNode
(
	hid_t handle
);

void *iufCompoundFileInstanceLoad
(
	hid_t handle
);

int iufCompoundFileInstanceDelete
(
	iucfi_t instanceData
);

int iufCompoundFileSetFilename
(
	iucf_t inputFile,
	char * fileName
);

#endif //IUFLIBRARY_IUFHLINPUTFILEIMP_H
