
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusTransducerPrivate.h>
#include <iusPositionPrivate.h>
#include <ius2DTransducerElementListPrivate.h>

struct Ius2DTransducer
{
  struct IusTransducer     baseTransducer;
  iu2dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT
iu2dt_t ius2DTransducerCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
)
{
    if ( name == NULL ) return IU2DT_INVALID;
    if ( shape == IUS_INVALID_TRANSDUCER_SHAPE ) return IU2DT_INVALID;
    if ( shape != IUS_LINE && shape != IUS_CIRCLE) return IU2DT_INVALID;

    // NAN check
    if ( centerFrequency != centerFrequency ) return IU2DT_INVALID;
    if ( numElements < 0 ) return IU2DT_INVALID;

    iu2dt_t created = calloc(1,sizeof(Ius2DTransducer));
    created->elements = ius2DTransducerElementListCreate(numElements);
    created->baseTransducer.type = IUS_2D_SHAPE;
    created->baseTransducer.pTransducerName = strdup(name);
    created->baseTransducer.shape = shape;
    created->baseTransducer.centerFrequency = centerFrequency;
    created->baseTransducer.deepDelete = IUS_FALSE;
    return created;
}

int ius2DTransducerDeepDelete
(
    iu2dt_t ius2DTransducer
)
{
    if (ius2DTransducer == NULL) return IUS_ERR_VALUE;
    ius2DTransducer->baseTransducer.deepDelete = IUS_TRUE;
    return ius2DTransducerDelete(ius2DTransducer);
}

int ius2DTransducerDelete
(
	iu2dt_t ius2DTransducer
)
{
    if(ius2DTransducer == NULL) return IUS_ERR_VALUE;
    int numElements =  ius2DTransducerElementListGetSize(ius2DTransducer->elements);
    if (numElements > 0)
    {
        if (ius2DTransducer->baseTransducer.deepDelete == IUS_TRUE)
        {
            ius2DTransducerElementListDeepDelete(ius2DTransducer->elements);

        }
        else
        {
            ius2DTransducerElementListDelete(ius2DTransducer->elements);
        }
    }
    free(ius2DTransducer->baseTransducer.pTransducerName);
    free(ius2DTransducer);
    return IUS_E_OK;
}

// operations
int ius2DTransducerCompare
(
    iu2dt_t reference,
    iu2dt_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( ius2DTransducerElementListCompare(reference->elements,actual->elements) == IUS_FALSE )  return IUS_FALSE;
    return iusBaseTransducerCompare((iut_t) reference, (iut_t) actual);
}

// getters
iu2dte_t ius2DTransducerGetElement
(
    iu2dt_t transducer,
    int elementIndex
)
{
    if( transducer == NULL ) return IU2DTE_INVALID;
    return ius2DTransducerElementListGet(transducer->elements,elementIndex);
}

int ius2DTransducerGetNumElements
(
	iu2dt_t transducer
)
{
	if( transducer == NULL ) return -1;
	return ius2DTransducerElementListGetSize(transducer->elements);
}

// setters
int ius2DTransducerSetElement
(
    iu2dt_t transducer,
    int elementIndex,
    iu2dte_t element
)
{
    if( transducer == NULL ) return IUS_ERR_VALUE;
    if( elementIndex >= ius2DTransducerElementListGetSize(transducer->elements) ) return IUS_ERR_VALUE;
    if( elementIndex < 0 ) return IUS_ERR_VALUE;
    if( element == NULL ) return IUS_ERR_VALUE;
    return ius2DTransducerElementListSet(transducer->elements,element,elementIndex);
}

int ius2DTransducerSetElementList
(
    iu2dt_t transducer,
    iu2dtel_t elementList
)
{
    if( transducer == NULL ) return IUS_ERR_VALUE;
    if( elementList == NULL ) return IUS_ERR_VALUE;
    if( transducer->elements != NULL ) ius2DTransducerElementListDelete(transducer->elements);
    transducer->elements = elementList;
    return IUS_E_OK;
}

herr_t ius2DTransducerSave
(
    iu2dt_t transducer,
    hid_t handle
)
{
    herr_t status=0;
    status = iusBaseTransducerSave((iut_t)transducer, handle);
    if (status != 0)
        return status;

    status = ius2DTransducerElementListSave(transducer->elements, handle);
    return status;
}



iu2dt_t ius2DTransducerLoad
(
    hid_t handle
)
{
	iut_t baseTransducer = iusBaseTransducerLoad(handle);
    if (baseTransducer == IUT_INVALID) return IU2DT_INVALID;

    iu2dtel_t elements = ius2DTransducerElementListLoad(handle);
	if (elements == IU2DTEL_INVALID) return IU2DT_INVALID;
	int numElements = ius2DTransducerElementListGetSize(elements);
    iu2dt_t transducer = ius2DTransducerCreate( baseTransducer->pTransducerName,
                                                  baseTransducer->shape,
                                                  baseTransducer->centerFrequency,
                                                  numElements);
	if (transducer == IU2DT_INVALID) return IU2DT_INVALID;
    ius2DTransducerSetElementList(transducer,elements);
    transducer->baseTransducer.deepDelete = IUS_TRUE;
    iusBaseTransducerDelete(baseTransducer);
    return transducer;
}

