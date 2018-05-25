//
// Created by nlv09165 on 24/04/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusHLApodizationList.h>


// ADT
struct IusApodizationList
{
    int numApodizations;
    int numElements;
    float *pApodizations;
}  ;


float *iusCreateTransmitApodizations
(
int numApodizations,
int numElements
)
{
    float *pTransmitApodization = (float *)calloc( (size_t) numApodizations * numElements, sizeof(float) );
    if( pTransmitApodization == NULL ) return NULL;
    for (int i = 0; i < numElements*numApodizations; ++i)
    {
        pTransmitApodization[i] = 1.0f;
    }
    return pTransmitApodization;
}

iual_t iusHLApodizationListCreate
(
int numApodizations,
int numElements
)
{
    if( numApodizations < 0 ) return IUAL_INVALID;
    if( numElements < 0 ) return IUAL_INVALID;
    iual_t created = calloc(1,sizeof(IusApodizationList));
    if( created == NULL ) return IUAL_INVALID;
    created->pApodizations = iusCreateTransmitApodizations(numApodizations,numElements);
    if( created->pApodizations == NULL ) return IUAL_INVALID;
    created->numApodizations = numApodizations;
    created->numElements = numElements;
    return created;
}

int iusHLApodizationListDelete
(
iual_t iusApodizationList
)
{
    int status = IUS_ERR_VALUE;
    if(iusApodizationList != NULL)
    {
        free(iusApodizationList);
        iusApodizationList = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLApodizationListCompare
(
iual_t reference,
iual_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->numApodizations != actual->numApodizations ) return IUS_FALSE;
    if( reference->numElements != actual->numElements ) return IUS_FALSE;

    int index;
    int numValues = actual->numApodizations*actual->numElements;
    for(index = 0 ; index < numValues; index++)
    {
        if(IUS_EQUAL_FLOAT(reference->pApodizations[index],actual->pApodizations[index]) == IUS_FALSE)
            return IUS_FALSE;
    }
    return IUS_TRUE;
}

// Getters
int iusHLApodizationListGetNumApodizations
(
iual_t iusApodizationList
)
{
    if(iusApodizationList == NULL)
    {
        return -1;
    }
    return iusApodizationList->numApodizations;
}

int iusHLApodizationListGetNumElements
(
iual_t iusApodizationList
)
{
    if(iusApodizationList == NULL)
    {
        return -1;
    }
    return iusApodizationList->numElements;
}

float iusHLApodizationListGetApodization
(
iual_t iusApodizationList,
int listIndex,
int elementIndex
)
{
    if(iusApodizationList == NULL)
        return NAN;
    if( listIndex >= iusApodizationList->numApodizations || listIndex < 0 )
        return NAN;
    if( elementIndex >= iusApodizationList->numElements || elementIndex < 0 )
        return NAN;
    return( iusApodizationList->pApodizations[listIndex*iusApodizationList->numElements+elementIndex]);

}

// Setters
int iusHLApodizationListSetApodization
(
iual_t iusApodizationList,
int listIndex,
int elementIndex,
float apodization
)
{
    if(iusApodizationList == NULL)
        return IUS_ERR_VALUE;
    if( listIndex >= iusApodizationList->numApodizations || listIndex < 0 )
        return IUS_ERR_VALUE;
    if( elementIndex >= iusApodizationList->numElements || elementIndex < 0 )
        return IUS_ERR_VALUE;
    if( apodization < 0.0f || apodization > 1.0f )
        return IUS_ERR_VALUE;
    iusApodizationList->pApodizations[listIndex*iusApodizationList->numElements+elementIndex]=apodization;
    return IUS_E_OK;
}

