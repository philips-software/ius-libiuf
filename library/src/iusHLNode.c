//
// Created by Ruijzendaal on 12/03/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <include/iusUuid.h>
#include <memory.h>

#include "iusNode.h"
#include "iusHLNode.h"


iun_t iusHLCreateNode(char *pNodeType, int parents) {
    IusNode *pIusNode = calloc(1, sizeof( IusNode ));
    // Uuid should be generated for it.
    setIusUuidCreate( pIusNode->pId );
    strncpy( pIusNode->pType, pNodeType, MAX_TYPE_LENGTH );
    return pIusNode;
}

int iusHLNodeGetNumParents( iun_t node ) {
    return node->numberOfParents;
}

char *iusHLGetNodeId(iun_t node) {
    return node->pId;
}

char *iusHLGetNodeType(iun_t node) {
    return node->pType;
}

