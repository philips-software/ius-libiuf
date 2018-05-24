//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLNODE_H
#define IUSLIBRARY_IUSHLNODE_H

#include "iusTypes.h"

// ADT
struct IusNode;
typedef struct IusNode IusNode;
typedef IusNode *iun_t;
#define IUN_INVALID (iun_t) NULL

// Operations
iun_t iusHLNodeCreate
(
char *pNodeType,
int parents
);

int iusHLNodeDelete
(
iun_t node
);

IUS_BOOL iuHLNodeCompare
(
    iun_t reference,
    iun_t actual
);

// getters
char *iusHLNodeGetType
(
iun_t node
);

int iusHLNodeGetNumParents
(
    iun_t node
);

char *iusHLNodeGetId
(
iun_t node
);


#endif //IUSLIBRARY_IUSHLNODE_H
