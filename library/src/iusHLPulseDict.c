//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <math.h>

#include <uthash.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLPulseImp.h>
#include <iusHLPulseDict.h>

// ADT
struct HashablePulse
{
    iup_t pulse;
    UT_hash_handle hh;         /* makes this structure hashable */
} ;

typedef struct HashablePulse HashablePulse;

struct IusPulseDict
{
    HashablePulse *   pPulses ;
} ;


// ADT
iupd_t iusHLPulseDictCreate
(
)
{
    iupd_t list = calloc(1, sizeof(IusPulseDict));
    if(list!=NULL)
    {
        list->pPulses = NULL;
    }
    return list;
}

int iusHLPulseDictDelete
(
iupd_t dict
)
{
    HashablePulse *el;
    HashablePulse *tmp;
    if(dict == NULL) return IUS_ERR_VALUE;
    HASH_ITER(hh, dict->pPulses, el, tmp) {
        HASH_DEL(dict->pPulses, el);
    }

    free(dict);
    return IUS_E_OK;
}


static int iusHLPulseDictSourceInTarget
(
    iupd_t source,
    iupd_t target
)
{
    HashablePulse *sourceElement;
    HashablePulse *targetElement;
    IUS_BOOL sourceInTarget = IUS_FALSE;

    // iterate over source list elements using the hash double linked list
    for(sourceElement=source->pPulses; sourceElement != NULL; sourceElement=sourceElement->hh.next) {
        // lookup source item
        HASH_FIND_STR( target->pPulses, sourceElement->pulse->label, targetElement);
        if( targetElement == NULL)
            return IUS_FALSE;
        if( iusHLPulseCompare(sourceElement->pulse,targetElement->pulse) == IUS_FALSE )
            return IUS_FALSE;
        sourceInTarget = IUS_TRUE;
    }
    return sourceInTarget;
}

// operations
int iusHLPulseDictCompare
(
iupd_t reference,
iupd_t actual
)
{
    HashablePulse *referenceElement;
    HashablePulse *actualElement;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->pPulses == actual->pPulses) return IUS_TRUE;

    // check if elements of source hash are in target hash
    if( iusHLPulseDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusHLPulseDictSourceInTarget(actual,reference);
}


int iusHLPulseDictGetSize
(
    iupd_t dict
)
{
    return HASH_COUNT(dict->pPulses);
}

iup_t iusHLPulseDictGet
(
    iupd_t dict,
    char * key
)
{
    HashablePulse * search;
    if( dict == NULL ) return NULL;
    HASH_FIND_STR( dict->pPulses, key, search);
    if( search != NULL ) return NULL;
    return search->pulse;
}

int iusHLPulseDictSet
(
    iupd_t dict,
    iup_t member
)
{
    HashablePulse *newMember = calloc(1, sizeof(HashablePulse));
    newMember->pulse = member;
    char *label = iusHLPulseGetLabel(member);
    if( dict == NULL  ) return IUS_ERR_VALUE;
    HASH_ADD_KEYPTR( hh, dict->pPulses, label, strlen(label), newMember );
    return IUS_E_OK;
}