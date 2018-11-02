//
// Created by nlv09165 on 07/09/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusHistoryNodePrivate.h>
#include <iusHistoryNodeListPrivate.h>

// ADT
struct IusHistoryNodeList
{
    int numHistoryNodes;
    iuhn_t *   pHistoryNodes ;
    IUS_BOOL loadedFromFile;
} ;

// ADT
iuhnl_t iusHistoryNodeListCreate
(
    int numHistoryNodes
)
{
    if (numHistoryNodes<=0) return IUHNL_INVALID;
    iuhnl_t list = calloc(1, sizeof(IusHistoryNodeList));
    if(list!=NULL)
    {
        list->loadedFromFile = IUS_FALSE;
        list->numHistoryNodes = numHistoryNodes;
        list->pHistoryNodes = (iuhn_t *) calloc((size_t)numHistoryNodes, sizeof(iuhn_t));
        if( list->pHistoryNodes == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}


int iusHistoryNodeListDelete
(
    iuhnl_t list
)
{
    int i;
    if(list == NULL) return IUS_ERR_VALUE;
    if(list->loadedFromFile==IUS_TRUE)
    {
        for (i=0;i<list->numHistoryNodes;i++)
        {
            iusHistoryNodeDelete(list->pHistoryNodes[i]);
        }
    }
    if(list->numHistoryNodes>0)
        free(list->pHistoryNodes);
    free(list);
    return IUS_E_OK;
}


// operations
int iusHistoryNodeListCompare
(
    iuhnl_t reference,
    iuhnl_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->numHistoryNodes != actual->numHistoryNodes ) return IUS_FALSE;
    for(index = 0 ; index < actual->numHistoryNodes ; index++ )
    {
        if( iusHistoryNodeCompare( reference->pHistoryNodes[index], actual->pHistoryNodes[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHistoryNodeListGetSize
(
    iuhnl_t list
)
{
    if( list == NULL ) return -1;
    return list->numHistoryNodes;
}

iuhn_t iusHistoryNodeListGet
(
    iuhnl_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->numHistoryNodes ) return NULL;
    return list->pHistoryNodes[index];
}

int iusHistoryNodeListSet
(
    iuhnl_t list,
    iuhn_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->numHistoryNodes ) return IUS_ERR_VALUE;
    list->pHistoryNodes[index] = member;
    return IUS_E_OK;
}

iuhnl_t iusHistoryNodeListLoad
(
    hid_t handle,
    int numHistoryNodes
)
{
    int i;
    int status=0;
    iuhn_t node;
    iuhnl_t nodeList = iusHistoryNodeListCreate(numHistoryNodes);
    char parentPath[IUS_MAX_HDF5_PATH];
    if ( handle == H5I_INVALID_HID ) return IUHNL_INVALID;

    for (i=0;i<numHistoryNodes;i++)
    {
        sprintf(parentPath, "parent%d", i);
        hid_t group_id = H5Gopen(handle, parentPath, H5P_DEFAULT);
        if (group_id != H5I_INVALID_HID)
        {
            node = iusHistoryNodeLoadAnyType(group_id);
            status |= iusHistoryNodeListSet(nodeList, node, i);
            status |= H5Gclose(group_id );
        }
        else
        {
            status = IUS_ERR_VALUE;
        }
    }

    if (status!=0)
    {
        iusHistoryNodeListDelete(nodeList);
        nodeList = IUHNL_INVALID;
    }
    nodeList->loadedFromFile = IUS_TRUE;
    return nodeList;
}

IUS_BOOL iusHistoryNodeListFull
(
    iuhnl_t node
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < node->numHistoryNodes;i++)
    {
        if(node->pHistoryNodes[i] == IUHN_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}

int iusHistoryNodeListSave
(
    iuhnl_t node,
    hid_t handle
)
{
    int i;
    int status=0;
    char parentPath[IUS_MAX_HDF5_PATH];

    if( node == NULL ) return IUS_ERR_VALUE;
    if ( handle == H5I_INVALID_HID ) return IUS_ERR_VALUE;
    if(iusHistoryNodeListFull(node) == IUS_FALSE)
        return IUS_ERR_VALUE;

    for (i=0;i<node->numHistoryNodes;i++)
    {
        sprintf(parentPath, "parent%d", i);
        hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iusHistoryNodeSaveAnyType(node->pHistoryNodes[i], group_id);
        status |= H5Gclose(group_id );
    }
    return status;
}

