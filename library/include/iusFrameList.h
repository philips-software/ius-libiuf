// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAMELIST_H
#define IUSLIBRARY_IUSHLFRAMELIST_H

#include <iusFrame.h>

// ADT
typedef struct IusFrameList IusFrameList;
typedef IusFrameList *iufl_t;
#define  IUFL_INVALID (iufl_t) NULL

iufl_t iusFrameListCreate
(
    int numFrame
);

int iusFrameListDelete
(
    iufl_t list
);

int iusFrameListDeepDelete
(
    iufl_t list
);

int iusFrameListCompare
(
    iufl_t reference,
    iufl_t actual
);

int iusFrameListGetSize
(
    iufl_t list
);

iufr_t iusFrameListGet
(
    iufl_t list,
    int index
);

int iusFrameListSet
(
    iufl_t list,
    iufr_t member,
    int index
);

#endif //IUSLIBRARY_IUSHLFRAMELIST_H
