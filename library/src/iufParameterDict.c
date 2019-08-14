#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufParameterDictPrivate.h>
#include <iufParameterDictADT.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableParameter, const char, struct HashableParameter)

// ADT
iupad_t iufParameterDictCreate
(
)
{
    iupad_t dict = calloc(1, sizeof(IufParameterDict));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufParameterDict, IUPAD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
    return dict;
}

static void iufParameterDictDeleteKeys
(
    iupad_t dict
)
{
    if (dict->kys != NULL)
    {
        free(dict->kys);
    }
}

int iufParameterDictDelete
(
    iupad_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    HashableParameter *iterElement;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        free(iterElement->key);
        free(iterElement->value);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    iufParameterDictDeleteKeys(dict);
    free(dict);
    return IUF_E_OK;
}


static int iufParameterDictSourceInTarget
(
    iupad_t source,
    iupad_t target
)
{
    char * targetElement;
    char * sourceElement;
    HashableParameter *iterElement;

    IUF_BOOL sourceInTarget = IUF_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        sourceElement = iufParameterDictGet(source, iterElement->key);
        targetElement = iufParameterDictGet(target, iterElement->key);
        if (targetElement == NULL)
            return IUF_FALSE;

        if (strcmp(sourceElement, targetElement) != 0)
            return IUF_FALSE;
        sourceInTarget = IUF_TRUE;
    }

    return sourceInTarget;
}

// operations
int iufParameterDictCompare
(
    iupad_t reference,
    iupad_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUF_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUF_TRUE;

    // check if elements of source hash are in target hash
    if( iufParameterDictSourceInTarget(reference,actual) == IUF_FALSE )
        return IUF_FALSE;

    // check if elements of target hash are in source hash
    return iufParameterDictSourceInTarget(actual,reference);
}


size_t iufParameterDictGetSize
(
    iupad_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
    return (int) hashmap_size(&dict->map);
}

char * iufParameterDictGet
(
    iupad_t dict,
    char * key
)
{
    HashableParameter * search;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    IUF_ERR_CHECK_NULL_N_RETURN(key, NULL);

    search = HashableParameter_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return NULL;
    }
    return search->value;
}

char **iufParameterDictGetKeys
(
    iupad_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufParameterDictUpdateKeys
(
    iupad_t  dict
)
{
    iufParameterDictDeleteKeys(dict);
    // allocate memory for the kys
    int keyIndex;
    size_t size = iufParameterDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableParameter *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufParameterDictSet
(
    iupad_t dict,
    char * key,
    char * value
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(value, IUF_ERR_VALUE);
    HashableParameter *newMember = calloc(1, sizeof(HashableParameter));
    newMember->key = strdup(key);
    newMember->value = strdup(value);

    if (HashableParameter_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        printf("discarding blob with duplicate key: %s\n", newMember->key);
        free(newMember->key);
        free(newMember->value);
        free(newMember);
        return IUF_ERR_VALUE;
    }
    return iufParameterDictUpdateKeys(dict);
}


int iufParameterDictSave
(
    iupad_t dict,
    hid_t handle
)
{
    HashableParameter *iterElement;
    struct hashmap_iter *iter;
    herr_t returnValue=0;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter)) {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        returnValue |= iufHdf5WriteString(handle,iterElement->key,iterElement->value);
    }
    return returnValue;
}


iupad_t iufParameterDictLoad
(
    hid_t handle
)
{
    hsize_t nobj;
    hsize_t i;
    int status = H5Gget_num_objs(handle, &nobj);
    char memberName[IUF_MAX_HDF5_PATH];
    char memberValue[IUF_MAX_HDF5_PATH];
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPAD_INVALID);

    iupad_t dict = iufParameterDictCreate();
    if (dict == IUPAD_INVALID) return IUPAD_INVALID;
    for (i = 0; i < nobj && status == IUF_E_OK; i++)
    {
        H5Gget_objname_by_idx(handle, i, memberName, (size_t)IUF_MAX_HDF5_PATH);
        iufHdf5ReadString(handle,memberName, memberValue);
        status = iufParameterDictSet(dict, memberName, memberValue);
    }
    dict->deepDelete = IUF_TRUE;
    return dict;
}
