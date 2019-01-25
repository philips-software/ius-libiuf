#include "mxHandleList.h"


#define MXH_IDBASE (1.0000)
#define MXH_IDSTEP (0.0001)

static double mxHandleId = MXH_IDBASE;

struct MxHandle {
    double handleId;
    handle_t handle;
    mxh_t next;
};


double mxHandleListAppend
(
    mxh_t * list,
    handle_t handle
)
{
    mxh_t h;
    mxh_t current;
    if (handle == MXHANDLE_INVALID)
    {
        mexErrMsgIdAndTxt("mxHandleList:mxHandleListAppend", "Invalid handle supplied.");
    }
    h = calloc(1, sizeof(MxHandle));
    if (h == MXH_INVALID)
    {
        mexErrMsgIdAndTxt("mxHandleList:mxHandleListAppend", "Unable to allocate memory for handle struct.");
    }
    
    /* Fill the struct */    
    h->handleId = mxHandleId;
    h->handle = handle;
    h->next = MXH_INVALID;

    /* Update global handle id */
    mxHandleId += MXH_IDSTEP;

    /* Append newly created handle to the list*/
    if (*list == MXH_INVALID)
    {
        *list = h;
    }
    else
    {
        current = *list;
        while (current->next != MXH_INVALID)
        {
            current = current->next;
        }
        current->next = h;
    }
    return h->handleId;
}

handle_t mxHandleListGetById
(
    mxh_t * list,
    double handleId
)
{
    mxh_t current = *list;
    while (current != MXH_INVALID)
    {
        if (current->handleId == handleId)
        {
            return current->handle;
        }
        current = current->next;
    }
    mexErrMsgIdAndTxt("mxHandleList:mxHandleListGetById", "Unable to find requested handle.");
    return MXHANDLE_INVALID;
}

handle_t mxHandleListRemoveById
(
    mxh_t * list,
    double handleId
)
{
    mxh_t current = *list;
    mxh_t found = MXH_INVALID;
    handle_t handle = MXHANDLE_INVALID;

    if (*list == MXH_INVALID)
    {
        mexErrMsgIdAndTxt("mxHandleList:mxHandleListRemoveById", "Handle not found, list was empty.");
    }

    if (current->handleId == handleId) /* Handle is the first one */
    {
        /* Register found handle */
        found = current;
        handle = found->handle;
        /* Update the list */
        *list = current->next;
    }
    else /* Find handle further down the list */
    {
        while (current->next != MXH_INVALID)
        {
            if (current->next->handleId == handleId) /* Handle found */
            {
                /* Register found handle */
                found = current->next;
                handle = found->handle;
                /* Update the list */
                current->next = found->next;
            }
            /* Step to next handle */
            current = current->next;
        }
    }

    if( found == MXH_INVALID )
    {
        mexErrMsgIdAndTxt("mxHandleList:mxHandleListRemoveById", "Handle not found.");
    }

    /* Free memory of the handle list item */
    free(found);
    /* Return the found handle or MXH_INVALID */
    return handle;
}


handle_t mxHandleListRemoveFirst
(
    mxh_t * list
)
{
    if (*list == MXH_INVALID)
    {
        mexErrMsgIdAndTxt("mxHandleList:mxHandleListRemoveFirst", "List was empty.");
    }
    return mxHandleListRemoveById( list, (*list)->handleId );
}

