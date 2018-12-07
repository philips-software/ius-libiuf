//
// Created by Ruijzendaal on 12/03/2018.
//
#include <string.h>

#include <ius.h>
#include <iusInputFilePrivate.h>
#include <iusParameterDictPrivate.h>
#include <iusHistoryNodeListPrivate.h>

#define MAX_ID_LENGTH   40

// ADT
struct IusHistoryNode
{

    char                *pId;
    char                *pType;
    int                 numberOfParents;
    iuhnl_t             parents;
    int                 numberOfParameters;
    iupad_t             parameters;
    void *              instanceData;
    IUS_BOOL            deepDelete;

} ;

static iuhn_t iusHistoryNodeCreateWithId
(
    char *ID,
    char *pNodeType
)
{
    if ( pNodeType == NULL ) return IUHN_INVALID;
    IusHistoryNode *pIusNode = calloc(1, sizeof( IusHistoryNode ));
    pIusNode->pId = strdup(ID);
    pIusNode->pType = strdup(pNodeType);
    pIusNode->numberOfParameters = 0;
    pIusNode->parents = IUHNL_INVALID;
    pIusNode->parameters = IUPAD_INVALID;
    pIusNode->instanceData = NULL;
    pIusNode->deepDelete = IUS_FALSE;
    return pIusNode;
}

iuhn_t iusHistoryNodeCreate
(
    char *pNodeType
)
{
    char ID[MAX_ID_LENGTH];
    if ( pNodeType == NULL ) return IUHN_INVALID;
    // Uuid should be generated for it.
    setIusUuidCreate( ID );
    return iusHistoryNodeCreateWithId(ID,pNodeType);
}

int iusHistoryNodeDelete
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    if(node->numberOfParents!=0 && node->deepDelete == IUS_TRUE)
        iusHistoryNodeListDelete(node->parents);
    if(node->numberOfParameters!=0 && node->deepDelete == IUS_TRUE)
    {
        iusParameterDictDelete(node->parameters);
    }
    free(node->pId);
    free(node->pType);
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
    if ( node->parents == IUHNL_INVALID ) return 0;
    return iusHistoryNodeListGetSize(node->parents);
}

