// Created by nlv09165 on 31/08/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODE_H
#define IUSLIBRARY_IUSHISTORYNODE_H

#include <include/iusParameterDict.h>

// ADT
typedef struct IusHistoryNodeList IusHistoryNodeList;
typedef IusHistoryNodeList *iuhnl_t;
#define  IUHNL_INVALID (iuhnl_t) NULL

typedef struct IusHistoryNode IusHistoryNode;
typedef IusHistoryNode *iuhn_t;
#define  IUHN_INVALID (iuhn_t) NULL

iuhn_t iusHistoryNodeCreate
(
    char *pNodeType
);

int iusHistoryNodeDelete
(
    iuhn_t iusHistoryNode
);

// operations
int iusHistoryNodeCompareWithId
(
    iuhn_t reference,
    iuhn_t actual
);

int iusHistoryNodeCompare
(
    iuhn_t reference,
    iuhn_t actual
);

// Getters
int iusHistoryNodeGetNumParents
(
    iuhn_t iusHistoryNode
);

int iusHistoryNodeGetNumParams
(
    iuhn_t iusHistoryNode
);

char *iusHistoryNodeGetType
(
    iuhn_t iusHistoryNode
);

iuhnl_t iusHistoryNodeGetParents
(
    iuhn_t iusHistoryNode
);

iupad_t iusHistoryNodeGetParameters
(
    iuhn_t node
);

// Setters
int iusHistoryNodeSetParents
(
    iuhn_t iusHistoryNode,
    iuhnl_t parents
);


int iusHistoryNodeSetParameters
(
    iuhn_t node,
    iupad_t parameterDict
);
#endif //IUSLIBRARY_IUSHISTORYNODE_H
