


//
// Created by nlv09165 on 07/09/2018.
//
#include <stdlib.h>
#include <math.h>
#include <include/iusHistoryNodeList.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>


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