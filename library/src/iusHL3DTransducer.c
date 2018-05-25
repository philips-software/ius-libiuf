
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <include/iusHL3DTransducerElementList.h>
#include <include/iusHLTransducer.h>
#include <string.h>
#include "include/iusHL3DTransducer.h"
#include "include/iusHLTransducerImp.h"

struct Ius3DTransducer
{
  struct IusTransducer     baseTransducer;
  iu3dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT

iu3dt_t iusHL3DTransducerCreate
(
  char *name,
  IusTransducerShape shape,
  float centerFrequency,
  int numElements
)
{
  if( numElements < 0 ) return IU3DT_INVALID;
  iu3dt_t created = calloc(1,sizeof(Ius3DTransducer));
  created->elements = iusHL3DTransducerElementListCreate(numElements);
  created->baseTransducer.pTransducerName = strdup(name);
  created->baseTransducer.shape = shape;
  created->baseTransducer.centerFrequency = centerFrequency;
  created->baseTransducer.type = IUS_3D_SHAPE;

  return created;
}


int iusHL3DTransducerDelete
(
iu3dt_t ius3DTransducer
)
{
  int status = IUS_ERR_VALUE;
  if(ius3DTransducer != NULL)
  {
    free(ius3DTransducer);
    ius3DTransducer = NULL;
    status = IUS_E_OK;
  }
  return status;
}


// operations
int iusHL3DTransducerCompare
(
  iu3dt_t reference,
  iu3dt_t actual
)
{
  if( reference == actual ) return IUS_TRUE;
  if( reference == NULL || actual == NULL ) return IUS_FALSE;
  if( iusHL3DTransducerElementListGetSize(reference->elements) !=
  iusHL3DTransducerElementListGetSize(actual->elements) ) return IUS_FALSE;
  if( iusHL3DTransducerElementListCompare(reference->elements,actual->elements) == IUS_FALSE )  return IUS_FALSE;
  return IUS_TRUE;
}

// getters
iu3dte_t iusHL3DTransducerGetElement
(
  iu3dt_t transducer,
  int elementIndex
)
{
  if( transducer == NULL ) return IU3DTE_INVALID;
  return iusHL3DTransducerElementListGet(transducer->elements,elementIndex);
}

int iusHL3DTransducerGetNumElements
(
  iu3dt_t transducer
)
{
  if( transducer == NULL ) return -1;
  return iusHL3DTransducerElementListGetSize(transducer->elements);
}

// setters
int iusHL3DTransducerSetElement
(
  iu3dt_t transducer,
  int elementIndex,
  iu3dte_t element
)
{
  if( transducer == NULL ) return IUS_TRUE;
  return iusHL3DTransducerElementListSet(transducer->elements,element,elementIndex);
}