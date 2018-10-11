
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusHDF5.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusTransducer.h>
#include <iusTransducerPrivate.h>
#include <ius2DTransducer.h>
#include <ius2DTransducerElement.h>
#include <ius2DTransducerElementList.h>
#include <ius3DTransducer.h>
#include <ius3DTransducerElement.h>
#include <ius3DTransducerElementList.h>
#include <iusPositionPrivate.h>
#include <include/ius2DTransducerElementListPrivate.h>

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
    created->baseTransducer.loadedFromFile = IUS_FALSE;
    return created;
}


int ius2DTransducerDelete
(
	iu2dt_t ius2DTransducer
)
{
    if(ius2DTransducer == NULL) return IUS_ERR_VALUE;
    if(ius2DTransducer->baseTransducer.loadedFromFile == IUS_TRUE)
    {
        ius2DTransducerElementListDeepDelete(ius2DTransducer->elements);
    }
    else
    {
        ius2DTransducerElementListDelete(ius2DTransducer->elements);
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

#if 0
int ius2DTransducerWriteElementPositions(Ius2DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t status = 0;
	hid_t position_tid; // File datatype identifier for IusPosition
	hid_t dataset, space;
	hsize_t dims[1] = { 1 };
	Ius2DPosition * pPositionArray;
	int numElements = ius2DTransducerElementListGetSize(pTransducer->elements);
	int i; //iterator

	/* write the /Transducer/Elements/ positions, angles and sizes are compound types */
	dims[0] = numElements;

	//Positions
	space = H5Screate_simple(1, dims, NULL);
	// step a:  create H5 dataset
	position_tid = H5Tcreate(H5T_COMPOUND, sizeof(Ius2DPosition));
	status |= H5Tinsert(position_tid, "x", HOFFSET(Ius2DPosition, x), H5T_NATIVE_FLOAT);
	status |= H5Tinsert(position_tid, "z", HOFFSET(Ius2DPosition, z), H5T_NATIVE_FLOAT);
	dataset = H5Dcreate(subgroup_id, "positions", position_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// step b:  create array of positions
	pPositionArray = (Ius2DPosition *)calloc(numElements, sizeof(Ius2DPosition)); //three dimensions for position
	for (i = 0; i < numElements; i++)
	{
		iu2dte_t element = ius2DTransducerGetElement(pTransducer, i);
		iu2dp_t elemPosition = ius2DTransducerElementGetPosition(element);
		pPositionArray[i].x = elemPosition->x;
		pPositionArray[i].z = elemPosition->z;
	}

	// step c: write the array to the dataset
	status |= H5Dwrite(dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPositionArray);

	free(pPositionArray);

	// step d: release resources
	status |= H5Tclose(position_tid);
	status |= H5Sclose(space);
	status |= H5Dclose(dataset);
	return status;
}

int ius2DTransducerWriteElementSizes(Ius2DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t        status = 0;
	hid_t size_tid; // File datatype identifier for IusPosition
	hid_t dataset, space;
	hsize_t dims[1] = { 1 };
	Ius2DSize * pSizeArray;
	int numElements = ius2DTransducerElementListGetSize(pTransducer->elements);
	int i; //iterator

		   //
	space = H5Screate_simple(1, dims, NULL);

	// step a:  create H5 dataset
	size_tid = H5Tcreate(H5T_COMPOUND, sizeof(Ius2DSize));
	status |= H5Tinsert(size_tid, "sx", HOFFSET(Ius2DSize, sx), H5T_NATIVE_FLOAT);
	status |= H5Tinsert(size_tid, "sz", HOFFSET(Ius2DSize, sz), H5T_NATIVE_FLOAT);
	dataset = H5Dcreate(subgroup_id, "sizes",
		size_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// step b:  create array of sizes
	pSizeArray = (Ius2DSize *)calloc(numElements, sizeof(Ius2DSize)); //three dimensions for size
	for (i = 0; i < numElements; i++)
	{
		iu2dte_t element = ius2DTransducerGetElement(pTransducer, i);
		iu2ds_t elemSize= ius2DTransducerElementGetSize(element);
		pSizeArray[i].sx = elemSize->sx;
		pSizeArray[i].sz = elemSize->sz;
	}
	// step c: write the array to the dataset
	status |= H5Dwrite(dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeArray);

	// step d: release resources
	free(pSizeArray);
	status |= H5Tclose(size_tid);
	status |= H5Sclose(space);
	status |= H5Dclose(dataset);
	return status;
}

int ius2DTransducerWriteElementAngles(Ius2DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t        status = 0;
	hid_t dataset, space;
	float * pAngleArray;
	const int numElements = ius2DTransducerElementListGetSize(pTransducer->elements);
	hsize_t dims[1] = { numElements }; // TODO: fix warning C4204: nonstandard extension used: non-constant aggregate initialize 
	int i; //iterator

    // Angles
	space = H5Screate_simple(1, dims, NULL);
	// step a:  create H5 dataset
	dataset = H5Dcreate(subgroup_id, "theta", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// step b:  create array of angles
	pAngleArray = (float *)calloc(numElements, sizeof(float)); //two dimensions for angle
	for (i = 0; i < numElements; i++)
	{
		 iu2dte_t element = ius2DTransducerGetElement(pTransducer, i);
		 pAngleArray[i] = ius2DTransducerElementGetAngle(element);
	}

	// step c: write the array to the dataset
	status |= H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleArray);

	free(pAngleArray);
	// step d: release resources
	status |= H5Sclose(space);
	status |= H5Dclose(dataset);
	return status;
}
#endif

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
    ius2DTransducerElementListDeepDelete(transducer->elements);
	transducer->elements = elements;
    iusBaseTransducerDelete(baseTransducer);
    return transducer;
}

