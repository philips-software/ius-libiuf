


//
// Created by nlv09165 on 18/07/2018.
//
#include <stdlib.h>
#include <math.h>
#include <include/iusHLPatternList.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>



// ADT
struct IusPatternList
{
    int count;
    iupa_t *   pPatterns ;
} ;

// ADT

iupal_t iusHLPatternListCreate
(
    int numPatterns
)
{
    iupal_t list = calloc(1, sizeof(IusPatternList));
    if(list!=NULL)
    {
        list->count = numPatterns;
        list->pPatterns = (iupa_t *) calloc((size_t)numPatterns, sizeof(iupa_t));
        if( list->pPatterns == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusHLPatternListDelete
(
    iupal_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHLPatternListCompare
(
    iupal_t reference,
    iupal_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusHLPatternCompare( reference->pPatterns[index], actual->pPatterns[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHLPatternListGetSize
(
    iupal_t list
)
{
    return list->count;
}

iupa_t iusHLPatternListGet
(
    iupal_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->pPatterns[index];
}

int iusHLPatternListSet
(
    iupal_t list,
    iupa_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
    list->pPatterns[index] = member;
    return IUS_E_OK;
}