#include <string.h>

#include <iuf.h>
#include <iufInputFilePrivate.h>
#include <iufIqFilePrivate.h>
#include <iufParameterDictPrivate.h>
#include <iufHistoryNodeListPrivate.h>
#include "iufHistoryNodeADT.h"

#define MAX_ID_LENGTH   40


static iuhn_t iufHistoryNodeCreateWithId
(
    char *ID,
    char *pNodeType
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(ID, IUHN_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(pNodeType, IUHN_INVALID);
    IufHistoryNode *pIufNode = calloc(1, sizeof( IufHistoryNode ));
    IUF_ERR_ALLOC_NULL_N_RETURN(pIufNode, IufHistoryNode, IUHN_INVALID);
    pIufNode->pId = strdup(ID);
    pIufNode->pType = strdup(pNodeType);
    pIufNode->numberOfParameters = 0;
    pIufNode->parents = IUHNL_INVALID;
    pIufNode->parameters = IUPAD_INVALID;
    pIufNode->instanceData = NULL;
    pIufNode->deepDelete = IUF_FALSE;
    return pIufNode;
}

iuhn_t iufHistoryNodeCreate
(
    char *pNodeType
)
{
    char ID[MAX_ID_LENGTH];
    IUF_ERR_CHECK_NULL_N_RETURN(pNodeType, IUHN_INVALID);
    // Uuid should be generated for it.
    setIufUuidCreate( ID );
    return iufHistoryNodeCreateWithId(ID,pNodeType);
}

int iufHistoryNodeDelete
(
    iuhn_t historyNode
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUF_ERR_VALUE);
    if(historyNode->numberOfParents!=0 && historyNode->deepDelete == IUF_TRUE)
    {
        iufHistoryNodeListDelete(historyNode->parents);
    }
    if(historyNode->numberOfParameters!=0 && historyNode->deepDelete == IUF_TRUE)
    {
        iufParameterDictDelete(historyNode->parameters);
    }
    free(historyNode->pId);
    free(historyNode->pType);
    free(historyNode);
    return IUF_E_OK;
}

IUF_BOOL iufHistoryNodeCompareWithId
(
    iuhn_t reference,
    iuhn_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( strcmp( reference->pId, actual->pId ) != 0 )
    {
        return IUF_FALSE;
    }
    return iufHistoryNodeCompare(reference,actual);
}

IUF_BOOL iufHistoryNodeCompare
(
    iuhn_t reference,
    iuhn_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if ( reference->numberOfParents != actual->numberOfParents )
    {
        return IUF_FALSE;
    }

    if ( reference->numberOfParameters != actual->numberOfParameters )
    {
        return IUF_FALSE;
    }

    if ( strcmp( reference->pType, actual->pType ) != 0 )
    {
        return IUF_FALSE;
    }

    if( iufHistoryNodeListCompare(reference->parents,actual->parents) == IUF_FALSE )
    {
        return IUF_FALSE;
    }

    return iufParameterDictCompare(reference->parameters,actual->parameters);
}


int iufHistoryNodeGetNumParents
(
    iuhn_t historyNode
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, -1);
    if ( historyNode->parents == IUHNL_INVALID ) return 0;
    return iufHistoryNodeListGetSize(historyNode->parents);
}

int iufHistoryNodeGetNumParams
(
    iuhn_t historyNode
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, -1);
    return historyNode->numberOfParameters;
}

iupad_t iufHistoryNodeGetParameters
(
    iuhn_t historyNode
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUPAD_INVALID);
    return historyNode->parameters;
}

char *iufHistoryNodeGetId(iuhn_t historyNode) {
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, NULL);
    return historyNode->pId;
}

char *iufHistoryNodeGetType(iuhn_t historyNode) {
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, NULL);
    return historyNode->pType;
}

iuhnl_t iufHistoryNodeGetParents
(
    iuhn_t historyNode
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUHNL_INVALID);
    return historyNode->parents;
}

void *iufHistoryNodeGetInstanceData
(
    iuhn_t historyNode
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUHNL_INVALID);
    return historyNode->instanceData;
}

int iufHistoryNodeSetParents
(
    iuhn_t historyNode,
    iuhnl_t parents
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(parents, IUF_ERR_VALUE);
    historyNode->parents = parents;
    return IUF_E_OK;
}

int iufHistoryNodeSetParameters
(
    iuhn_t historyNode,
    iupad_t parameterDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(parameterDict, IUF_ERR_VALUE);
    historyNode->parameters = parameterDict;
    historyNode->numberOfParameters = (int) iufParameterDictGetSize(historyNode->parameters);
    return IUF_E_OK;
}

int iufHistoryNodeSetInstanceData
(
    iuhn_t historyNode,
    void *instanceData
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(instanceData, IUF_ERR_VALUE);
    historyNode->instanceData = instanceData;
    return IUF_E_OK;
}

#define NODE_ID "ID"
#define NODE_TYPE "Type"
#define NODE_NUMBER_OF_PARENTS "NumParents"
#define NODE_NUMBER_OF_PARAMETERS "NumParameters"
#define NODE_PARAMETERS "NodeParameters"


