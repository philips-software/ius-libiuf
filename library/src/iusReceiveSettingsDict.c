//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusReceiveSettingsPrivate.h>

// ADT
struct HashableReceiveSettings
{
    iurs_t receiveSettings;
    char key[256];
} ;

typedef struct HashableReceiveSettings HashableReceiveSettings;

struct IusReceiveSettingsDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveSettings, const char, struct HashableReceiveSettings)

// ADT
iursd_t iusReceiveSettingsDictCreate
(
)
{
    iursd_t dict = calloc(1, sizeof(IusReceiveSettingsDict));
    if(dict!=NULL)
    {
      hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
      dict->deepDelete = IUS_FALSE;
    }
    return dict;
}

int iusReceiveSettingsDictDeepDelete
(
    iursd_t dict
)
{
    if (dict == NULL) return IUS_ERR_VALUE;
    dict->deepDelete = IUS_TRUE;
    return iusReceiveSettingsDictDelete(dict);
}

int iusReceiveSettingsDictDelete
(
    iursd_t dict
)
{
    HashableReceiveSettings *iterElement;
    struct hashmap_iter *iter;

    if (dict == NULL) return IUS_ERR_VALUE;
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashableReceiveSettings_hashmap_iter_get_data(iter);
        if (dict->deepDelete == IUS_TRUE)
            iusReceiveSettingsDelete(iterElement->receiveSettings);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    free(dict);
    return IUS_E_OK;
}


static int iusReceiveSettingsDictSourceInTarget
(
    iursd_t source,
    iursd_t target
)
{
    iurs_t targetElement;
    iurs_t sourceElement;
    HashableReceiveSettings *iterElement;

    IUS_BOOL sourceInTarget = IUS_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashableReceiveSettings_hashmap_iter_get_data(iter);
      sourceElement=iterElement->receiveSettings;
      targetElement = iusReceiveSettingsDictGet(target, iterElement->key);
      if( targetElement == IURS_INVALID)
        return IUS_FALSE;

      if( iusReceiveSettingsCompare(sourceElement, targetElement) == IUS_FALSE )
        return IUS_FALSE;
      sourceInTarget = IUS_TRUE;
    }

  return sourceInTarget;
}

// operations
int iusReceiveSettingsDictCompare
(
  iursd_t reference,
  iursd_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUS_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUS_TRUE;

    // check if elements of source hash are in target hash
    if( iusReceiveSettingsDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusReceiveSettingsDictSourceInTarget(actual,reference);
}


int iusReceiveSettingsDictGetSize
(
    iursd_t dict
)
{
    if (dict == NULL)
        return -1;
    return (int) hashmap_size(&dict->map);
}

iurs_t iusReceiveSettingsDictGet
(
    iursd_t dict,
    char * key
)
{
    if (dict == NULL || key == NULL) return IURS_INVALID;
    HashableReceiveSettings * search;
    search = HashableReceiveSettings_hashmap_get(&dict->map, key);
    if( search == NULL )
        return IURS_INVALID;
    return search->receiveSettings;
}

int iusReceiveSettingsDictSet
(
    iursd_t dict,
    char * key,
    iurs_t member
)
{
    if (dict == NULL || key == NULL) return IUS_ERR_VALUE;
    HashableReceiveSettings *newMember = calloc(1, sizeof(HashableReceiveSettings));
    newMember->receiveSettings = member;
    strcpy(newMember->key,key);
    if (HashableReceiveSettings_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
      printf("discarding blob with duplicate key: %s\n", newMember->key);
      free(newMember);
      return IUS_ERR_VALUE;
    }
    return IUS_E_OK;
}


// serialization
int iusReceiveSettingsDictSave
(
    iursd_t dict,
    hid_t handle
)
{
    int status=0;
    struct hashmap_iter *iter;
	hid_t group_id;

    if(dict == NULL)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;

	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGSDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGSDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGSDICT, H5P_DEFAULT);
	}
	if (group_id == H5I_INVALID_HID)
		return IUS_ERR_VALUE;
	status = 0;
    HashableReceiveSettings *receiveSettingsDictItem;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
    {
		receiveSettingsDictItem = HashableReceiveSettings_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, receiveSettingsDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iusReceiveSettingsSave(receiveSettingsDictItem->receiveSettings, subgroup_id);
		status |= H5Gclose(subgroup_id);
    }

    status |= H5Gclose(group_id );
    return status;
}


iursd_t iusReceiveSettingsDictLoad
(
    hid_t handle
)
{
    int i;
    int status = 0;
    char member_name[IUS_MAX_HDF5_PATH];

    hid_t grpid = H5Gopen(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGSDICT, H5P_DEFAULT);
    if (handle == H5I_INVALID_HID)
        return NULL;

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iursd_t dict = iusReceiveSettingsDictCreate();
    for (i = 0; i < (int) nobj; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i, member_name, (size_t) IUS_MAX_HDF5_PATH);
     
		hid_t settings_id = H5Gopen(grpid, member_name, H5P_DEFAULT);
        iurs_t receiveSettings = iusReceiveSettingsLoad(settings_id);
        status = iusReceiveSettingsDictSet(dict, member_name, receiveSettings);
		H5Gclose(settings_id);
    }
    H5Gclose(grpid);
    
	if (status != IUS_E_OK)
    {
        return NULL;
    }
    dict->deepDelete = IUS_TRUE;
    return dict;
}
