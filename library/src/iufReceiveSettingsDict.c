#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufReceiveSettingsPrivate.h>
#include <iufReceiveSettingsDictADT.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveSettings, const char, struct HashableReceiveSettings)

iursd_t iufReceiveSettingsDictCreate
(
)
{
    iursd_t dict = calloc(1, sizeof(IufReceiveSettingsDict));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufReceiveSettingsDict, IURSD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
    return dict;
}

int iufReceiveSettingsDictDeepDelete
(
    iursd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    dict->deepDelete = IUF_TRUE;
    return iufReceiveSettingsDictDelete(dict);
}

static void iufReceiveSettingsDictDeleteKeys
(
        iursd_t dict
)
{
    if (dict->kys != NULL)
    {
        free(dict->kys);
    }
}

int iufReceiveSettingsDictDelete
(
    iursd_t dict
)
{
    HashableReceiveSettings *iterElement;
    struct hashmap_iter *iter;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashableReceiveSettings_hashmap_iter_get_data(iter);
        if (dict->deepDelete == IUF_TRUE)
            iufReceiveSettingsDelete(iterElement->receiveSettings);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    iufReceiveSettingsDictDeleteKeys(dict);
    free(dict);
    return IUF_E_OK;
}


static int iufReceiveSettingsDictSourceInTarget
(
    iursd_t source,
    iursd_t target
)
{
    iurs_t targetElement;
    iurs_t sourceElement;
    HashableReceiveSettings *iterElement;

    IUF_BOOL sourceInTarget = IUF_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashableReceiveSettings_hashmap_iter_get_data(iter);
      sourceElement=iterElement->receiveSettings;
      targetElement = iufReceiveSettingsDictGet(target, iterElement->key);
      if( targetElement == IURS_INVALID)
        return IUF_FALSE;

      if( iufReceiveSettingsCompare(sourceElement, targetElement) == IUF_FALSE )
        return IUF_FALSE;
      sourceInTarget = IUF_TRUE;
    }

  return sourceInTarget;
}

// operations
int iufReceiveSettingsDictCompare
(
  iursd_t reference,
  iursd_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUF_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUF_TRUE;

    // check if elements of source hash are in target hash
    if( iufReceiveSettingsDictSourceInTarget(reference,actual) == IUF_FALSE )
        return IUF_FALSE;

    // check if elements of target hash are in source hash
    return iufReceiveSettingsDictSourceInTarget(actual,reference);
}


size_t iufReceiveSettingsDictGetSize
(
    iursd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
    return (int) hashmap_size(&dict->map);
}

iurs_t iufReceiveSettingsDictGet
(
    iursd_t dict,
    char * key
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IURS_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IURS_INVALID);
    HashableReceiveSettings * search;
    search = HashableReceiveSettings_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IURS_INVALID;
    }
    return search->receiveSettings;
}

char **iufReceiveSettingsDictGetKeys
(
    iursd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufReceiveSettingsUpdateKeys
(
        iursd_t dict
)
{
    iufReceiveSettingsDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufReceiveSettingsDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableReceiveSettings *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableReceiveSettings_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufReceiveSettingsDictSet
(
    iursd_t dict,
    char * key,
    iurs_t member
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    HashableReceiveSettings *newMember = calloc(1, sizeof(HashableReceiveSettings));
    newMember->receiveSettings = member;
    strncpy(newMember->key,key, sizeof(newMember->key));
    if (HashableReceiveSettings_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
        free(newMember);
        return IUF_ERR_VALUE;
    }
    return iufReceiveSettingsUpdateKeys(dict);
}


int iufReceiveSettingsDictRemove
        (
                iursd_t dict,
                char * key
        )
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    if (HashableReceiveSettings_hashmap_remove(&dict->map, key)==NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "key: %s does not exist in dictionary", key);
        return IUF_ERR_VALUE;
    }
    return iufReceiveSettingsUpdateKeys(dict);
}


// serialization
int iufReceiveSettingsDictSave
(
    iursd_t dict,
    hid_t handle
)
{
    int status=0;
    struct hashmap_iter *iter;
	hid_t group_id;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGSDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGSDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGSDICT, H5P_DEFAULT);
	}

	if (group_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
        return IUF_ERR_VALUE;
    }

    status = 0;
    HashableReceiveSettings *receiveSettingsDictItem;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
    {
		receiveSettingsDictItem = HashableReceiveSettings_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, receiveSettingsDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iufReceiveSettingsSave(receiveSettingsDictItem->receiveSettings, subgroup_id);
        if (subgroup_id <= 0)
        {
            H5Gclose(group_id);
            return IUF_ERR_VALUE;
        }
		status |= H5Gclose(subgroup_id);
    }

    status |= H5Gclose(group_id );
    return status;
}


iursd_t iufReceiveSettingsDictLoad
(
    hid_t handle
)
{
    int i;
    int status = 0;
    char member_name[IUF_MAX_HDF5_PATH];

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IURSD_INVALID);
    hid_t grpid = H5Gopen(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGSDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_RECEIVESETTINGSDICT);
        return IURSD_INVALID;
    }

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iursd_t dict = iufReceiveSettingsDictCreate();
    for (i = 0; i < (int) nobj; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i, member_name, (size_t) IUF_MAX_HDF5_PATH);
     
		hid_t settings_id = H5Gopen(grpid, member_name, H5P_DEFAULT);
        iurs_t receiveSettings = iufReceiveSettingsLoad(settings_id);
        status = iufReceiveSettingsDictSet(dict, member_name, receiveSettings);
		H5Gclose(settings_id);
    }
    H5Gclose(grpid);
    
	if (status != IUF_E_OK)
    {
        return IURSD_INVALID;
    }
    dict->deepDelete = IUF_TRUE;
    return dict;
}
