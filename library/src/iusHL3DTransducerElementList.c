//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHL3DTransducerElementListImp.h>
#include <iusHL3DTransducerElementImp.h>
#include <include/iusHDF5.h>

// ADT
struct Ius3DTransducerElementList
{
    int count;
    iu3dte_t *   p3DTransducerElements ;
} ;

// ADT

iu3dtel_t iusHL3DTransducerElementListCreate
(
    int num3DTransducerElements
)
{
    int i;
    iu3dtel_t list = calloc(1, sizeof(Ius3DTransducerElementList));
    if(list!=NULL)
    {
        list->count = num3DTransducerElements;
        list->p3DTransducerElements = (iu3dte_t *) calloc((size_t)num3DTransducerElements, sizeof(iu3dte_t));
        if( list->p3DTransducerElements == NULL )
        {
            free(list);
            list = NULL;
        }
        else
        {
            for (i=0;i < list->count ;i++)
            {
                iusHL3DTransducerElementListSet(list,IU3DTE_INVALID,i);
            }
        }
    }
    return list;
}

int iusHL3DTransducerElementListDelete
(
    iu3dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHL3DTransducerElementListCompare
(
    iu3dtel_t reference,
    iu3dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusHL3DTransducerElementCompare( reference->p3DTransducerElements[index], actual->p3DTransducerElements[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHL3DTransducerElementListGetSize
(
    iu3dtel_t list
)
{
    if( list == NULL ) return -1;
    return list->count;
}

iu3dte_t iusHL3DTransducerElementListGet
(
    iu3dtel_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->p3DTransducerElements[index];
}

int iusHL3DTransducerElementListSet
(
    iu3dtel_t list,
    iu3dte_t member,
    int index
)
{
    if( list == NULL || member == NULL ) return IUS_ERR_VALUE;
    if( index < 0 ) return IUS_ERR_VALUE;
    if( index >= list->count ) return IUS_ERR_VALUE;
    list->p3DTransducerElements[index] = member;
    return IUS_E_OK;
}

IUS_BOOL iusHL3DTransducerElementListFull
(
    iu3dtel_t list
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < list->count;i++)
    {
        if(list->p3DTransducerElements[i] == IU3DTE_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}




#define ELEMENTLISTFMT "Element[%d]"
#define LISTSIZEFMT "%s/Size"

iu3dtel_t iusHL3DTransducerElementListLoad
(
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int i,size;

	hid_t elements_id = H5Gopen(handle, "Elements", H5P_DEFAULT);
    
	if(handle == H5I_INVALID_HID || elements_id == H5I_INVALID_HID)
        return IU3DTEL_INVALID;

    //sprintf(path, LISTSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(elements_id, "Size", &(size));
    if(status <0)
        return IU3DTEL_INVALID;

    iu3dtel_t elementList = iusHL3DTransducerElementListCreate(size);
    iu3dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, ELEMENTLISTFMT, i); 
		hid_t element_id = H5Gopen(elements_id, path, H5P_DEFAULT);
        loadedElement = iusHL3DTransducerElementLoad(element_id); //THIS NEEDS TO BE GROUP_ID
        if(loadedElement == IU3DTE_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
        status = iusHL3DTransducerElementListSet(elementList,loadedElement,i);
        if( status == IUS_ERR_VALUE )
            break;
    }

    if( status == IUS_ERR_VALUE )
    {
        iusHL3DTransducerElementListDelete(elementList);
        elementList = IU3DTEL_INVALID;
    }
    return elementList;
}


int iusHL3DTransducerElementListSave
(
    iu3dtel_t list,
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
    if(iusHL3DTransducerElementListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;

    hid_t group_id = H5Gcreate(handle, "Elements", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu3dte_t sourceElement;
    //sprintf(path, LISTSIZEFMT, parentPath);
    status |= iusHdf5WriteInt(group_id, "Size", &(list->count), 1); //TODO centralize this string

    // iterate over source list elements and save'em
    for (i=0;i < list->count ;i++)
    {
        sourceElement = iusHL3DTransducerElementListGet(list,i);
        if(sourceElement == IU3DTE_INVALID) continue;

        sprintf(path, ELEMENTLISTFMT, i);
		hid_t singleElementId = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iusHL3DTransducerElementSave(sourceElement, singleElementId);
		H5Gclose(singleElementId);

        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
