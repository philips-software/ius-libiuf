//
// Created by nlv09165 on 31/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusSourcePrivate.h>

// ADT
struct HashableSource
{
    ius_t source;
    char key[256];
} ;

typedef struct HashableSource HashableSource;

struct IusSourceDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
    char **keys;
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableSource, const char, struct HashableSource)

// ADT
iusd_t iusSourceDictCreate
(
)
{
    iusd_t dict = calloc(1, sizeof(IusSourceDict));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusSourceDict, IUSD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUS_FALSE;
    dict->keys = NULL;
    return dict;
}


int iusSourceDictDeepDelete
(
    iusd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    dict->deepDelete = IUS_TRUE;
    return iusSourceDictDelete(dict);
}

static void iusSourceDictDeleteKeys
(
    iusd_t dict
)
{
    if (dict->keys != NULL)
        free(dict->keys);
}

int iusSourceDictDelete
(
  iusd_t dict
)
{
    HashableSource *iterElement;
    struct hashmap_iter *iter;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter)) {
        iterElement = HashableSource_hashmap_iter_get_data(iter);
        if (dict->deepDelete==IUS_TRUE)
            iusSourceDelete(iterElement->source);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    iusSourceDictDeleteKeys(dict);
    free(dict);
    return IUS_E_OK;
}


static int iusSourceDictSourceInTarget
(
    iusd_t source,
    iusd_t target
)
{
    ius_t targetElement;
    ius_t sourceElement;
    HashableSource *iterElement;

    IUS_BOOL sourceInTarget = IUS_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashableSource_hashmap_iter_get_data(iter);
      sourceElement=iterElement->source;
      targetElement = iusSourceDictGet(target, iterElement->key);
      if( targetElement == IUS_INVALID)
        return IUS_FALSE;

      if( iusSourceCompare(sourceElement, targetElement) == IUS_FALSE )
        return IUS_FALSE;
      sourceInTarget = IUS_TRUE;
    }

  return sourceInTarget;
}

// operations
int iusSourceDictCompare
(
  iusd_t reference,
  iusd_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUS_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUS_TRUE;

    // check if elements of source hash are in target hash
    if( iusSourceDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusSourceDictSourceInTarget(actual,reference);
}


int iusSourceDictGetSize
(
    iusd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, -1);
    return (int) hashmap_size(&dict->map);
}

ius_t iusSourceDictGet
(
    iusd_t dict,
    char * key
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_INVALID);
    HashableSource * search;
    search = HashableSource_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IUS_INVALID;
    }
    return search->source;
}

char **iusSourceDictGetKeys
(
    iusd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->keys;
}

static int iusSourceDictUpdateKeys
(
    iusd_t dict
)
{
    iusSourceDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iusSourceDictGetSize(dict);
    dict->keys = calloc(size+1, sizeof(char*));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableSource *iterElement;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableSource_hashmap_iter_get_data(iter);
        dict->keys[keyIndex] = iterElement->key;
    }
    dict->keys[keyIndex] = NULL;
    return IUS_E_OK;
}

int iusSourceDictSet
(
    iusd_t dict,
    char * key,
    ius_t member
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);
    HashableSource *newMember = calloc(1, sizeof(HashableSource));
    newMember->source = member;
    strcpy(newMember->key,key);
    if (HashableSource_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
        free(newMember);
        return IUS_ERR_VALUE;
    }
    return iusSourceDictUpdateKeys(dict);
}


// serialization
int iusSourceDictSave
(
    iusd_t dict,
    hid_t handle
)
{

    int status=0;
    struct hashmap_iter *iter;
	hid_t sources_id;

    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_SOURCEDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		sources_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_SOURCEDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		sources_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SOURCEDICT, H5P_DEFAULT);
	}
    if (sources_id == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_SOURCEDICT);
        return IUS_ERR_VALUE;
    }

    HashableSource *sourceElement;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        sourceElement = HashableSource_hashmap_iter_get_data(iter);
		const char *sourceLabel = HashableSource_hashmap_iter_get_key(iter);
		hid_t src_id = H5Gcreate(sources_id, sourceLabel, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iusSourceSave(sourceElement->source, src_id);
		status |= H5Gclose(src_id);
    }
    status |= H5Gclose(sources_id);
    return status;
}


#define MAX_NAME 1024

iusd_t iusSourceDictLoad
(
    hid_t handle
)
{
    int i;
    int status;
    char memb_name[MAX_NAME];
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUSD_INVALID);
    hid_t grpid = H5Gopen(handle, IUS_INPUTFILE_PATH_SOURCEDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_SOURCEDICT);
        return NULL;
    }

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iusd_t dict = iusSourceDictCreate();
    for (i = 0; i < (int) nobj && status == IUS_E_OK; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i, memb_name, (size_t) MAX_NAME);
		hid_t source_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
        ius_t source = iusSourceLoad(source_id);
		status |= H5Gclose(source_id);
        status |= iusSourceDictSet(dict, memb_name, source);
    }
    status |= H5Gclose(grpid);
    if( status != IUS_E_OK )
    {
        return NULL;
    }
    dict->deepDelete = IUS_TRUE;
    return dict;
}
