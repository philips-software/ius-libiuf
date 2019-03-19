
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufPositionPrivate.h>
#include <iufTransducerPrivate.h>
#include <iuf3DTransducerElementListPrivate.h>

#include <iufTransducerADT.h>
#include <iuf3DTransducerADT.h>
#include <iuf3DTransducerElementListPrivate.h>

iu3dt_t iuf3DTransducerCreate
(
  char *name,
  IufTransducerShape shape,
  float centerFrequency,
  int numElements
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(name, IU3DT_INVALID);
    IUF_ERR_EVAL_N_RETURN(shape == IUF_INVALID_TRANSDUCER_SHAPE, IU3DT_INVALID);
    IUF_ERR_EVAL_N_RETURN(shape == IUF_LINE || shape == IUF_CIRCLE, IU3DT_INVALID);

    // NAN check
    IUF_ERR_EVAL_N_RETURN(centerFrequency != centerFrequency, IU3DT_INVALID);
    IUF_ERR_EVAL_N_RETURN(numElements < 0, IU3DT_INVALID);
    iu3dt_t created = calloc(1,sizeof(Iuf3DTransducer));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DTransducer, IU3DT_INVALID);
    created->elements = iuf3DTransducerElementListCreate(numElements);
    created->baseTransducer.pTransducerName = strdup(name);
    created->baseTransducer.shape = shape;
    created->baseTransducer.centerFrequency = centerFrequency;
    created->baseTransducer.type = IUF_3D_SHAPE;
    created->baseTransducer.deepDelete = IUF_FALSE;

    return created;
}

int iuf3DTransducerDeepDelete
(
    iu3dt_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    transducer->baseTransducer.deepDelete = IUF_TRUE;
    return iuf3DTransducerDelete(transducer);
}

int iuf3DTransducerDelete
(
    iu3dt_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    int numElements =  iuf3DTransducerElementListGetSize(transducer->elements);
    if (numElements > 0)
    {
        if (transducer->baseTransducer.deepDelete == IUF_TRUE)
        {
            iuf3DTransducerElementListDeepDelete(transducer->elements);

        }
        else
        {
            iuf3DTransducerElementListDelete(transducer->elements);
        }
    }
    free(transducer->baseTransducer.pTransducerName);
    free(transducer);
    return IUF_E_OK;
}



// operations
int iuf3DTransducerCompare
(
  iu3dt_t reference,
  iu3dt_t actual
)
{
  if( reference == actual ) return IUF_TRUE;
  if( reference == NULL || actual == NULL ) return IUF_FALSE;
  if( iuf3DTransducerElementListGetSize(reference->elements) !=
  iuf3DTransducerElementListGetSize(actual->elements) ) return IUF_FALSE;
  if( iuf3DTransducerElementListCompare(reference->elements,actual->elements) == IUF_FALSE )  return IUF_FALSE;
  return iufBaseTransducerCompare((iut_t) reference, (iut_t) actual);
}

// getters
iu3dte_t iuf3DTransducerGetElement
(
  iu3dt_t transducer,
  int index
)
{
  IUF_ERR_CHECK_NULL_N_RETURN(transducer, IU3DTE_INVALID);
  return iuf3DTransducerElementListGet(transducer->elements,index);
}

int iuf3DTransducerGetNumElements
(
  iu3dt_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, -1);
    return iuf3DTransducerElementListGetSize(transducer->elements);
}

IufTransducerShape iuf3DTransducerGetShape(
	iu3dt_t transducer
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transducer, -1);
	return transducer->baseTransducer.shape;
}

char *iuf3DTransducerGetName
(
	iu3dt_t transducer
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transducer, NULL);
	return transducer->baseTransducer.pTransducerName;
}

float iuf3DTransducerGetCenterFrequency
(
	iu3dt_t transducer
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(transducer, 0.0f);
	return transducer->baseTransducer.centerFrequency;
}


// setters
int iuf3DTransducerSetElement
(
  iu3dt_t transducer,
  int elementIndex,
  iu3dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(elementIndex >= iuf3DTransducerElementListGetSize(transducer->elements), IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(elementIndex < 0, IUF_ERR_VALUE);
    return iuf3DTransducerElementListSet(transducer->elements,element,elementIndex);
}

int iuf3DTransducerSetElementList
(
    iu3dt_t transducer,
    iu3dtel_t elementList
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(elementList, IUF_ERR_VALUE);
    if (transducer->elements != NULL) iuf3DTransducerElementListDelete(transducer->elements);
    transducer->elements = elementList;
    return IUF_E_OK;
}

herr_t iuf3DTransducerSave
(
    iu3dt_t transducer,
    hid_t handle
)
{
	herr_t status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status = iufBaseTransducerSave((iut_t)transducer, handle);
    if (status != 0)
        return status;

    status = iuf3DTransducerElementListSave(transducer->elements, handle);
//    status |= H5Gclose(handle);
    return status;
}


iu3dt_t iuf3DTransducerLoad
(
	hid_t handle
)
{
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DT_INVALID);
	iut_t baseTransducer = iufBaseTransducerLoad(handle);
	if (baseTransducer == IUT_INVALID) return IU3DT_INVALID;

	iu3dtel_t elements = iuf3DTransducerElementListLoad(handle);
	if (elements == IU3DTEL_INVALID) return IU3DT_INVALID;

	int numElements = iuf3DTransducerElementListGetSize(elements);
	iu3dt_t transducer = iuf3DTransducerCreate( baseTransducer->pTransducerName,
	                                              baseTransducer->shape,
	                                              baseTransducer->centerFrequency,
	                                              numElements);
    iuf3DTransducerSetElementList(transducer,elements);
    iufBaseTransducerDelete(baseTransducer);
    transducer->baseTransducer.deepDelete = IUF_TRUE;
    return transducer;
}

