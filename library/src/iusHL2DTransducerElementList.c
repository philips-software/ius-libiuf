


//
// Created by nlv09165 on 23/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHL2DTransducerElementList.h>
#include <include/iusHL2DTransducerElementImp.h>

// ADT
struct Ius2DTransducerElementList
{
    int count;
    iu2dte_t *   p2DTransducerElements ;
} ;

// ADT

iu2dtel_t iusHL2DTransducerElementListCreate
(
    int num2DTransducerElements
)
{
    iu2dtel_t list = calloc(1, sizeof(Ius2DTransducerElementList));
    if(list!=NULL)
    {
        list->count = num2DTransducerElements;
        list->p2DTransducerElements = (iu2dte_t *) calloc((size_t)num2DTransducerElements, sizeof(iu2dte_t));
        if( list->p2DTransducerElements == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusHL2DTransducerElementListDelete
(
    iu2dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHL2DTransducerElementListCompare
(
    iu2dtel_t reference,
    iu2dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusHL2DTransducerElementCompare( reference->p2DTransducerElements[index], actual->p2DTransducerElements[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHL2DTransducerElementListGetSize
(
    iu2dtel_t list
)
{
    return list->count;
}

iu2dte_t iusHL2DTransducerElementListGet
(
    iu2dtel_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->p2DTransducerElements[index];
}

int iusHL2DTransducerElementListSet
(
    iu2dtel_t list,
    iu2dte_t member,
    int index
)
{
    if( list == NULL || member == NULL ) return IUS_ERR_VALUE;
    if( index >= list->count ) return IUS_ERR_VALUE;
    if( index < 0 ) return IUS_ERR_VALUE;
    list->p2DTransducerElements[index] = member;
    return IUS_E_OK;
}

IUS_BOOL iusHL2DTransducerElementListFull
(
    iu2dtel_t list
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < list->count;i++)
    {
        if(list->p2DTransducerElements[i] == IU2DTE_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}



#define ELEMENTLISTFMT "%s/Element[%d]"
#define LISTSIZEFMT "%s/Size"

iu2dtel_t iusHL2DTransducerElementListLoad
(
hid_t handle,
const char *parentPath
)
{
    char path[IUS_MAX_HDF5_PATH];
    int i,size;

    if(parentPath == NULL || handle == H5I_INVALID_HID)
        return IU2DTEL_INVALID;

    sprintf(path, LISTSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(handle, path, &(size));
    if(status <0)
        return IU2DTEL_INVALID;

    iu2dtel_t elementList = iusHL2DTransducerElementListCreate(size);
    iu2dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, ELEMENTLISTFMT, parentPath, i);
        loadedElement = iusHL2DTransducerElementLoad(handle,path);
        if(loadedElement == IU2DTE_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
        status = iusHL2DTransducerElementListSet(elementList,loadedElement,i);
        if( status == IUS_ERR_VALUE )
            break;
    }

    if( status == IUS_ERR_VALUE )
    {
        iusHL2DTransducerElementListDelete(elementList);
        elementList = IU2DTEL_INVALID;
    }
    return elementList;
}


int iusHL2DTransducerElementListSave
(
iu2dtel_t list,
const char *parentPath,
hid_t handle
)
{
    int status=0;
    int i;
    char path[IUS_MAX_HDF5_PATH];

    if(list == NULL)
        return IUS_ERR_VALUE;
    if(parentPath == NULL || handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;
    if(iusHL2DTransducerElementListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu2dte_t sourceElement;
    sprintf(path, LISTSIZEFMT, parentPath);
    status |= iusHdf5WriteInt(handle, path, &(list->count), 1);

    // iterate over source list elements and save'em
    for (i=0;i < list->count ;i++)
    {
        sourceElement = iusHL2DTransducerElementListGet(list,i);
        if(sourceElement == IU2DTE_INVALID) continue;

        sprintf(path, ELEMENTLISTFMT, parentPath, i);
        status = iusHL2DTransducerElementSave(sourceElement,path,group_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id );
    return status;
}
