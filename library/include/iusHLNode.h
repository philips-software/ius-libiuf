//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLNODE_H
#define IUSLIBRARY_IUSHLNODE_H

#ifndef IUSLIBRARY_IMPLEMENTATION
struct IusNode;
typedef struct IusNode IusNode;
#define IUN_INVALID NULL
#endif //IUSLIBRARY_IMPLEMENTATION

// ADT
typedef IusNode *iun_t;

// Operations
iun_t iusHLCreateNode(char *pNodeType,int parents);
char *iusHLGetNodeType(iun_t node);
int iusHLNodeGetNumParents(iun_t node);
char *iusHLGetNodeId(iun_t node);
IUS_BOOL iusCompareNode(iun_t reference, iun_t actual);

#endif //IUSLIBRARY_IUSHLNODE_H
