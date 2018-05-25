//
// Created by nlv09165 on 02/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLPulseList.h>


// ADT
struct IusPulseList
{
    int count;
    iup_t *   pPulses ;
} ;

// ADT
iupl_t iusHLPulseListCreate
(
    int numPulses
)
{
    iupl_t list = calloc(1, sizeof(IusPulseList));
    if(list!=NULL)
    {
        list->count = numPulses;
        list->pPulses = (iup_t *) calloc((size_t)numPulses, sizeof(iup_t));
        if( list->pPulses == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusHLPulseListDelete
(
    iupl_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHLPulseListCompare
(
    iupl_t reference,
    iupl_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if(iusHLPulseCompare(reference->pPulses[index], actual->pPulses[index])
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHLPulseListGetSize
(
    iupl_t list
)
{
    return list->count;
}

iup_t iusHLPulseListGet
(
    iupl_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->pPulses[index];
}

int iusHLPulseListSet
(
    iupl_t list,
    iup_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
    list->pPulses[index] = member;
    return IUS_E_OK;
}