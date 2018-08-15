// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAMELIST_H
#define IUSLIBRARY_IUSHLFRAMELIST_H

#include <iusHLFrame.h>

// ADT
typedef struct IusFrameList IusFrameList;
typedef IusFrameList *iufl_t;
#define  IUFL_INVALID (iufl_t) NULL

iufl_t iusHLFrameListCreate
(
    int numFrame
);

int iusHLFrameListDelete
(
    iufl_t list
);

int iusHLFrameListCompare
(
    iufl_t reference,
    iufl_t actual
);

int iusHLFrameListGetSize
(
    iufl_t list
);

iuf_t iusHLFrameListGet
(
    iufl_t list,
    int index
);

int iusHLFrameListSet
(
    iufl_t list,
    iuf_t member,
    int index
);

#endif //IUSLIBRARY_IUSHLFRAMELIST_H
