//
// Created by Ruijzendaal on 12/03/2018.
//
#include <string.h>

#include <ius.h>
#include <iusInputFilePrivate.h>
#include <iusIqFilePrivate.h>
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

iuhn_t iusHistoryNodeLoad(hid_t handle);

static iuhn_t iusHistoryNodeCreateWithId
(
    char *ID,
    char *pNodeType
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(ID, IUHN_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(pNodeType, IUHN_INVALID);
    IusHistoryNode *pIusNode = calloc(1, sizeof( IusHistoryNode ));
    IUS_ERR_ALLOC_NULL_N_RETURN(pIusNode, IusHistoryNode, IUHN_INVALID);
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
    IUS_ERR_CHECK_NULL_N_RETURN(pNodeType, IUHN_INVALID);
    // Uuid should be generated for it.
    setIusUuidCreate( ID );
    return iusHistoryNodeCreateWithId(ID,pNodeType);
}

iuhn_t iusHistoryNodeCreateFromFile
(
    char *pNodeType,
    char *filename
)
{
    IUS_ERR_CHECK_EMPTYSTR_N_RETURN(pNodeType, IUHN_INVALID);
    IUS_ERR_CHECK_EMPTYSTR_N_RETURN(filename, IUHN_INVALID);
    hid_t handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (handle <= 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "File open of %s failed", filename);
        return IUHN_INVALID;
    }

    iuhn_t savedObj = iusHistoryNodeLoad(handle);
    H5Fclose(handle);
    if (savedObj == IUHN_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Unable to load node structure from file %s", filename);
        return IUHN_INVALID;
    }

    // create new node NN
    iuhn_t node = iusHistoryNodeCreate(pNodeType);
    iuhnl_t nodeList_nt = iusHistoryNodeListCreate(1);
    iusHistoryNodeListSet(nodeList_nt,savedObj,0);
    iusHistoryNodeSetParents(node, nodeList_nt);
    return node;
}

int iusHistoryNodeDelete
(
    iuhn_t historyNode
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUS_ERR_VALUE);
    if(historyNode->numberOfParents!=0 && historyNode->deepDelete == IUS_TRUE)
    {
        iusHistoryNodeListDelete(historyNode->parents);
    }
    if(historyNode->numberOfParameters!=0 && historyNode->deepDelete == IUS_TRUE)
    {
        iusParameterDictDelete(historyNode->parameters);
    }
    free(historyNode->pId);
    free(historyNode->pType);
    free(historyNode);
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
    iuhn_t historyNode
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, -1);
    if ( historyNode->parents == IUHNL_INVALID ) return 0;
    return iusHistoryNodeListGetSize(historyNode->parents);
}

int iusHistoryNodeGetNumParams
(
    iuhn_t historyNode
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, -1);
    return historyNode->numberOfParameters;
}

iupad_t iusHistoryNodeGetParameters
(
    iuhn_t historyNode
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUPAD_INVALID);
    return historyNode->parameters;
}

char *iusHistoryNodeGetId(iuhn_t historyNode) {
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, NULL);
    return historyNode->pId;
}

char *iusHistoryNodeGetType(iuhn_t historyNode) {
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, NULL);
    return historyNode->pType;
}

iuhnl_t iusHistoryNodeGetParents
(
    iuhn_t historyNode
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUHNL_INVALID);
    return historyNode->parents;
}

void *iusHistoryNodeGetInstanceData
(
    iuhn_t historyNode
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUHNL_INVALID);
    return historyNode->instanceData;
}

int iusHistoryNodeSetParents
(
    iuhn_t historyNode,
    iuhnl_t parents
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(parents, IUS_ERR_VALUE);
    historyNode->parents = parents;
    historyNode->numberOfParents = iusHistoryNodeListGetSize(parents);
    return IUS_E_OK;
}

