//
// Created by nlv09165 on 12/09/2018.
//
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include <hashmap.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusParameterDictImp.h>
#include <include/iusHDF5.h>

// ADT
struct HashableParameter
{
    char *value;
    char *key;
} ;

typedef struct HashableParameter HashableParameter;

struct IusParameterDict
{
    struct hashmap map;
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableParameter, const char, struct HashableParameter)

// ADT
iupad_t iusParameterDictCreate
(
)
{
    iupad_t list = calloc(1, sizeof(IusParameterDict));
    if(list!=NULL)
    {
        hashmap_init(&list->map, hashmap_hash_string, hashmap_compare_string, 0);
    }
    return list;
}

int iusParameterDictDelete
(
    iupad_t dict
)
{
    if(dict == NULL) return IUS_ERR_VALUE;
    /* Free all allocated resources associated with map and reset its state */
    hashmap_destroy(&dict->map);
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


int iusParameterDictGetSize
(
    iupad_t dict
)
{
    if( dict == NULL )
        return -1;
    return (int) hashmap_size(&dict->map);
}

char * iusParameterDictGet
(
    iupad_t dict,
    char * key
)
{
    HashableParameter * search;
    search = HashableParameter_hashmap_get(&dict->map, key);
    if( dict == NULL )
        return NULL;
    return search->value;
}

int iusParameterDictSet
(
    iupad_t dict,
    char * key,
    char * value
)
{
    if( dict == NULL ) return IUS_ERR_VALUE;
    if( key == NULL ) return IUS_ERR_VALUE;

    HashableParameter *newMember = calloc(1, sizeof(HashableParameter));
    newMember->key = strdup(key);
    newMember->value = strdup(value);

    if (HashableParameter_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        printf("discarding blob with duplicate key: %s\n", newMember->key);
        free(newMember);
        return IUS_ERR_VALUE;
    }
    return IUS_E_OK;
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

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter)) {
        iterElement = HashableParameter_hashmap_iter_get_data(iter);
        returnValue |= iusHdf5WriteString(handle,iterElement->key,iterElement->value,0);
    }
    return returnValue;
}

#define MAX_NAME 1024

iupad_t iusParameterDictLoad
(
    hid_t handle
)
{
    hsize_t nobj;
    int i;
    int status = H5Gget_num_objs(handle, &nobj);
    char memberName[MAX_NAME];
    const char *memberValue;

    iupad_t dict = iusParameterDictCreate();
    for (i = 0; i < (int) nobj && status == IUS_E_OK; i++)
    {
        H5Gget_objname_by_idx(handle, i, memberName, (size_t)MAX_NAME);
        iusHdf5ReadString(handle,memberName, &memberValue);
        status = iusParameterDictSet(dict, memberName, (char *)memberValue);
    }
    return dict;
}

//
//// serialization
//#define NUMPULSEVALUESFMT  "%s/numParameterValues"
//#define PULSEAMPLITUDESFMT "%s/rawParameterAmplitudes"
//#define PULSETIMESFMT      "%s/rawParameterTimes"
//
//
//#define LABELPATH          "%s/%s"
//int iusParameterDictSave
//(
//    iupad_t dict,
//    char *parentPath,
//    hid_t handle
//)
//{
//    int status=0;
//    char path[IUS_MAX_HDF5_PATH];
//    struct hashmap_iter *iter;
//
//    if(dict == NULL)
//        return IUS_ERR_VALUE;
//    if(parentPath == NULL || handle == H5I_INVALID_HID)
//        return IUS_ERR_VALUE;
//
//
//
//    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//    HashableParameter *sourceElement;
//    char *label;
//    // iterate over source list elements and save'em
//    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
//    {
//        sourceElement = HashableParameter_hashmap_iter_get_data(iter);
//        label = iusParameterGetLabel(sourceElement->value);
//        sprintf(path, LABELPATH, parentPath, label);
//        iusParameterSave(sourceElement->value,path,handle);
//    }
//
//    status |= H5Gclose(group_id );
//    return status;
//}
//
//
//#define MAX_NAME 1024
//
//iupad_t iusParameterDictLoad
//(
//hid_t handle,
//const char *parentPath
//)
//{
//    int i;
//    int status = 0;
//    char path[IUS_MAX_HDF5_PATH];
//    char memb_name[MAX_NAME];
//
//
//    hid_t grpid = H5Gopen(handle, parentPath, H5P_DEFAULT);
//    if(parentPath == NULL || handle == H5I_INVALID_HID || grpid == H5I_INVALID_HID)
//        return NULL;
//
//    hsize_t nobj;
//    status = H5Gget_num_objs(grpid, &nobj);
//
//    iupad_t dict = iusParameterDictCreate();
//    for (i = 0; i < (int) nobj; i++)
//    {
//        H5Gget_objname_by_idx(grpid, (hsize_t) i,
//                              memb_name, (size_t) MAX_NAME);
//        sprintf(path,"%s/%s", parentPath,memb_name);
//        char * receiveSettings = iusParameterLoad(handle,path,memb_name);
//        status = iusParameterDictSet(dict, memb_name, receiveSettings);
//    }
//
//    H5Gclose(handle);
//    if( status != IUS_E_OK )
//    {
//        return NULL;
//    }
//    return dict;
//}