void *iufHistoryNodeLoadInstance
(
    iuhn_t historyNode,
    hid_t handle
)
{
    void *instance = NULL;
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode, NULL);
    if ( strcmp(historyNode->pType,IUF_INPUT_TYPE)  == 0 )
    {
        instance = iufInputFileInstanceLoad(handle);
        if (instance == NULL) return NULL;
    }
	else if (strcmp(historyNode->pType, IUF_IQ_TYPE) == 0)
	{
		instance = iufIqFileInstanceLoad(handle);
		if (instance == NULL) return NULL;
	}
    return instance;
}


iuhn_t iufHistoryNodeLoad
(
    hid_t handle
)
{
    herr_t status = 0;
    char ID[IUF_MAX_HDF5_PATH];
    char type[IUF_MAX_HDF5_PATH];
    int numberOfParents=0;
    int numberOfParameters=0;
    if ( handle == H5I_INVALID_HID ) return IUHN_INVALID;
    status |= iufHdf5ReadString(handle, NODE_ID, ID);
    status |= iufHdf5ReadString(handle, NODE_TYPE, type);
    status |= iufHdf5ReadInt(handle, NODE_NUMBER_OF_PARENTS, &numberOfParents);
    status |= iufHdf5ReadInt(handle, NODE_NUMBER_OF_PARAMETERS, &numberOfParameters);
    if (status!=0)
        return IUHN_INVALID;

    iuhn_t loadedObj = iufHistoryNodeCreateWithId((char *)ID, (char *)type);
    if (numberOfParents > 0)
    {
        iuhnl_t loadedParents = iufHistoryNodeListLoad(handle, numberOfParents);
        status = iufHistoryNodeSetParents(loadedObj, loadedParents);
        if (status != 0)
        {
            iufHistoryNodeDelete(loadedObj);
            return IUHN_INVALID;
        }
    }

    // Processing parameters are optional
    if (numberOfParameters > 0)
    {
        hid_t group_id = H5Gopen(handle, NODE_PARAMETERS, H5P_DEFAULT);
        iupad_t loadedParams = iufParameterDictLoad(group_id);
        status |= H5Gclose(group_id );
        status |= iufHistoryNodeSetParameters(loadedObj, loadedParams);
        if (status != 0)
        {
            iufHistoryNodeDelete(loadedObj);
            return IUHN_INVALID;
        }
    }

    // load instance data
    void *instance = iufHistoryNodeLoadInstance(loadedObj,handle);
    iufHistoryNodeSetInstanceData(loadedObj,instance);
    loadedObj->deepDelete = IUF_TRUE;
    return loadedObj;
}

iuhn_t iufHistoryNodeLoadAnyType
(
    hid_t handle
)
{
    if ( handle == H5I_INVALID_HID ) return IUHN_INVALID;

    iuhn_t node = iufHistoryNodeLoad(handle);
    if ( node == IUHN_INVALID ) return IUHN_INVALID;
    if ( strcmp( node->pType, IUF_INPUT_TYPE) == 0 )
    {
        node = iufInputFileLoadNode(handle);
    }
	else if (strcmp(node->pType, IUF_IQ_TYPE) == 0)
	{
		node = iufIqFileLoadNode(handle);
	}
    return node;
}

int iufHistoryNodeSaveInstance
(
    iuhn_t historyNode,
    hid_t handle
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode,  IUF_ERR_VALUE);
    if (strcmp(iufHistoryNodeGetType(historyNode),IUF_INPUT_TYPE)==0)
    {
        iuifi_t instance = (iuifi_t) iufHistoryNodeGetInstanceData(historyNode);
        return iufInputFileSaveInstance(handle, instance);
    }
	if (strcmp(iufHistoryNodeGetType(historyNode), IUF_IQ_TYPE) == 0)
	{
		iuiqfi_t instance = (iuiqfi_t)iufHistoryNodeGetInstanceData(historyNode);
		return iufIqFileSaveInstance(handle, instance);
	}
    return IUF_E_OK;
}

int iufHistoryNodeSave
(
    iuhn_t historyNode,
    hid_t handle
)
{
    herr_t status = 0;
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode,  IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status |= iufHdf5WriteString(handle, NODE_ID, historyNode->pId);
    status |= iufHdf5WriteString(handle, NODE_TYPE, historyNode->pType);
    status |= iufHdf5WriteInt(handle, NODE_NUMBER_OF_PARENTS, &historyNode->numberOfParents, 1);
    status |= iufHdf5WriteInt(handle, NODE_NUMBER_OF_PARAMETERS, &historyNode->numberOfParameters, 1);

    // Processing parameters are optional
    if (historyNode->numberOfParameters != 0)
    {
        hid_t group_id = H5Gcreate(handle, NODE_PARAMETERS, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iufParameterDictSave(historyNode->parameters, group_id);
        status |= H5Gclose(group_id );
    }

    if (historyNode->numberOfParents != 0)
    {
        status |= iufHistoryNodeListSave(historyNode->parents, handle);
    }
    status |= iufHistoryNodeSaveInstance(historyNode, handle);
    return status;
}

int iufHistoryNodeSaveAnyType
(
    iuhn_t historyNode,
    hid_t handle
)
{
    int status;
    IUF_ERR_CHECK_NULL_N_RETURN(historyNode,  IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    // Save Node
    status = iufHistoryNodeSave(historyNode,handle);

    // Save instance data
    if ( strcmp( historyNode->pType, IUF_INPUT_TYPE) == 0 )
    {
        iuifi_t instance = (iuifi_t)iufHistoryNodeGetInstanceData(historyNode);
        status = iufInputFileSaveInstance(handle, instance);
    }
    return status;
}