int iusHistoryNodeGetNumParams
(
    iuhn_t node
)
{
    if ( node == NULL ) return -1;
    return node->numberOfParameters;
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

void *iusHistoryNodeGetInstanceData
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUHNL_INVALID;
    return node->instanceData;
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

int iusHistoryNodeSetInstanceData
(
    iuhn_t node,
    void *instanceData
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    node->instanceData = instanceData;
    return IUS_E_OK;
}

#define NODE_ID "ID"
#define NODE_TYPE "Type"
#define NODE_NUMBER_OF_PARENTS "NumParents"
#define NODE_NUMBER_OF_PARAMETERS "NumParameters"
#define NODE_PARAMETERS "NodeParameters"


void *iusHistoryNodeLoadInstance
(
    iuhn_t node,
    hid_t handle
)
{
    void *instance = NULL;
    if ( strcmp(node->pType,IUS_INPUT_TYPE)  == 0 )
    {
        instance = iusInputFileInstanceLoad(handle);
        if (instance == NULL) return NULL;
    }
    return instance;
}


iuhn_t iusHistoryNodeLoad
(
    hid_t handle
)
{
    herr_t status = 0;
    char ID[IUS_MAX_HDF5_PATH];
    char type[IUS_MAX_HDF5_PATH];
    int numberOfParents=0;
    int numberOfParameters=0;
    if ( handle == H5I_INVALID_HID ) return IUHN_INVALID;
    status |= iusHdf5ReadString(handle, NODE_ID, ID);
    status |= iusHdf5ReadString(handle, NODE_TYPE, type);
    status |= iusHdf5ReadInt(handle, NODE_NUMBER_OF_PARENTS, &numberOfParents);
    status |= iusHdf5ReadInt(handle, NODE_NUMBER_OF_PARAMETERS, &numberOfParameters);
    if (status!=0)
        return IUHN_INVALID;

    iuhn_t loadedObj = iusHistoryNodeCreateWithId((char *)ID, (char *)type);
    if (numberOfParents > 0)
    {
        iuhnl_t loadedParents = iusHistoryNodeListLoad(handle, numberOfParents);
        status = iusHistoryNodeSetParents(loadedObj, loadedParents);
        if (status != 0)
        {
            iusHistoryNodeDelete(loadedObj);
            return IUHN_INVALID;
        }
    }

    // Processing parameters are optional
    if (numberOfParameters > 0)
    {
        hid_t group_id = H5Gopen(handle, NODE_PARAMETERS, H5P_DEFAULT);
        iupad_t loadedParams = iusParameterDictLoad(group_id);
        status |= H5Gclose(group_id );
        status |= iusHistoryNodeSetParameters(loadedObj, loadedParams);
        if (status != 0)
        {
            iusHistoryNodeDelete(loadedObj);
            return IUHN_INVALID;
        }
    }

    // load instance data
    void *instance = iusHistoryNodeLoadInstance(loadedObj,handle);
    iusHistoryNodeSetInstanceData(loadedObj,instance);
    loadedObj->deepDelete = IUS_TRUE;
    return loadedObj;
}

iuhn_t iusHistoryNodeLoadAnyType
(
    hid_t handle
)
{
    if ( handle == H5I_INVALID_HID ) return IUHN_INVALID;

    iuhn_t node = iusHistoryNodeLoad(handle);
    if ( node == IUHN_INVALID ) return IUHN_INVALID;
    if ( strcmp( node->pType, IUS_INPUT_TYPE) == 0 )
    {
        node = iusInputFileLoadNode(handle);
    }
    return node;
}

int iusHistoryNodeSaveInstance
(
    iuhn_t node,
    hid_t handle
)
{
    if (strcmp(iusHistoryNodeGetType(node),IUS_INPUT_TYPE)==0)
    {
        iuifi_t instance = (iuifi_t) iusHistoryNodeGetInstanceData(node);
        return iusInputFileSaveInstance(handle, instance);
    }
    return IUS_E_OK;
}

int iusHistoryNodeSave
(
    iuhn_t node,
    hid_t handle
)
{
    herr_t status = 0;
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( handle == H5I_INVALID_HID ) return IUS_ERR_VALUE;
    status |= iusHdf5WriteString(handle, NODE_ID, node->pId);
    status |= iusHdf5WriteString(handle, NODE_TYPE, node->pType);
    status |= iusHdf5WriteInt(handle, NODE_NUMBER_OF_PARENTS, &node->numberOfParents, 1);
    status |= iusHdf5WriteInt(handle, NODE_NUMBER_OF_PARAMETERS, &node->numberOfParameters, 1);

    // Processing parameters are optional
    if (node->numberOfParameters != 0)
    {
        hid_t group_id = H5Gcreate(handle, NODE_PARAMETERS, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iusParameterDictSave(node->parameters, group_id);
        status |= H5Gclose(group_id );
    }

    if (node->numberOfParents != 0)
    {
        status |= iusHistoryNodeListSave(node->parents, handle);
    }
    status |= iusHistoryNodeSaveInstance(node, handle);
    return status;
}

int iusHistoryNodeSaveAnyType
(
    iuhn_t node,
    hid_t handle
)
{
    int status;
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( handle == H5I_INVALID_HID ) return IUS_ERR_VALUE;

    // Save Node
    status = iusHistoryNodeSave(node,handle);

    // Save instance data
    if ( strcmp( node->pType, IUS_INPUT_TYPE) == 0 )
    {
        iuifi_t instance = (iuifi_t)iusHistoryNodeGetInstanceData(node);
        status = iusInputFileSaveInstance(handle, instance);
    }
    return status;
}


