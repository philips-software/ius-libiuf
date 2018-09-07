

// Created by nlv09165 on 07/09/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODELIST_H
#define IUSLIBRARY_IUSHISTORYNODELIST_H

#include <include/iusHistoryNode.h>

// ADT
typedef struct IusHistoryNodeList IusHistoryNodeList;
typedef IusHistoryNodeList *iuhnl_t;
#define  IUHNL_INVALID (iuhnl_t) NULL

iuhnl_t iusHistoryNodeListCreate
(
    int numHistoryNode
);

int iusHistoryNodeListDelete
(
    iuhnl_t list
);

int iusHistoryNodeListCompare
(
    iuhnl_t reference,
    iuhnl_t actual
);

int iusHistoryNodeListGetSize
(
    iuhnl_t list
);

iuhn_t iusHistoryNodeListGet
(
    iuhnl_t list,
    int index
);

int iusHistoryNodeListSet
(
    iuhnl_t list,
    iuhn_t member,
    int index
);

#endif //IUSLIBRARY_IUSHISTORYNODELIST_H
