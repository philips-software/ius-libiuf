#include <stdlib.h>

#include <iuf.h>
#include <iuf3DTransducerElementPrivate.h>
#include <iuf3DTransducerElementListPrivate.h>

struct Iuf3DTransducerElementList
{
    int numElements;
    iu3dte_t *   p3DTransducerElements ;
    IUF_BOOL deepDelete;
} ;

// ADT
iu3dtel_t iuf3DTransducerElementListCreate
(
    int numElements
)
{
    int i;
    IUF_ERR_EVAL_N_RETURN(numElements<=0, IU3DTEL_INVALID);
    iu3dtel_t list = calloc(1, sizeof(Iuf3DTransducerElementList));
    IUF_ERR_ALLOC_NULL_N_RETURN(list, Iuf3DTransducerElementList, IU3DTEL_INVALID);
    list->deepDelete = IUF_FALSE;
    list->numElements = numElements;
    list->p3DTransducerElements = (iu3dte_t *) calloc((size_t)numElements, sizeof(iu3dte_t));
    if( list->p3DTransducerElements == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for p3DTransducerElements member");
        free(list);
        list = IU3DTEL_INVALID;
    }
    else
    {
        for (i=0;i < list->numElements ;i++)
        {
            iuf3DTransducerElementListSet(list,IU3DTE_INVALID,i);
        }
    }
    return list;
}



int iuf3DTransducerElementListDeepDelete
(
    iu3dtel_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    list->deepDelete = IUF_TRUE;
    return iuf3DTransducerElementListDelete(list);
}

int iuf3DTransducerElementListDelete
(
    iu3dtel_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    if(list->deepDelete == IUF_TRUE)
    {
        for (int i = 0 ; i < list->numElements ; i++ )
        {
            iuf3DTransducerElementDeepDelete(list->p3DTransducerElements[i]);
        }
    }
    free(list->p3DTransducerElements);
    free(list);
    return IUF_E_OK;
}


// operations
int iuf3DTransducerElementListCompare
(
    iu3dtel_t reference,
    iu3dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numElements != actual->numElements ) return IUF_FALSE;
    for(index = 0 ; index < actual->numElements ; index++ )
    {
        if( iuf3DTransducerElementCompare( reference->p3DTransducerElements[index], actual->p3DTransducerElements[index] )
            == IUF_FALSE )
            return IUF_FALSE;
    }
    return IUF_TRUE;
}


int iuf3DTransducerElementListGetSize
(
    iu3dtel_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, -1);
    return list->numElements;
}

iu3dte_t iuf3DTransducerElementListGet
(
    iu3dtel_t list,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IU3DTE_INVALID);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numElements, IU3DTE_INVALID);
    return list->p3DTransducerElements[index];
}

int iuf3DTransducerElementListSet
(
    iu3dtel_t list,
    iu3dte_t member,
    int index
)
{
//    IUF_ERR_CHECK_NULL_N_RETURN(member, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numElements, IUF_ERR_VALUE);
    list->p3DTransducerElements[index] = member;
    return IUF_E_OK;
}

IUF_BOOL iuf3DTransducerElementListFull
(
    iu3dtel_t list
)
{
    IUF_BOOL isFull = IUF_TRUE;
    int i;
    for (i=0;i < list->numElements;i++)
    {
        if(list->p3DTransducerElements[i] == IU3DTE_INVALID)
        {
            isFull = IUF_FALSE;
            break;
        }
    }
    return isFull;
}

iu3dtel_t iuf3DTransducerElementListLoad
(
    hid_t handle
)
{
    char path[IUF_MAX_HDF5_PATH];
    int i,size;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DTEL_INVALID);
    hid_t elements_id = H5Gopen(handle, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, H5P_DEFAULT);
    IUF_ERR_EVAL_N_RETURN(elements_id == H5I_INVALID_HID, IU3DTEL_INVALID);

    int status = iufHdf5ReadInt(elements_id, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(size));
    if(status <0)
        return IU3DTEL_INVALID;

    iu3dtel_t elementList = iuf3DTransducerElementListCreate(size);
    iu3dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
        hid_t element_id = H5Gopen(elements_id, path, H5P_DEFAULT);
        loadedElement = iuf3DTransducerElementLoad(element_id);
        if(loadedElement == IU3DTE_INVALID)
        {
            status = IUF_ERR_VALUE;
            break;
        }
        status = iuf3DTransducerElementListSet(elementList,loadedElement,i);
        if( status == IUF_ERR_VALUE )
            break;
    }

    elementList->deepDelete = IUF_TRUE;
    if( status == IUF_ERR_VALUE )
    {
        iuf3DTransducerElementListDelete(elementList);
        elementList = IU3DTEL_INVALID;
    }
    return elementList;
}


int iuf3DTransducerElementListSave
(
    iu3dtel_t list,
    hid_t handle
)
{
    int status=0;
    int i;
    char path[IUF_MAX_HDF5_PATH];

    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    if(iuf3DTransducerElementListFull(list) == IUF_FALSE)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "argument list (3DTransducerElement list) was not full");
        return IUF_ERR_VALUE;
    }

    hid_t group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu3dte_t sourceElement;

    status |= iufHdf5WriteInt(group_id, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(list->numElements), 1);

    // iterate over source list elements and save'em
    for (i=0;i < list->numElements ;i++)
    {
        sourceElement = iuf3DTransducerElementListGet(list,i);
        if(sourceElement == IU3DTE_INVALID) continue;

        sprintf(path, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
        hid_t singleElementId = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iuf3DTransducerElementSave(sourceElement, singleElementId);
        H5Gclose(singleElementId);

        if(status != IUF_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
