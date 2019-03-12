//
// Created by nlv12901 on 08/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusPatternListDictADT.h>
#include <iusPatternListPrivate.h>



/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashablePatternList, const char, struct HashablePatternList)

// ADT
iupald_t iusPatternListDictCreate
(
)
{
	iupald_t dict = calloc(1, sizeof(IusPatternListDict));
	IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusPatternListDict, IUPALD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUS_FALSE;
    dict->kys = NULL;
	return dict;
}



int iusPatternListDictDeepDelete
(
    iupald_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    dict->deepDelete = IUS_TRUE;
    return iusPatternListDictDelete(dict);
}

static void iusPatternListDictDeleteKeys
(
    iupald_t dict
)
{
    if (dict->kys != NULL)
        free(dict->kys);
}

int iusPatternListDictDelete
(
	iupald_t dict
)
{
    HashablePatternList *iterElement;
    struct hashmap_iter *iter;
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);

	/* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashablePatternList_hashmap_iter_get_data(iter);
        if (dict->deepDelete == IUS_TRUE)
            iusPatternListDeepDelete(iterElement->patternList);
        free(iterElement);
    }
	hashmap_destroy(&dict->map);
    iusPatternListDictDeleteKeys(dict);
	free(dict);
	return IUS_E_OK;
}


static int iusPatternListDictSourceInTarget
(
	iupald_t source,
	iupald_t target
)
{
	iupal_t targetElement;
	iupal_t sourceElement;
	HashablePatternList *iterElement;

	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashablePatternList_hashmap_iter_get_data(iter);
		sourceElement = iterElement->patternList;
		targetElement = iusPatternListDictGet(target, iterElement->key);
		if (targetElement == IUPAL_INVALID)
			return IUS_FALSE;

		if (iusPatternListCompare(sourceElement, targetElement) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusPatternListDictCompare
(
	iupald_t reference,
	iupald_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusPatternListDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusPatternListDictSourceInTarget(actual, reference);
}


size_t iusPatternListDictGetSize
(
	iupald_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return (int)hashmap_size(&dict->map);
}

iupal_t iusPatternListDictGet
(
	iupald_t dict,
	char * key
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUPAL_INVALID);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUPAL_INVALID);
	HashablePatternList * search;
	search = HashablePatternList_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUPAL_INVALID;
	}

	return search->patternList;
}

char **iusPatternListDictGetKeys
(
    iupald_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iusPatternListDictUpdateKeys
(
    iupald_t dict
)
{
    iusPatternListDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iusPatternListDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

    struct hashmap_iter *iter;
    HashablePatternList *iterElement;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashablePatternList_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUS_E_OK;
}

int iusPatternListDictSet
(
	iupald_t dict,
	char * key,
	iupal_t member
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);
	HashablePatternList *newMember = calloc(1, sizeof(HashablePatternList));
	newMember->patternList = member;
	strcpy(newMember->key, key);
	if (HashablePatternList_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return iusPatternListDictUpdateKeys(dict);
}


// serialization
int iusPatternListDictSave
(
	iupald_t dict,
	hid_t handle
)
{
	int status = 0;
	struct hashmap_iter *iter;

	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	hid_t group_id;
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_PATTERNLISTDICT);
        return IUS_ERR_VALUE;
    }

	status = 0;
	HashablePatternList *patternListDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == IUS_E_OK; iter = hashmap_iter_next(&dict->map, iter))
	{
		patternListDictItem = HashablePatternList_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, patternListDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (subgroup_id <= 0)
        {
            H5Gclose(group_id);
            return IUS_ERR_VALUE;
        }

		iusPatternListSave(patternListDictItem->patternList, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}
	status |= H5Gclose(group_id);
	return status;
}


#define MAX_NAME 1024

iupald_t iusPatternListDictLoad
(
	hid_t handle
)
{
    hsize_t i;
	int status;
	char memb_name[MAX_NAME];

	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPALD_INVALID);
	hid_t grpid = H5Gopen(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_PATTERNLISTDICT);
        return IUPALD_INVALID;
    }


	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iupald_t dict = iusPatternListDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, memb_name, (size_t)MAX_NAME);
		hid_t patternList_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
		iupal_t patternList = iusPatternListLoad(patternList_id); 
		status |= H5Gclose(patternList_id);
		status |= iusPatternListDictSet(dict, memb_name, patternList);
	}
	H5Gclose(grpid);
	if (status != IUS_E_OK)
	{
		return IUPALD_INVALID;
	}
	dict->deepDelete = IUS_TRUE;
	return dict;
}
