
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusPositionPrivate.h>
#include <iusTransducerPrivate.h>
#include <ius3DTransducerElementListPrivate.h>

struct Ius3DTransducer
{
  struct IusTransducer     baseTransducer;
  iu3dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT
iu3dt_t ius3DTransducerCreate
(
  char *name,
  IusTransducerShape shape,
  float centerFrequency,
  int numElements
)
{
    if ( name == NULL ) return IU3DT_INVALID;
    if ( shape == IUS_INVALID_TRANSDUCER_SHAPE ) return IU3DT_INVALID;
    if ( shape == IUS_LINE || shape == IUS_CIRCLE) return IU3DT_INVALID;

    // NAN check
    if ( centerFrequency != centerFrequency ) return IU3DT_INVALID;
    if ( numElements < 0 ) return IU3DT_INVALID;

    iu3dt_t created = calloc(1,sizeof(Ius3DTransducer));
    created->elements = ius3DTransducerElementListCreate(numElements);
    created->baseTransducer.pTransducerName = strdup(name);
    created->baseTransducer.shape = shape;
    created->baseTransducer.centerFrequency = centerFrequency;
    created->baseTransducer.type = IUS_3D_SHAPE;
    created->baseTransducer.loadedFromFile = IUS_FALSE;

    return created;
}


int ius3DTransducerDelete
(
    iu3dt_t ius3DTransducer
)
{
    if (ius3DTransducer == NULL) return IUS_ERR_VALUE;
    ius3DTransducerElementListDelete(ius3DTransducer->elements);
    free(ius3DTransducer->baseTransducer.pTransducerName);
    free(ius3DTransducer);
    return IUS_E_OK;
}



// operations
int ius3DTransducerCompare
(
  iu3dt_t reference,
  iu3dt_t actual
)
{
  if( reference == actual ) return IUS_TRUE;
  if( reference == NULL || actual == NULL ) return IUS_FALSE;
  if( ius3DTransducerElementListGetSize(reference->elements) !=
  ius3DTransducerElementListGetSize(actual->elements) ) return IUS_FALSE;
  if( ius3DTransducerElementListCompare(reference->elements,actual->elements) == IUS_FALSE )  return IUS_FALSE;
  return iusBaseTransducerCompare((iut_t) reference, (iut_t) actual);
}

// getters
iu3dte_t ius3DTransducerGetElement
(
  iu3dt_t transducer,
  int elementIndex
)
{
  if( transducer == NULL ) return IU3DTE_INVALID;
  return ius3DTransducerElementListGet(transducer->elements,elementIndex);
}

int ius3DTransducerGetNumElements
(
  iu3dt_t transducer
)
{
  if( transducer == NULL ) return -1;
  return ius3DTransducerElementListGetSize(transducer->elements);
}

// setters
int ius3DTransducerSetElement
(
  iu3dt_t transducer,
  int elementIndex,
  iu3dte_t element
)
{
  if( transducer == NULL ) return IUS_ERR_VALUE;
  if( elementIndex >= ius3DTransducerElementListGetSize(transducer->elements) ) return IUS_ERR_VALUE;
  if( elementIndex < 0 ) return IUS_ERR_VALUE;
  if( element == NULL ) return IUS_ERR_VALUE;

  return ius3DTransducerElementListSet(transducer->elements,element,elementIndex);
}

int ius3DTransducerSetElementList
(
    iu3dt_t transducer,
    iu3dtel_t elementList
)
{
    if (transducer == NULL) return IUS_ERR_VALUE;
    if (elementList == NULL) return IUS_ERR_VALUE;
    if (transducer->elements != NULL) ius3DTransducerElementListDelete(transducer->elements);
    transducer->elements = elementList;
    return IUS_E_OK;
}

herr_t ius3DTransducerSave
(
    iu3dt_t transducer,
    hid_t handle
)
{
	herr_t status=0;

    status = iusBaseTransducerSave((iut_t)transducer, handle);
    if (status != 0)
        return status;

    status = ius3DTransducerElementListSave(transducer->elements, handle);
    return status;
}


iu3dt_t ius3DTransducerLoad
(
	hid_t handle
)
{
	iut_t baseTransducer = iusBaseTransducerLoad(handle);
	if (baseTransducer == IUT_INVALID) return IU3DT_INVALID;

	iu3dtel_t elements = ius3DTransducerElementListLoad(handle);
	if (elements == IU3DTEL_INVALID) return IU3DT_INVALID;

	int numElements = ius3DTransducerElementListGetSize(elements);
	iu3dt_t transducer = ius3DTransducerCreate( baseTransducer->pTransducerName,
	                                              baseTransducer->shape,
	                                              baseTransducer->centerFrequency,
	                                              numElements);
    ius3DTransducerSetElementList(transducer,elements);
    iusBaseTransducerDelete(baseTransducer);
    return transducer;
}

