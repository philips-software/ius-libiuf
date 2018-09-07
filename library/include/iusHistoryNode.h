
// Created by nlv09165 on 31/08/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODE_H
#define IUSLIBRARY_IUSHISTORYNODE_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct IusHistoryNode IusHistoryNode;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct IusHistoryNode IusHistoryNode;
typedef IusHistoryNode *iuhn_t;
#define  IUHN_INVALID (iuhn_t) NULL

iuhn_t iusHistoryNodeCreate
(
    int intParam,
    float floatParam
);

int iusHistoryNodeDelete
(
    iuhn_t iusHistoryNode
);


// operations
int iusHistoryNodeCompare
(
    iuhn_t reference,
    iuhn_t actual
);

// Getters
int iusHistoryNodeGetIntParam
(
    iuhn_t iusHistoryNode
);

float iusHistoryNodeGetFloatParam
(
    iuhn_t iusHistoryNode
);

// Setters
int iusHistoryNodeSetIntParam
(
    iuhn_t iusHistoryNode,
    int intParam
);

int iusHistoryNodeSetFloatParam
(
    iuhn_t iusHistoryNode,
    float floatParam
);


#endif //IUSLIBRARY_IUSHISTORYNODE_H
