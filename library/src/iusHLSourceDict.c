//
// Created by nlv09165 on 31/07/2018.
//
#include <stdlib.h>
#include <math.h>

#include <hashmap.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLSourceImp.h>
#include <iusHLSourceDict.h>
#include <assert.h>
#include <string.h>

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
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableSource, const char, struct HashableSource)

// ADT
iusd_t iusHLSourceDictCreate
(
)
{
    iusd_t list = calloc(1, sizeof(IusSourceDict));
    if(list!=NULL)
    {
      hashmap_init(&list->map, hashmap_hash_string, hashmap_compare_string, 0);
    }
    return list;
}

int iusHLSourceDictDelete
(
  iusd_t dict
)
{
    if(dict == NULL) return IUS_ERR_VALUE;
    /* Free all allocated resources associated with map and reset its state */
    hashmap_destroy(&dict->map);
    free(dict);
    return IUS_E_OK;
}


static int iusHLSourceDictSourceInTarget
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
      targetElement = iusHLSourceDictGet(target, iusHLSourceGetLabel(sourceElement));
      if( targetElement == IUS_INVALID)
        return IUS_FALSE;

      if( iusHLSourceCompare(sourceElement, targetElement) == IUS_FALSE )
        return IUS_FALSE;
      sourceInTarget = IUS_TRUE;
    }

  return sourceInTarget;
}

// operations
int iusHLSourceDictCompare
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
    if( iusHLSourceDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusHLSourceDictSourceInTarget(actual,reference);
}


int iusHLSourceDictGetSize
(
    iusd_t dict
)
{
    if( dict == NULL )
        return -1;
    return (int) hashmap_size(&dict->map);
}

ius_t iusHLSourceDictGet
(
    iusd_t dict,
    char * key
)
{
    HashableSource * search;
    search = HashableSource_hashmap_get(&dict->map, key);
    if( dict == NULL )
        return IUS_INVALID;
    return search->source;
}

int iusHLSourceDictSet
(
    iusd_t dict,
    char * key,
    ius_t member
)
{
    if( dict == NULL ) return IUS_ERR_VALUE;
    if( key == NULL ) return IUS_ERR_VALUE;

    HashableSource *newMember = calloc(1, sizeof(HashableSource));
    newMember->source = member;
    strcpy(newMember->key,key);
    if (HashableSource_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
      printf("discarding blob with duplicate key: %s\n", newMember->key);
      free(newMember);
      return IUS_ERR_VALUE;
    }
    return IUS_E_OK;
}


// serialization
//#define NUMPULSEVALUESFMT  "%s/numSourceValues"
//#define PULSEAMPLITUDESFMT "%s/rawSourceAmplitudes"
//#define PULSETIMESFMT      "%s/rawSourceTimes"
//#define LABELPATH          "%s/%s"
int iusHLSourceDictSave
(
    iusd_t dict,
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

    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    HashableSource *sourceElement;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        sourceElement = HashableSource_hashmap_iter_get_data(iter);
        //sprintf(path, LABELPATH, parentPath, sourceElement->source->label);
        iusHLSourceSave(sourceElement->source, handle);
    }

    //status |= H5Gclose(group_id );
    return status;
}


#define MAX_NAME 1024

iusd_t iusHLSourceDictLoad
(
    hid_t handle
)
{
    int i;
    int status = 0;
    //char path[IUS_MAX_HDF5_PATH];
    char memb_name[MAX_NAME];


    hid_t grpid = H5Gopen(handle, "Sources", H5P_DEFAULT);
    if(handle == H5I_INVALID_HID)
        return NULL;

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iusd_t dict = iusHLSourceDictCreate();
    for (i = 0; i < nobj; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i,
                                    memb_name, (size_t) MAX_NAME);
        //sprintf(path,"%s", memb_name);
        ius_t source = iusHLSourceLoad(grpid, memb_name);
        status = iusHLSourceDictSet(dict, memb_name, source);
    }

    H5Gclose(grpid);
    if( status != IUS_E_OK )
    {
        return NULL;
    }
    return dict;
}
