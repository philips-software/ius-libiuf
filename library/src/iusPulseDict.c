//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <math.h>

#include <hashmap.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusPulseImp.h>
#include <iusParametricPulseImp.h>
#include <iusNonParametricPulseImp.h>
#include <iusPulseDict.h>
#include <assert.h>
#include <string.h>

// ADT
struct HashablePulse
{
    iup_t pulse;
    char key[256];
} ;

typedef struct HashablePulse HashablePulse;

struct IusPulseDict
{
    struct hashmap map;
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashablePulse, const char, struct HashablePulse)

// ADT
iupd_t iusPulseDictCreate
(
)
{
    iupd_t list = calloc(1, sizeof(IusPulseDict));
    if(list!=NULL)
    {
      hashmap_init(&list->map, hashmap_hash_string, hashmap_compare_string, 0);
    }
    return list;
}

int iusPulseDictDelete
(
  iupd_t dict
)
{
    if(dict == NULL) return IUS_ERR_VALUE;
    /* Free all allocated resources associated with map and reset its state */
    hashmap_destroy(&dict->map);
    free(dict);
    return IUS_E_OK;
}


static int iusPulseDictSourceInTarget
(
    iupd_t source,
    iupd_t target
)
{
    iup_t targetElement;
    iup_t sourceElement;
    HashablePulse *iterElement;

    IUS_BOOL sourceInTarget = IUS_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashablePulse_hashmap_iter_get_data(iter);
      sourceElement=iterElement->pulse;
      targetElement = iusPulseDictGet(target, iusPulseGetLabel(sourceElement));
      if( targetElement == IUP_INVALID)
        return IUS_FALSE;

      if( iusPulseCompare(sourceElement, targetElement) == IUS_FALSE )
        return IUS_FALSE;
      sourceInTarget = IUS_TRUE;
    }

  return sourceInTarget;
}

// operations
int iusPulseDictCompare
(
  iupd_t reference,
  iupd_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUS_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUS_TRUE;

    // check if elements of source hash are in target hash
    if( iusPulseDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusPulseDictSourceInTarget(actual,reference);
}


int iusPulseDictGetSize
(
    iupd_t dict
)
{
    if( dict == NULL )
        return -1;
    return (int) hashmap_size(&dict->map);
}

iup_t iusPulseDictGet
(
    iupd_t dict,
    char * key
)
{
    HashablePulse * search;
    search = HashablePulse_hashmap_get(&dict->map, key);
    if( dict == NULL )
        return IUP_INVALID;
    return search->pulse;
}

int iusPulseDictSet
(
    iupd_t dict,
    char * key,
    iup_t member
)
{
    if( dict == NULL ) return IUS_ERR_VALUE;
    if( key == NULL ) return IUS_ERR_VALUE;

    HashablePulse *newMember = calloc(1, sizeof(HashablePulse));
    newMember->pulse = member;
    strcpy(newMember->key,key);
    if (HashablePulse_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
      printf("discarding blob with duplicate key: %s\n", newMember->key);
      free(newMember);
      return IUS_ERR_VALUE;
    }
    return IUS_E_OK;
}


// serialization
#define NUMPULSEVALUESFMT  "%s/numPulseValues"
#define PULSEAMPLITUDESFMT "%s/rawPulseAmplitudes"
#define PULSETIMESFMT      "%s/rawPulseTimes"


#define LABELPATH          "%s/%s"
int iusPulseDictSave
(
    iupd_t dict,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[IUS_MAX_HDF5_PATH];
    struct hashmap_iter *iter;

    if(dict == NULL)
        return IUS_ERR_VALUE;
    if(parentPath == NULL || handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;



    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    HashablePulse *sourceElement;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        sourceElement = HashablePulse_hashmap_iter_get_data(iter);
        sprintf(path, LABELPATH, parentPath, sourceElement->pulse->label);
        iusPulseSave(sourceElement->pulse,path,handle);
    }

    status |= H5Gclose(group_id );
    return status;
}


#define MAX_NAME 1024

iupd_t iusPulseDictLoad
(
    hid_t handle,
    const char *parentPath
)
{
    int i;
    int status = 0;
    char path[IUS_MAX_HDF5_PATH];
    char memb_name[MAX_NAME];


    hid_t grpid = H5Gopen(handle, parentPath, H5P_DEFAULT);
    if(parentPath == NULL || handle == H5I_INVALID_HID || grpid == H5I_INVALID_HID)
        return NULL;

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iupd_t dict = iusPulseDictCreate();
    for (i = 0; i < nobj; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i,
                                    memb_name, (size_t) MAX_NAME);
        sprintf(path,"%s/%s", parentPath,memb_name);
        iup_t pulse = iusPulseLoad(handle,path);
        status = iusPulseDictSet(dict, memb_name, pulse);
    }

    H5Gclose(handle);
    if( status != IUS_E_OK )
    {
        return NULL;
    }
    return dict;
}
