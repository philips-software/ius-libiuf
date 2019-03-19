//
// Created by nlv09165 on 07/09/2018.
//
#include <stdlib.h>

#include <iuf.h>
#include <iufHistoryNodePrivate.h>
#include <iufHistoryNodeListPrivate.h>

// ADT
struct IufHistoryNodeList
{
    int numHistoryNodes;
    iuhn_t *   pHistoryNodes ;
    IUF_BOOL deepDelete;
} ;

// ADT
iuhnl_t iufHistoryNodeListCreate
(
    int numHistoryNodes
)
{
    IUF_ERR_EVAL_N_RETURN(numHistoryNodes<=0, IUHNL_INVALID);
    iuhnl_t list = calloc(1, sizeof(IufHistoryNodeList));
    IUF_ERR_ALLOC_NULL_N_RETURN(list, IufHistoryNodeList, IUHNL_INVALID);
    list->deepDelete = IUF_FALSE;
    list->numHistoryNodes = numHistoryNodes;
    list->pHistoryNodes = (iuhn_t *) calloc((size_t)numHistoryNodes, sizeof(iuhn_t));
    if( list->pHistoryNodes == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pHistoryNodes member");
        free(list);
        list = IUHNL_INVALID;
    }
    return list;
}


int iufHistoryNodeListDelete
(
    iuhnl_t list
)
{
    int i;
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    if(list->deepDelete==IUF_TRUE)
    {
        for (i=0;i<list->numHistoryNodes;i++)
        {
            iufHistoryNodeDelete(list->pHistoryNodes[i]);
        }
    }
    if(list->numHistoryNodes>0)
        free(list->pHistoryNodes);
    free(list);
    return IUF_E_OK;
}


// operations
int iufHistoryNodeListCompare
(
    iuhnl_t reference,
    iuhnl_t actual
)
{
    int index;
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numHistoryNodes != actual->numHistoryNodes ) return IUF_FALSE;
    for(index = 0 ; index < actual->numHistoryNodes ; index++ )
    {
        if( iufHistoryNodeCompare( reference->pHistoryNodes[index], actual->pHistoryNodes[index] )
            == IUF_FALSE )
            return IUF_FALSE;
    }
    return IUF_TRUE;
}


int iufHistoryNodeListGetSize
(
    iuhnl_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, -1);
    return list->numHistoryNodes;
}

iuhn_t iufHistoryNodeListGet
(
    iuhnl_t list,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUHN_INVALID);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numHistoryNodes, IUHN_INVALID);
    return list->pHistoryNodes[index];
}

int iufHistoryNodeListSet
(
    iuhnl_t list,
    iuhn_t member,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numHistoryNodes, IUF_ERR_VALUE);
    list->pHistoryNodes[index] = member;
    return IUF_E_OK;
}

iuhnl_t iufHistoryNodeListLoad
(
    hid_t handle,
    int numHistoryNodes
)
{
    int i;
    int status=0;
    iuhn_t node;
    iuhnl_t nodeList = iufHistoryNodeListCreate(numHistoryNodes);
    char parentPath[IUF_MAX_HDF5_PATH];
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUHNL_INVALID);
    for (i=0;i<numHistoryNodes;i++)
    {
        sprintf(parentPath, "parent%d", i);
        hid_t group_id = H5Gopen(handle, parentPath, H5P_DEFAULT);
        if (group_id != H5I_INVALID_HID)
        {
            node = iufHistoryNodeLoadAnyType(group_id);
            status |= iufHistoryNodeListSet(nodeList, node, i);
            status |= H5Gclose(group_id );
        }
        else
        {
            status = IUF_ERR_VALUE;
        }
    }

    if (status!=0)
    {
        iufHistoryNodeListDelete(nodeList);
        nodeList = IUHNL_INVALID;
    }
    nodeList->deepDelete = IUF_TRUE;
    return nodeList;
}

IUF_BOOL iufHistoryNodeListFull
(
    iuhnl_t node
)
{
    IUF_BOOL isFull = IUF_TRUE;
    int i;
    for (i=0;i < node->numHistoryNodes;i++)
    {
        if(node->pHistoryNodes[i] == IUHN_INVALID)
        {
            isFull = IUF_FALSE;
            break;
        }
    }
    return isFull;
}

int iufHistoryNodeListSave
(
    iuhnl_t node,
    hid_t handle
)
{
    int i;
    int status=0;
    char parentPath[IUF_MAX_HDF5_PATH];

    IUF_ERR_CHECK_NULL_N_RETURN(node, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    if(iufHistoryNodeListFull(node) == IUF_FALSE)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "argument list (history node list) was not full");
        return IUF_ERR_VALUE;
    }

    for (i=0;i<node->numHistoryNodes;i++)
    {
        sprintf(parentPath, "parent%d", i);
        hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iufHistoryNodeSaveAnyType(node->pHistoryNodes[i], group_id);
        status |= H5Gclose(group_id );
    }
    return status;
}

