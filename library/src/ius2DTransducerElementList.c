//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <ius2DTransducerElementPrivate.h>

// ADT
struct Ius2DTransducerElementList
{
    int numElements;
    iu2dte_t *   p2DTransducerElements ;
    IUS_BOOL loadedFromFile;
} ;

// ADT
iu2dtel_t ius2DTransducerElementListCreate
(
    int num2DTransducerElements
)
{
    int i;
    iu2dtel_t list = calloc(1, sizeof(Ius2DTransducerElementList));
    if(list!=NULL)
    {
        list->loadedFromFile = IUS_FALSE;
        list->numElements = num2DTransducerElements;
        list->p2DTransducerElements = (iu2dte_t *) calloc((size_t)num2DTransducerElements, sizeof(iu2dte_t));
        if( list->p2DTransducerElements == NULL )
        {
            free(list);
            list = NULL;
        }
        else
        {
            for(i = 0 ; i < list->numElements ; i++ )
            {
                ius2DTransducerElementListSet(list,IU2DTE_INVALID,i);
            }
        }
    }
    return list;
}


int ius2DTransducerElementListDeepDelete
(
    iu2dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    list->loadedFromFile = IUS_TRUE;
    return ius2DTransducerElementListDelete(list);
}

int ius2DTransducerElementListDelete
(
    iu2dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    if(list->loadedFromFile == IUS_TRUE)
    {
        for (int i = 0 ; i < list->numElements ; i++ )
        {
            ius2DTransducerElementDelete(list->p2DTransducerElements[i]);
        }
    }
    free(list->p2DTransducerElements);
    free(list);
    return IUS_E_OK;
}

// operations
int ius2DTransducerElementListCompare
(
    iu2dtel_t reference,
    iu2dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->numElements != actual->numElements ) return IUS_FALSE;
    for(index = 0 ; index < actual->numElements ; index++ )
    {
        if( ius2DTransducerElementCompare( reference->p2DTransducerElements[index], actual->p2DTransducerElements[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int ius2DTransducerElementListGetSize
(
    iu2dtel_t list
)
{
    if( list == NULL ) return -1;
    return list->numElements;
}

iu2dte_t ius2DTransducerElementListGet
(
    iu2dtel_t list,
    int index
)
{
    if( list == NULL ) return NULL;
    if( index >= list->numElements ) return NULL;
    if( index < 0 ) return NULL;
    return list->p2DTransducerElements[index];
}

int ius2DTransducerElementListSet
(
    iu2dtel_t list,
    iu2dte_t member,
    int index
)
{
    if( list == NULL || member == NULL ) return IUS_ERR_VALUE;
    if( index >= list->numElements ) return IUS_ERR_VALUE;
    if( index < 0 ) return IUS_ERR_VALUE;
    list->p2DTransducerElements[index] = member;
    return IUS_E_OK;
}

IUS_BOOL ius2DTransducerElementListFull
(
    iu2dtel_t list
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < list->numElements;i++)
    {
        if(list->p2DTransducerElements[i] == IU2DTE_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}

iu2dtel_t ius2DTransducerElementListLoad
(
	hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int i,size;

    if(handle == H5I_INVALID_HID) return IU2DTEL_INVALID;
	
	hid_t elements_id = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, H5P_DEFAULT);
	if (elements_id == H5I_INVALID_HID)
		return IU2DTEL_INVALID;

    int status = iusHdf5ReadInt(elements_id, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(size));
    if(status <0)
        return IU2DTEL_INVALID;

    iu2dtel_t elementList = ius2DTransducerElementListCreate(size);
    iu2dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
		hid_t element_single_id = H5Gopen(elements_id, path, H5P_DEFAULT);
        loadedElement = ius2DTransducerElementLoad(element_single_id);
        if(loadedElement == IU2DTE_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
		H5Gclose(element_single_id);
        status = ius2DTransducerElementListSet(elementList,loadedElement, i);
        if( status == IUS_ERR_VALUE )
            break;
    }

	H5Gclose(elements_id);
    elementList->loadedFromFile = IUS_TRUE;
    if( status == IUS_ERR_VALUE )
    {
        ius2DTransducerElementListDelete(elementList);
        elementList = IU2DTEL_INVALID;
    }
    return elementList;
}


int ius2DTransducerElementListSave
(
    iu2dtel_t list,
    hid_t handle
)
{
    int status=0;
    int i;
    char path[IUS_MAX_HDF5_PATH];

    if(list == NULL)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;
    if(ius2DTransducerElementListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;

    iu2dte_t sourceElement;
	hid_t elements_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusHdf5WriteInt(elements_id, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(list->numElements), 1);

    // iterate over source list elements and save'em
    for (i=0; i<list->numElements; i++)
    {
        sourceElement = ius2DTransducerElementListGet(list,i);
        if(sourceElement == IU2DTE_INVALID) continue;

        sprintf(path, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
		hid_t element_single_id = H5Gcreate(elements_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = ius2DTransducerElementSave(sourceElement, element_single_id);
		H5Gclose(element_single_id);
        if(status != IUS_E_OK) break;
    }
	H5Gclose(elements_id);
    return status;
}
