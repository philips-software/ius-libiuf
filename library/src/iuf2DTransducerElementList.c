#include <stdlib.h>

#include <iuf.h>
#include <iuf2DTransducerElementListPrivate.h>
#include <iuf2DTransducerElementPrivate.h>


// ADT
iu2dtel_t iuf2DTransducerElementListCreate
(
    int numElements
)
{
    int i;
    IUF_ERR_EVAL_N_RETURN(numElements<=0, IU2DTEL_INVALID);
    iu2dtel_t list = calloc(1, sizeof(Iuf2DTransducerElementList));
    IUF_ERR_ALLOC_NULL_N_RETURN(list, Iuf2DTransducerElementList, IU2DTEL_INVALID);
    list->deepDelete = IUF_FALSE;
    list->numElements = numElements;
    list->p2DTransducerElements = (iu2dte_t *) calloc((size_t)numElements, sizeof(iu2dte_t));
    if( list->p2DTransducerElements == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for p2DTransducerElements member");
        free(list);
        list = NULL;
    }
    else
    {
        for(i = 0 ; i < list->numElements ; i++ )
        {
            iuf2DTransducerElementListSet(list,IU2DTE_INVALID,i);
        }
    }
    return list;
}


int iuf2DTransducerElementListDeepDelete
(
    iu2dtel_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    list->deepDelete = IUF_TRUE;
    return iuf2DTransducerElementListDelete(list);
}

int iuf2DTransducerElementListDelete
(
    iu2dtel_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    if(list->deepDelete == IUF_TRUE)
    {
        for (int i = 0 ; i < list->numElements ; i++ )
        {
            iuf2DTransducerElementDelete(list->p2DTransducerElements[i]);
        }
    }
    free(list->p2DTransducerElements);
    free(list);
    return IUF_E_OK;
}

// operations
int iuf2DTransducerElementListCompare
(
    iu2dtel_t reference,
    iu2dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numElements != actual->numElements ) return IUF_FALSE;
    for(index = 0 ; index < actual->numElements ; index++ )
    {
        if( iuf2DTransducerElementCompare( reference->p2DTransducerElements[index], actual->p2DTransducerElements[index] )
            == IUF_FALSE )
            return IUF_FALSE;
    }
    return IUF_TRUE;
}


int iuf2DTransducerElementListGetSize
(
    iu2dtel_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, -1);
    return list->numElements;
}

iu2dte_t iuf2DTransducerElementListGet
(
    iu2dtel_t list,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IU2DTE_INVALID);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numElements, IU2DTE_INVALID);
    return list->p2DTransducerElements[index];
}

int iuf2DTransducerElementListSet
(
    iu2dtel_t list,
    iu2dte_t member,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numElements, IUF_ERR_VALUE);
    list->p2DTransducerElements[index] = member;
    return IUF_E_OK;
}

IUF_BOOL iuf2DTransducerElementListFull
(
    iu2dtel_t list
)
{
    IUF_BOOL isFull = IUF_TRUE;
    int i;
    for (i=0;i < list->numElements;i++)
    {
        if(list->p2DTransducerElements[i] == IU2DTE_INVALID)
        {
            isFull = IUF_FALSE;
            break;
        }
    }
    return isFull;
}

iu2dtel_t iuf2DTransducerElementListLoad
(
    hid_t handle
)
{
    char path[IUF_MAX_HDF5_PATH];
    int i,size;

    if(handle == H5I_INVALID_HID) return IU2DTEL_INVALID;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DTEL_INVALID);
    hid_t elements_id = H5Gopen(handle, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST, H5P_DEFAULT);
    IUF_ERR_EVAL_N_RETURN(elements_id == H5I_INVALID_HID, IU2DTEL_INVALID);

    int status = iufHdf5ReadInt(elements_id, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(size));
    if(status <0)
        return IU2DTEL_INVALID;

    iu2dtel_t elementList = iuf2DTransducerElementListCreate(size);
    iu2dte_t loadedElement;
    for (i=0;i < size;i++)
    {
        sprintf(path, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
        hid_t element_single_id = H5Gopen(elements_id, path, H5P_DEFAULT);
        loadedElement = iuf2DTransducerElementLoad(element_single_id);
        if(loadedElement == IU2DTE_INVALID)
        {
            status = IUF_ERR_VALUE;
            break;
        }
        H5Gclose(element_single_id);
        status = iuf2DTransducerElementListSet(elementList,loadedElement, i);
        if( status == IUF_ERR_VALUE )
            break;
    }

    H5Gclose(elements_id);
    elementList->deepDelete = IUF_TRUE;
    if( status == IUF_ERR_VALUE )
    {
        iuf2DTransducerElementListDelete(elementList);
        elementList = IU2DTEL_INVALID;
    }
    return elementList;
}


int iuf2DTransducerElementListSave
(
    iu2dtel_t list,
    hid_t handle
)
{
    int status=0;
    int i;
    char path[IUF_MAX_HDF5_PATH];

    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    if(iuf2DTransducerElementListFull(list) == IUF_FALSE)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "argument list (2DTransducerElement list) was not full");
        return IUF_ERR_VALUE;
    }

    iu2dte_t sourceElement;
    hid_t elements_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iufHdf5WriteInt(elements_id, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE, &(list->numElements), 1);

    // iterate over source list elements and save'em
    for (i=0; i<list->numElements; i++)
    {
        sourceElement = iuf2DTransducerElementListGet(list,i);
        if(sourceElement == IU2DTE_INVALID) continue;

        sprintf(path, IUF_INPUTFILE_PATH_TRANSDUCER_ELEMENT, i);
        hid_t element_single_id = H5Gcreate(elements_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iuf2DTransducerElementSave(sourceElement, element_single_id);
        H5Gclose(element_single_id);
        if(status != IUF_E_OK) break;
    }
    H5Gclose(elements_id);
    return status;
}
