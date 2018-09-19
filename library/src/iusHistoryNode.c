//
// Created by Ruijzendaal on 12/03/2018.
//
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusUuid.h>
#include <iusTypes.h>
#include <iusHDF5.h>
#include <iusError.h>
#include <hdf5_hl.h>
#include <iusHistoryNodeList.h>
#include <iusHistoryNode.h>
#include <include/iusHistoryNodeListImp.h>
#include <include/iusParameterDictImp.h>

#define MAX_TYPE_LENGTH 40
#define MAX_ID_LENGTH   40
#define MAX_PARENTS     10

// ADT
struct IusHistoryNode
{
    char                *pId;
    char                *pType;
    int                 numberOfParents;
    iuhnl_t             parents;
    int                 numberOfParameters;
    iupad_t             parameters;
} ;

static iuhn_t iusHistoryNodeCreateWithId
(
    char *ID,
    char *pNodeType,
    int parents
)
{
    if ( pNodeType == NULL ) return IUHN_INVALID;
    if ( parents < 0 ) return IUHN_INVALID;
    IusHistoryNode *pIusNode = calloc(1, sizeof( IusHistoryNode ));
    pIusNode->pId = strdup(ID);
    pIusNode->pType = strdup(pNodeType);
    pIusNode->numberOfParents = parents;
    pIusNode->numberOfParameters = 0;
    pIusNode->parents = iusHistoryNodeListCreate(parents);
    pIusNode->parameters = IUPAD_INVALID;
    return pIusNode;
}

iuhn_t iusHistoryNodeCreate
(
    char *pNodeType,
    int parents
)
{
    char ID[MAX_ID_LENGTH];
    if ( pNodeType == NULL ) return IUHN_INVALID;
    if ( parents < 0 ) return IUHN_INVALID;
    // Uuid should be generated for it.
    setIusUuidCreate( ID );
    return iusHistoryNodeCreateWithId(ID,pNodeType,parents);
}

int iusHistoryNodeDelete
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    free(node);
    return IUS_E_OK;
}

IUS_BOOL iusHistoryNodeCompareWithId
(
    iuhn_t reference,
    iuhn_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( strcmp( reference->pId, actual->pId ) != 0 )
    {
        return IUS_FALSE;
    }
    return iusHistoryNodeCompare(reference,actual);
}

IUS_BOOL iusHistoryNodeCompare
(
    iuhn_t reference,
    iuhn_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->numberOfParents != actual->numberOfParents )
    {
        return IUS_FALSE;
    }

    if ( reference->numberOfParameters != actual->numberOfParameters )
    {
        return IUS_FALSE;
    }

    if ( strcmp( reference->pType, actual->pType ) != 0 )
    {
        return IUS_FALSE;
    }

    if( iusHistoryNodeListCompare(reference->parents,actual->parents) == IUS_FALSE )
    {
        return IUS_FALSE;
    }

    return iusParameterDictCompare(reference->parameters,actual->parameters);
}


int iusHistoryNodeGetNumParents
(
    iuhn_t node
)
{
    if ( node == NULL ) return -1;
    return node->numberOfParents;
}

int iusHistoryNodeGetNumParams
(
    iuhn_t node
)
{
    if ( node == NULL ) return -1;
    return iusParameterDictGetSize(node->parameters);
}

iupad_t iusHistoryNodeGetParameters
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUPAD_INVALID;
    return node->parameters;
}

char *iusHistoryNodeGetId(iuhn_t node) {
    if ( node == NULL ) return NULL;
    return node->pId;
}

char *iusHistoryNodeGetType(iuhn_t node) {
    if ( node == NULL ) return NULL;
    return node->pType;
}

iuhnl_t iusHistoryNodeGetParents
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUHNL_INVALID;
    return node->parents;
}

int iusHistoryNodeSetParents
(
    iuhn_t node,
    iuhnl_t parents
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( parents == NULL ) return IUS_ERR_VALUE;
    node->parents = parents;
    return IUS_E_OK;
}

int iusHistoryNodeSetParameters
(
    iuhn_t node,
    iupad_t parameterDict
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( parameterDict == NULL ) return IUS_ERR_VALUE;
    node->parameters = parameterDict;
    node->numberOfParameters = iusParameterDictGetSize(node->parameters);
    return IUS_E_OK;
}


#define NODE_ID "ID"
#define NODE_TYPE "type"
#define NODE_NUMBER_OF_PARENTS "numberOfParents"
#define NODE_NUMBER_OF_PARAMETERS "numberOfParameters"
#define NODE_PROCESSING_PARAMETERS "processingParameters"

int iusHistoryNodeSave
(
    iuhn_t node,
    hid_t handle
)
{
    herr_t status = 0;
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( handle == H5I_INVALID_HID ) return IUS_ERR_VALUE;
    status |= iusHdf5WriteString(handle, NODE_ID, node->pId, 1);
    status |= iusHdf5WriteString(handle, NODE_TYPE, node->pType, 1);
    status |= iusHdf5WriteInt(handle, NODE_NUMBER_OF_PARENTS, &node->numberOfParents, 1);
    status |= iusHdf5WriteInt(handle, NODE_NUMBER_OF_PARAMETERS, &node->numberOfParameters, 1);

    // Processing parameters are optional
    if (node->numberOfParameters != 0)
    {
        hid_t group_id = H5Gcreate(handle, NODE_PROCESSING_PARAMETERS, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iusParameterDictSave(node->parameters, group_id);
        status |= H5Gclose(group_id );
    }
    status |= iusHistoryNodeListSave(node->parents, handle);
    return status;
}

iuhn_t iusHistoryNodeLoad
(
    hid_t handle
)
{
    herr_t status = 0;
    const char *ID;
    const char *type;
    int numberOfParents=0;
    int numberOfParameters=0;
    if ( handle == H5I_INVALID_HID ) return IUHN_INVALID;
    status |= iusHdf5ReadString(handle, NODE_ID, &ID);
    status |= iusHdf5ReadString(handle, NODE_TYPE, &type);
    status |= iusHdf5ReadInt(handle, NODE_NUMBER_OF_PARENTS, &numberOfParents);
    status |= iusHdf5ReadInt(handle, NODE_PROCESSING_PARAMETERS, &numberOfParameters);
    if (status!=0)
        return IUHN_INVALID;

    iuhn_t loadedObj = iusHistoryNodeCreateWithId((char *)ID, (char *)type,numberOfParents);
    iuhnl_t loadedParents = iusHistoryNodeListLoad(handle, numberOfParents);
    status = iusHistoryNodeSetParents(loadedObj, loadedParents);
    if (status != 0)
    {
        iusHistoryNodeDelete(loadedObj);
        return IUHN_INVALID;
    }

    // Processing parameters are optional
    if (numberOfParameters > 0)
    {
        hid_t group_id = H5Gopen(handle, NODE_PROCESSING_PARAMETERS, H5P_DEFAULT);
        iupad_t loadedParams = iusParameterDictLoad(group_id);
        status |= H5Gclose(group_id );
        status |= iusHistoryNodeSetParameters(loadedObj, loadedParams);
        if (status != 0)
        {
            iusHistoryNodeDelete(loadedObj);
            return IUHN_INVALID;
        }
    }

    return loadedObj;
}