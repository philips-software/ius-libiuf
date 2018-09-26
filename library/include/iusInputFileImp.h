// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLINPUTFILEIMP_H
#define IUSLIBRARY_IUSHLINPUTFILEIMP_H

#include "iusInputFile.h"
#include "iusHistoryNode.h"

typedef struct IusInputFileInstance IusInputFileInstance;
typedef IusInputFileInstance *iuifi_t;
#define  IUIFI_INVALID (iuifi_t) NULL


iuif_t iusInputFileSpecificsLoad
(
    iuif_t inputFile
);


int iusInputFileSaveInstance
(
    hid_t handle,
    iuifi_t instanceData
);

iuhn_t iusInputFileLoadNode
(
    hid_t handle
);

void *iusInputFileInstanceLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHLINPUTFILEIMP_H