int iusHistoryNodeSetParameters
(
    iuhn_t historyNode,
    iupad_t parameterDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(parameterDict, IUS_ERR_VALUE);
    historyNode->parameters = parameterDict;
    historyNode->numberOfParameters = (int) iusParameterDictGetSize(historyNode->parameters);
    return IUS_E_OK;
}

int iusHistoryNodeSetInstanceData
(
    iuhn_t historyNode,
    void *instanceData
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(instanceData, IUS_ERR_VALUE);
    historyNode->instanceData = instanceData;
    return IUS_E_OK;
}

#define NODE_ID "ID"
#define NODE_TYPE "Type"
#define NODE_NUMBER_OF_PARENTS "NumParents"
#define NODE_NUMBER_OF_PARAMETERS "NumParameters"
#define NODE_PARAMETERS "NodeParameters"


void *iusHistoryNodeLoadInstance
(
    iuhn_t historyNode,
    hid_t handle
)
{
    void *instance = NULL;
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode, NULL);
    if ( strcmp(historyNode->pType,IUS_INPUT_TYPE)  == 0 )
    {
        instance = iusInputFileInstanceLoad(handle);
        if (instance == NULL) return NULL;
    }
	else if (strcmp(historyNode->pType, IUS_IQ_TYPE) == 0)
	{
		instance = iusIqFileInstanceLoad(handle);
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
	else if (strcmp(node->pType, IUS_IQ_TYPE) == 0)
	{
		node = iusIqFileLoadNode(handle);
	}
    return node;
}

int iusHistoryNodeSaveInstance
(
    iuhn_t historyNode,
    hid_t handle
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode,  IUS_ERR_VALUE);
    if (strcmp(iusHistoryNodeGetType(historyNode),IUS_INPUT_TYPE)==0)
    {
        iuifi_t instance = (iuifi_t) iusHistoryNodeGetInstanceData(historyNode);
        return iusInputFileSaveInstance(handle, instance);
    }
	if (strcmp(iusHistoryNodeGetType(historyNode), IUS_IQ_TYPE) == 0)
	{
		iuiqfi_t instance = (iuiqfi_t)iusHistoryNodeGetInstanceData(historyNode);
		return iusIqFileSaveInstance(handle, instance);
	}
    return IUS_E_OK;
}

int iusHistoryNodeSave
(
    iuhn_t historyNode,
    hid_t handle
)
{
    herr_t status = 0;
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode,  IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    status |= iusHdf5WriteString(handle, NODE_ID, historyNode->pId);
    status |= iusHdf5WriteString(handle, NODE_TYPE, historyNode->pType);
    status |= iusHdf5WriteInt(handle, NODE_NUMBER_OF_PARENTS, &historyNode->numberOfParents, 1);
    status |= iusHdf5WriteInt(handle, NODE_NUMBER_OF_PARAMETERS, &historyNode->numberOfParameters, 1);

    // Processing parameters are optional
    if (historyNode->numberOfParameters != 0)
    {
        hid_t group_id = H5Gcreate(handle, NODE_PARAMETERS, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iusParameterDictSave(historyNode->parameters, group_id);
        status |= H5Gclose(group_id );
    }

    if (historyNode->numberOfParents != 0)
    {
        status |= iusHistoryNodeListSave(historyNode->parents, handle);
    }
    status |= iusHistoryNodeSaveInstance(historyNode, handle);
    return status;
}

int iusHistoryNodeSaveAnyType
(
    iuhn_t historyNode,
    hid_t handle
)
{
    int status;
    IUS_ERR_CHECK_NULL_N_RETURN(historyNode,  IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    // Save Node
    status = iusHistoryNodeSave(historyNode,handle);

    // Save instance data
    if ( strcmp( historyNode->pType, IUS_INPUT_TYPE) == 0 )
    {
        iuifi_t instance = (iuifi_t)iusHistoryNodeGetInstanceData(historyNode);
        status = iusInputFileSaveInstance(handle, instance);
    }
    return status;
}


