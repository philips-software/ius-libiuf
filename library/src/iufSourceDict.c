#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufSourceDictADT.h>
#include <iufSourcePrivate.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableSource, const char, struct HashableSource)

// ADT
iusd_t iufSourceDictCreate
(
)
{
    iusd_t dict = calloc(1, sizeof(IufSourceDict));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufSourceDict, IUFD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
    return dict;
}


int iufSourceDictDeepDelete
(
    iusd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    dict->deepDelete = IUF_TRUE;
    return iufSourceDictDelete(dict);
}

static void iufSourceDictDeleteKeys
(
    iusd_t dict
)
{
    if (dict->kys != NULL)
        free(dict->kys);
}

int iufSourceDictDelete
(
  iusd_t dict
)
{
    HashableSource *iterElement;
    struct hashmap_iter *iter;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter)) {
        iterElement = HashableSource_hashmap_iter_get_data(iter);
        if (dict->deepDelete==IUF_TRUE)
            iufSourceDelete(iterElement->source);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    iufSourceDictDeleteKeys(dict);
    free(dict);
    return IUF_E_OK;
}


static int iufSourceDictSourceInTarget
(
    iusd_t source,
    iusd_t target
)
{
    ius_t targetElement;
    ius_t sourceElement;
    HashableSource *iterElement;

    IUF_BOOL sourceInTarget = IUF_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashableSource_hashmap_iter_get_data(iter);
      sourceElement=iterElement->source;
      targetElement = iufSourceDictGet(target, iterElement->key);
      if( targetElement == IUS_INVALID)
        return IUF_FALSE;

      if( iufSourceCompare(sourceElement, targetElement) == IUF_FALSE )
        return IUF_FALSE;
      sourceInTarget = IUF_TRUE;
    }

  return sourceInTarget;
}

// operations
int iufSourceDictCompare
(
  iusd_t reference,
  iusd_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUF_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUF_TRUE;

    // check if elements of source hash are in target hash
    if( iufSourceDictSourceInTarget(reference,actual) == IUF_FALSE )
        return IUF_FALSE;

    // check if elements of target hash are in source hash
    return iufSourceDictSourceInTarget(actual,reference);
}


int iufSourceDictGetSize
(
    iusd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, -1);
    return (int) hashmap_size(&dict->map);
}

ius_t iufSourceDictGet
(
    iusd_t dict,
    char * key
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUS_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUS_INVALID);
    HashableSource * search;
    search = HashableSource_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IUS_INVALID;
    }
    return search->source;
}

char **iufSourceDictGetKeys
(
    iusd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufSourceDictUpdateKeys
(
    iusd_t dict
)
{
    iufSourceDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufSourceDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableSource *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableSource_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufSourceDictSet
(
    iusd_t dict,
    char * key,
    ius_t member
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    HashableSource *newMember = calloc(1, sizeof(HashableSource));
    newMember->source = member;
    strncpy(newMember->key,key, sizeof(newMember->key));
    if (HashableSource_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
        free(newMember);
        return IUF_ERR_VALUE;
    }
    return iufSourceDictUpdateKeys(dict);
}


// serialization
int iufSourceDictSave
(
    iusd_t dict,
    hid_t handle
)
{

    int status=0;
    struct hashmap_iter *iter;
	hid_t sources_id;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_SOURCEDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		sources_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_SOURCEDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		sources_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SOURCEDICT, H5P_DEFAULT);
	}
    if (sources_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_SOURCEDICT);
        return IUF_ERR_VALUE;
    }

    HashableSource *sourceElement;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        sourceElement = HashableSource_hashmap_iter_get_data(iter);
		const char *sourceLabel = HashableSource_hashmap_iter_get_key(iter);
		hid_t src_id = H5Gcreate(sources_id, sourceLabel, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iufSourceSave(sourceElement->source, src_id);
		status |= H5Gclose(src_id);
    }
    status |= H5Gclose(sources_id);
    return status;
}


#define MAX_NAME 1024

iusd_t iufSourceDictLoad
(
    hid_t handle
)
{
    int i;
    int status;
    char memb_name[MAX_NAME];
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUFD_INVALID);
    hid_t grpid = H5Gopen(handle, IUF_INPUTFILE_PATH_SOURCEDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_SOURCEDICT);
        return NULL;
    }

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iusd_t dict = iufSourceDictCreate();
    for (i = 0; i < (int) nobj && status == IUF_E_OK; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i, memb_name, (size_t) MAX_NAME);
		hid_t source_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
        ius_t source = iufSourceLoad(source_id);
		status |= H5Gclose(source_id);
        status |= iufSourceDictSet(dict, memb_name, source);
    }
    status |= H5Gclose(grpid);
    if( status != IUF_E_OK )
    {
        return NULL;
    }
    dict->deepDelete = IUF_TRUE;
    return dict;
}
