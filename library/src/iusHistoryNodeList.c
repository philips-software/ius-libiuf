//
// Created by nlv09165 on 07/09/2018.
//
#include <stdlib.h>
#include <math.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>
#include <include/iusHistoryNodeListImp.h>
#include <include/iusHistoryNodeImp.h>
#include <include/iusHDF5.h>

// ADT
struct IusHistoryNodeList
{
    int count;
    iuhn_t *   pHistoryNodes ;
} ;

// ADT

iuhnl_t iusHistoryNodeListCreate
(
    int numHistoryNodes
)
{
    iuhnl_t list = calloc(1, sizeof(IusHistoryNodeList));
    if(list!=NULL)
    {
        list->count = numHistoryNodes;
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
    if(list == NULL) return IUS_ERR_VALUE;
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
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
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
    return list->count;
}

iuhn_t iusHistoryNodeListGet
(
    iuhnl_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
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
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
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
            node = iusHistoryNodeLoad(group_id);
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
    return nodeList;
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
    for (i=0;i<node->count;i++)
    {
        sprintf(parentPath, "parent%d", i);
        hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status |= iusHistoryNodeSave(node->pHistoryNodes[i], group_id);
        status |= H5Gclose(group_id );
    }
    return status;
}

