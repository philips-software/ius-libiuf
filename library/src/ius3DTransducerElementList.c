//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <ius3DTransducerElementListImp.h>
#include <ius3DTransducerElementImp.h>
#include <include/iusHDF5.h>

// ADT
struct Ius3DTransducerElementList
{
    int count;
    iu3dte_t *   p3DTransducerElements ;
} ;

// ADT

iu3dtel_t ius3DTransducerElementListCreate
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
                ius3DTransducerElementListSet(list,IU3DTE_INVALID,i);
            }
        }
    }
    return list;
}

int ius3DTransducerElementListDelete
(
    iu3dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int ius3DTransducerElementListCompare
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
        if( ius3DTransducerElementCompare( reference->p3DTransducerElements[index], actual->p3DTransducerElements[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int ius3DTransducerElementListGetSize
(
    iu3dtel_t list
)
{
    if( list == NULL ) return -1;
    return list->count;
}

iu3dte_t ius3DTransducerElementListGet
(
    iu3dtel_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->p3DTransducerElements[index];
}

int ius3DTransducerElementListSet
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

IUS_BOOL ius3DTransducerElementListFull
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




#define ELEMENTLISTFMT "%s/Element[%d]"
#define LISTSIZEFMT "%s/Size"

iu3dtel_t ius3DTransducerElementListLoad
(
    hid_t handle,
    const char *parentPath
)
{
    char path[IUS_MAX_HDF5_PATH];
    int i,size;

    if(parentPath == NULL || handle == H5I_INVALID_HID)
        return IU3DTEL_INVALID;

    sprintf(path, LISTSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(handle, path, &(size));
    if(status <0)
        return IU3DTEL_INVALID;

    iu3dtel_t elementList = ius3DTransducerElementListCreate(size);
    iu3dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, ELEMENTLISTFMT, parentPath, i);
        loadedElement = ius3DTransducerElementLoad(handle,path);
        if(loadedElement == IU3DTE_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
        status = ius3DTransducerElementListSet(elementList,loadedElement,i);
        if( status == IUS_ERR_VALUE )
            break;
    }

    if( status == IUS_ERR_VALUE )
    {
        ius3DTransducerElementListDelete(elementList);
        elementList = IU3DTEL_INVALID;
    }
    return elementList;
}


int ius3DTransducerElementListSave
(
    iu3dtel_t list,
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
    if(ius3DTransducerElementListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu3dte_t sourceElement;
    sprintf(path, LISTSIZEFMT, parentPath);
    status |= iusHdf5WriteInt(handle, path, &(list->count), 1);

    // iterate over source list elements and save'em
    for (i=0;i < list->count ;i++)
    {
        sourceElement = ius3DTransducerElementListGet(list,i);
        if(sourceElement == IU3DTE_INVALID) continue;

        sprintf(path, ELEMENTLISTFMT, parentPath, i);
        status = ius3DTransducerElementSave(sourceElement,path,group_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id );
    return status;
}