//
// Created by nlv09165 on 12/09/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusParameterDictPrivate.h>
#include <iusParameterDictADT.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableParameter, const char, struct HashableParameter)

// ADT
iupad_t iusParameterDictCreate
(
)
{
    iupad_t dict = calloc(1, sizeof(IusParameterDict));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusParameterDict, IUPAD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUS_FALSE;
    dict->keys = NULL;
    return dict;
}

static void iusParameterDictDeleteKeys
(
    iupad_t dict
)
{
    if (dict->keys != NULL)
        free(dict->keys);
}

int iusParameterDictDelete
(
    iupad_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
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
    iusParameterDictDeleteKeys(dict);
    free(dict);
    return IUS_E_OK;
}


static int iusParameterDictSourceInTarget
(
    iupad_t source,
    iupad_t target
)
{
    char * targetElement;
    char * sourceElement;
    HashableParameter *iterElement;

    IUS_BOOL sourceInTarget = IUS_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        sourceElement = iusParameterDictGet(source, iterElement->key);
        targetElement = iusParameterDictGet(target, iterElement->key);
        if (targetElement == NULL)
            return IUS_FALSE;

        if (strcmp(sourceElement, targetElement) != 0)
            return IUS_FALSE;
        sourceInTarget = IUS_TRUE;
    }

    return sourceInTarget;
}

// operations
int iusParameterDictCompare
(
    iupad_t reference,
    iupad_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUS_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUS_TRUE;

    // check if elements of source hash are in target hash
    if( iusParameterDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusParameterDictSourceInTarget(actual,reference);
}


size_t iusParameterDictGetSize
(
    iupad_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
    return (int) hashmap_size(&dict->map);
}

char * iusParameterDictGet
(
    iupad_t dict,
    char * key
)
{
    HashableParameter * search;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    IUS_ERR_CHECK_NULL_N_RETURN(key, NULL);

    search = HashableParameter_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return NULL;
    }
    return search->value;
}

char **iusParameterDictGetKeys
(
    iupad_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->keys;
}

static int iusParameterDictUpdateKeys
(
    iupad_t  dict
)
{
    iusParameterDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iusParameterDictGetSize(dict);
    dict->keys = calloc(size+1, sizeof(char*));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableParameter *iterElement;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        dict->keys[keyIndex] = iterElement->key;
    }
    dict->keys[keyIndex] = NULL;
    return IUS_E_OK;
}

int iusParameterDictSet
(
    iupad_t dict,
    char * key,
    char * value
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(value, IUS_ERR_VALUE);
    HashableParameter *newMember = calloc(1, sizeof(HashableParameter));
    newMember->key = strdup(key);
    newMember->value = strdup(value);

    if (HashableParameter_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        printf("discarding blob with duplicate key: %s\n", newMember->key);
        free(newMember->key);
        free(newMember->value);
        free(newMember);
        return IUS_ERR_VALUE;
    }
    return iusParameterDictUpdateKeys(dict);
}


int iusParameterDictSave
(
    iupad_t dict,
    hid_t handle
)
{
    HashableParameter *iterElement;
    struct hashmap_iter *iter;
    herr_t returnValue=0;

    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter)) {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        returnValue |= iusHdf5WriteString(handle,iterElement->key,iterElement->value);
    }
    return returnValue;
}


iupad_t iusParameterDictLoad
(
    hid_t handle
)
{
    hsize_t nobj;
    hsize_t i;
    int status = H5Gget_num_objs(handle, &nobj);
    char memberName[IUS_MAX_HDF5_PATH];
    char memberValue[IUS_MAX_HDF5_PATH];
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPAD_INVALID);

    iupad_t dict = iusParameterDictCreate();
    if (dict == IUPAD_INVALID) return IUPAD_INVALID;
    for (i = 0; i < nobj && status == IUS_E_OK; i++)
    {
        H5Gget_objname_by_idx(handle, i, memberName, (size_t)IUS_MAX_HDF5_PATH);
        iusHdf5ReadString(handle,memberName, memberValue);
        status = iusParameterDictSet(dict, memberName, memberValue);
    }
    dict->deepDelete = IUS_TRUE;
    return dict;
}
