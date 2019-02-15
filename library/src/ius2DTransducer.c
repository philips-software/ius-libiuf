
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
    IUS_ERR_CHECK_NULL_N_RETURN(name, IU2DT_INVALID);
    IUS_ERR_EVAL_N_RETURN(shape == IUS_INVALID_TRANSDUCER_SHAPE, IU2DT_INVALID);
    IUS_ERR_EVAL_N_RETURN(shape != IUS_LINE && shape != IUS_CIRCLE, IU2DT_INVALID);

    // NAN check
    IUS_ERR_EVAL_N_RETURN(centerFrequency != centerFrequency, IU2DT_INVALID);
    IUS_ERR_EVAL_N_RETURN(numElements < 0, IU2DT_INVALID);

    iu2dt_t created = calloc(1,sizeof(Ius2DTransducer));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius2DTransducer, IU2DT_INVALID);

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
    iu2dt_t transducer
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    transducer->baseTransducer.deepDelete = IUS_TRUE;
    return ius2DTransducerDelete(transducer);
}

int ius2DTransducerDelete
(
	iu2dt_t transducer
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    int numElements =  ius2DTransducerElementListGetSize(transducer->elements);
    if (numElements > 0)
    {
        if (transducer->baseTransducer.deepDelete == IUS_TRUE)
        {
            ius2DTransducerElementListDeepDelete(transducer->elements);

        }
        else
        {
            ius2DTransducerElementListDelete(transducer->elements);
        }
    }
    free(transducer->baseTransducer.pTransducerName);
    free(transducer);
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
    int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IU2DTE_INVALID);
    return ius2DTransducerElementListGet(transducer->elements,index);
}

int ius2DTransducerGetNumElements
(
	iu2dt_t transducer
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, -1);
	return ius2DTransducerElementListGetSize(transducer->elements);
}

IusTransducerShape ius2DTransducerGetShape(
	iu2dt_t transducer
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(transducer, -1);
	return transducer->baseTransducer.shape;
}

char *ius2DTransducerGetName
(
	iu2dt_t transducer
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(transducer, NULL);
	return transducer->baseTransducer.pTransducerName;
}

float ius2DTransducerGetCenterFrequency
(
	iu2dt_t transducer
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(transducer, 0.0f);
	return transducer->baseTransducer.centerFrequency;
}

// setters
int ius2DTransducerSetElement
(
    iu2dt_t transducer,
    int index,
    iu2dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(index >= ius2DTransducerElementListGetSize(transducer->elements), IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(index < 0, IUS_ERR_VALUE);
    return ius2DTransducerElementListSet(transducer->elements,element,index);
}

int ius2DTransducerSetElementList
(
    iu2dt_t transducer,
    iu2dtel_t elementList
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(elementList, IUS_ERR_VALUE);
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
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
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
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DT_INVALID);
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

