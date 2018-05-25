
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHLTransducer.h>
#include <iusHLTransducerImp.h>
#include <iusHL2DTransducer.h>
#include <iusHL2DTransducerElementList.h>



struct Ius2DTransducer
{
  struct IusTransducer     baseTransducer;
  iu2dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT

iu2dt_t iusHL2DTransducerCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
)
{
    if( numElements < 0 ) return IU2DT_INVALID;
    iu2dt_t created = calloc(1,sizeof(Ius2DTransducer));
    created->elements = iusHL2DTransducerElementListCreate(numElements);
    created->baseTransducer.type = IUS_2D_SHAPE;
    created->baseTransducer.pTransducerName = strdup(name);
    created->baseTransducer.shape = shape;
    created->baseTransducer.centerFrequency = centerFrequency;
    return created;
}


int iusHL2DTransducerDelete
(
iu2dt_t ius2DTransducer
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DTransducer != NULL)
    {
        free(ius2DTransducer);
        ius2DTransducer = NULL;
        status = IUS_E_OK;
    }
    return status;
}

// operations
int iusHL2DTransducerCompare
(
    iu2dt_t reference,
    iu2dt_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( iusHL2DTransducerElementListGetSize(reference->elements) !=
        iusHL2DTransducerElementListGetSize(actual->elements) ) return IUS_FALSE;
    if( iusHL2DTransducerElementListCompare(reference->elements,actual->elements) == IUS_FALSE )  return IUS_FALSE;
    return IUS_TRUE;
}

// getters
iu2dte_t iusHL2DTransducerGetElement
(
    iu2dt_t transducer,
    int elementIndex
)
{
    if( transducer == NULL ) return IU2DTE_INVALID;
    return iusHL2DTransducerElementListGet(transducer->elements,elementIndex);
}

int iusHL2DTransducerGetNumElements
(
  iu2dt_t transducer
)
{
  if( transducer == NULL ) return -1;
  return iusHL2DTransducerElementListGetSize(transducer->elements);
}

// setters
int iusHL2DTransducerSetElement
(
    iu2dt_t transducer,
    int elementIndex,
    iu2dte_t element
)
{
    if( transducer == NULL ) return IUS_TRUE;
    return iusHL2DTransducerElementListSet(transducer->elements,element,elementIndex);
}