
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufTransducerPrivate.h>
#include <iuf2DTransducerADT.h>
#include <iufPositionPrivate.h>
#include <iuf2DTransducerElementListPrivate.h>


// ADT
iu2dt_t iuf2DTransducerCreate
(
    char *name,
    IufTransducerShape shape,
    float centerFrequency,
    int numElements
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(name, IU2DT_INVALID);
    IUF_ERR_EVAL_N_RETURN(shape == IUF_INVALID_TRANSDUCER_SHAPE, IU2DT_INVALID);
    IUF_ERR_EVAL_N_RETURN(shape != IUF_LINE && shape != IUF_CIRCLE, IU2DT_INVALID);

    // NAN check
    IUF_ERR_EVAL_N_RETURN(centerFrequency != centerFrequency, IU2DT_INVALID);
    IUF_ERR_EVAL_N_RETURN(numElements < 0, IU2DT_INVALID);

    iu2dt_t created = calloc(1,sizeof(Iuf2DTransducer));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf2DTransducer, IU2DT_INVALID);

    created->elements = iuf2DTransducerElementListCreate(numElements);
    created->baseTransducer.type = IUF_2D_SHAPE;
    created->baseTransducer.pTransducerName = strdup(name);
    created->baseTransducer.shape = shape;
    created->baseTransducer.centerFrequency = centerFrequency;
    created->baseTransducer.deepDelete = IUF_FALSE;
    return created;
}

int iuf2DTransducerDeepDelete
(
    iu2dt_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    transducer->baseTransducer.deepDelete = IUF_TRUE;
    return iuf2DTransducerDelete(transducer);
}

int iuf2DTransducerDelete
(
	iu2dt_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    int numElements =  iuf2DTransducerElementListGetSize(transducer->elements);
    if (numElements > 0)
    {
        if (transducer->baseTransducer.deepDelete == IUF_TRUE)
        {
            iuf2DTransducerElementListDeepDelete(transducer->elements);

        }
        else
        {
            iuf2DTransducerElementListDelete(transducer->elements);
        }
    }
    free(transducer->baseTransducer.pTransducerName);
    free(transducer);
    return IUF_E_OK;
}

// operations
int iuf2DTransducerCompare
(
    iu2dt_t reference,
    iu2dt_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( iuf2DTransducerElementListCompare(reference->elements,actual->elements) == IUF_FALSE )  return IUF_FALSE;
    return iufBaseTransducerCompare((iut_t) reference, (iut_t) actual);
}

// getters
iu2dte_t iuf2DTransducerGetElement
(
    iu2dt_t transducer,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IU2DTE_INVALID);
    return iuf2DTransducerElementListGet(transducer->elements,index);
}

int iuf2DTransducerGetNumElements
(
	iu2dt_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, -1);
	return iuf2DTransducerElementListGetSize(transducer->elements);
}

IufTransducerShape iuf2DTransducerGetShape(
	iu2dt_t transducer
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transducer, -1);
	return transducer->baseTransducer.shape;
}

char *iuf2DTransducerGetName
(
	iu2dt_t transducer
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transducer, NULL);
	return transducer->baseTransducer.pTransducerName;
}

float iuf2DTransducerGetCenterFrequency
(
	iu2dt_t transducer
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transducer, 0.0f);
	return transducer->baseTransducer.centerFrequency;
}

// setters
int iuf2DTransducerSetElement
(
    iu2dt_t transducer,
    int index,
    iu2dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index >= iuf2DTransducerElementListGetSize(transducer->elements), IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0, IUF_ERR_VALUE);
    return iuf2DTransducerElementListSet(transducer->elements,element,index);
}

int iuf2DTransducerSetElementList
(
    iu2dt_t transducer,
    iu2dtel_t elementList
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(elementList, IUF_ERR_VALUE);
    if( transducer->elements != NULL ) iuf2DTransducerElementListDelete(transducer->elements);
    transducer->elements = elementList;
    return IUF_E_OK;
}

herr_t iuf2DTransducerSave
(
    iu2dt_t transducer,
    hid_t handle
)
{
    herr_t status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status = iufBaseTransducerSave((iut_t)transducer, handle);
    if (status != 0)
        return status;

    status = iuf2DTransducerElementListSave(transducer->elements, handle);
//    status |= H5Gclose(handle);
    return status;
}



iu2dt_t iuf2DTransducerLoad
(
    hid_t handle
)
{
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DT_INVALID);
	iut_t baseTransducer = iufBaseTransducerLoad(handle);
    if (baseTransducer == IUT_INVALID) return IU2DT_INVALID;

    iu2dtel_t elements = iuf2DTransducerElementListLoad(handle);
	if (elements == IU2DTEL_INVALID) return IU2DT_INVALID;
	int numElements = iuf2DTransducerElementListGetSize(elements);
    iu2dt_t transducer = iuf2DTransducerCreate( baseTransducer->pTransducerName,
                                                  baseTransducer->shape,
                                                  baseTransducer->centerFrequency,
                                                  numElements);
	if (transducer == IU2DT_INVALID) return IU2DT_INVALID;
    iuf2DTransducerSetElementList(transducer,elements);
    transducer->baseTransducer.deepDelete = IUF_TRUE;
    iufBaseTransducerDelete(baseTransducer);
    return transducer;
}

