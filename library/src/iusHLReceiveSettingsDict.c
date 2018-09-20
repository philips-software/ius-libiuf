//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include <hashmap.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLReceiveSettingsImp.h>
#include <iusHLReceiveSettingsDict.h>

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
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveSettings, const char, struct HashableReceiveSettings)

// ADT
iursd_t iusHLReceiveSettingsDictCreate
(
)
{
    iursd_t list = calloc(1, sizeof(IusReceiveSettingsDict));
    if(list!=NULL)
    {
      hashmap_init(&list->map, hashmap_hash_string, hashmap_compare_string, 0);
    }
    return list;
}

int iusHLReceiveSettingsDictDelete
(
  iursd_t dict
)
{
    if(dict == NULL) return IUS_ERR_VALUE;
    /* Free all allocated resources associated with map and reset its state */
    hashmap_destroy(&dict->map);
    free(dict);
    return IUS_E_OK;
}


static int iusHLReceiveSettingsDictSourceInTarget
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
      targetElement = iusHLReceiveSettingsDictGet(target, iusHLReceiveSettingsGetLabel(sourceElement));
      if( targetElement == IURS_INVALID)
        return IUS_FALSE;

      if( iusHLReceiveSettingsCompare(sourceElement, targetElement) == IUS_FALSE )
        return IUS_FALSE;
      sourceInTarget = IUS_TRUE;
    }

  return sourceInTarget;
}

// operations
int iusHLReceiveSettingsDictCompare
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
    if( iusHLReceiveSettingsDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusHLReceiveSettingsDictSourceInTarget(actual,reference);
}


int iusHLReceiveSettingsDictGetSize
(
    iursd_t dict
)
{
    if( dict == NULL )
        return -1;
    return (int) hashmap_size(&dict->map);
}

iurs_t iusHLReceiveSettingsDictGet
(
    iursd_t dict,
    char * key
)
{
    HashableReceiveSettings * search;
    search = HashableReceiveSettings_hashmap_get(&dict->map, key);
    if( dict == NULL )
        return IURS_INVALID;
    return search->receiveSettings;
}

int iusHLReceiveSettingsDictSet
(
    iursd_t dict,
    char * key,
    iurs_t member
)
{
    if( dict == NULL ) return IUS_ERR_VALUE;
    if( key == NULL ) return IUS_ERR_VALUE;

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
#define NUMPULSEVALUESFMT  "%s/numReceiveSettingsValues"
#define PULSEAMPLITUDESFMT "%s/rawReceiveSettingsAmplitudes"
#define PULSETIMESFMT      "%s/rawReceiveSettingsTimes"


//#define LABELPATH          "%s"
int iusHLReceiveSettingsDictSave
(
    iursd_t dict,
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    struct hashmap_iter *iter;

    if(dict == NULL)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;

    //hid_t group_id = H5Gcreate(handle, "ReceiveSettings", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    HashableReceiveSettings *receiveSettingsDictItem;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
		receiveSettingsDictItem = HashableReceiveSettings_hashmap_iter_get_data(iter);
		//sprintf(path, LABELPATH, parentPath, label);
        iusHLReceiveSettingsSave(receiveSettingsDictItem->receiveSettings, handle);
    }

    //status |= H5Gclose(group_id );
    return status;
}


#define MAX_NAME 1024

iursd_t iusHLReceiveSettingsDictLoad
(
    hid_t handle
)
{
    int i;
    int status = 0;
    char path[IUS_MAX_HDF5_PATH];
    char memb_name[MAX_NAME];

    hid_t grpid = H5Gopen(handle, "ReceiveSettings", H5P_DEFAULT);
    if (handle == H5I_INVALID_HID)
        return NULL;

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iursd_t dict = iusHLReceiveSettingsDictCreate();
    for (i = 0; i < nobj; i++)
    {
        H5Gget_objname_by_idx(handle, (hsize_t) i, memb_name, (size_t) MAX_NAME);
        sprintf(path,"/%s", memb_name);
		hid_t settings_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
        iurs_t receiveSettings = iusHLReceiveSettingsLoad(settings_id, memb_name);
        status = iusHLReceiveSettingsDictSet(dict, memb_name, receiveSettings);
		H5Gclose(settings_id);
    }
    H5Gclose(grpid);
    
	if (status != IUS_E_OK)
    {
        return NULL;
    }
    return dict;
}
