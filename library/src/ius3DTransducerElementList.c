//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <ius3DTransducerElementPrivate.h>
#include <ius3DTransducerElementListPrivate.h>

struct Ius3DTransducerElementList
{
    int numElements;
    iu3dte_t *   p3DTransducerElements ;
    IUS_BOOL deepDelete;
} ;

// ADT
iu3dtel_t ius3DTransducerElementListCreate
(
    int numElements
)
{
    int i;
    IUS_ERR_EVAL_N_RETURN(numElements<=0, IU3DTEL_INVALID);
    iu3dtel_t list = calloc(1, sizeof(Ius3DTransducerElementList));
    IUS_ERR_ALLOC_NULL_N_RETURN(list, Ius3DTransducerElementList, IU3DTEL_INVALID);
    list->deepDelete = IUS_FALSE;
    list->numElements = numElements;
    list->p3DTransducerElements = (iu3dte_t *) calloc((size_t)numElements, sizeof(iu3dte_t));
    if( list->p3DTransducerElements == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for p3DTransducerElements member");
        free(list);
        list = IU3DTEL_INVALID;
    }
    else
    {
        for (i=0;i < list->numElements ;i++)
        {
            ius3DTransducerElementListSet(list,IU3DTE_INVALID,i);
        }
    }
    return list;
}



int ius3DTransducerElementListDeepDelete
(
    iu3dtel_t list
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
    list->deepDelete = IUS_TRUE;
    return ius3DTransducerElementListDelete(list);
}

int ius3DTransducerElementListDelete
(
    iu3dtel_t list
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
    if(list->deepDelete == IUS_TRUE)
    {
        for (int i = 0 ; i < list->numElements ; i++ )
        {
            ius3DTransducerElementDeepDelete(list->p3DTransducerElements[i]);
        }
    }
    free(list->p3DTransducerElements);
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
    if( reference->numElements != actual->numElements ) return IUS_FALSE;
    for(index = 0 ; index < actual->numElements ; index++ )
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
    IUS_ERR_CHECK_NULL_N_RETURN(list, -1);
    return list->numElements;
}

iu3dte_t ius3DTransducerElementListGet
(
    iu3dtel_t list,
    int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(list, IU3DTE_INVALID);
    IUS_ERR_EVAL_N_RETURN(index < 0  || index >= list->numElements, IU3DTE_INVALID);
    return list->p3DTransducerElements[index];
}

int ius3DTransducerElementListSet
(
    iu3dtel_t list,
    iu3dte_t member,
    int index
)
{
//    IUS_ERR_CHECK_NULL_N_RETURN(member, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(index < 0  || index >= list->numElements, IUS_ERR_VALUE);
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
    for (i=0;i < list->numElements;i++)
    {
        if(list->p3DTransducerElements[i] == IU3DTE_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}

iu3dtel_t ius3DTransducerElementListLoad
(
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int i,size;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DTEL_INVALID);
    hid_t elements_id = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, H5P_DEFAULT);
    IUS_ERR_EVAL_N_RETURN(elements_id == H5I_INVALID_HID, IU3DTEL_INVALID);

    int status = iusHdf5ReadInt(elements_id, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(size));
    if(status <0)
        return IU3DTEL_INVALID;

    iu3dtel_t elementList = ius3DTransducerElementListCreate(size);
    iu3dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
        hid_t element_id = H5Gopen(elements_id, path, H5P_DEFAULT);
        loadedElement = ius3DTransducerElementLoad(element_id);
        if(loadedElement == IU3DTE_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
        status = ius3DTransducerElementListSet(elementList,loadedElement,i);
        if( status == IUS_ERR_VALUE )
            break;
    }

    elementList->deepDelete = IUS_TRUE;
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
    hid_t handle
)
{
    int status=0;
    int i;
    char path[IUS_MAX_HDF5_PATH];

    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    if(ius3DTransducerElementListFull(list) == IUS_FALSE)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument list (3DTransducerElement list) was not full");
        return IUS_ERR_VALUE;
    }

    hid_t group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu3dte_t sourceElement;

    status |= iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(list->numElements), 1);

    // iterate over source list elements and save'em
    for (i=0;i < list->numElements ;i++)
    {
        sourceElement = ius3DTransducerElementListGet(list,i);
        if(sourceElement == IU3DTE_INVALID) continue;

        sprintf(path, IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
        hid_t singleElementId = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = ius3DTransducerElementSave(sourceElement, singleElementId);
        H5Gclose(singleElementId);

        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
