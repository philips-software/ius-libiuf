//
// Created by nlv12901 on 08/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufPatternListDictADT.h>
#include <iufPatternListPrivate.h>



/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashablePatternList, const char, struct HashablePatternList)

// ADT
iupald_t iufPatternListDictCreate
(
)
{
	iupald_t dict = calloc(1, sizeof(IufPatternListDict));
	IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufPatternListDict, IUPALD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
	return dict;
}



int iufPatternListDictDeepDelete
(
    iupald_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    dict->deepDelete = IUF_TRUE;
    return iufPatternListDictDelete(dict);
}

static void iufPatternListDictDeleteKeys
(
    iupald_t dict
)
{
    if (dict->kys != NULL)
        free(dict->kys);
}

int iufPatternListDictDelete
(
	iupald_t dict
)
{
    HashablePatternList *iterElement;
    struct hashmap_iter *iter;
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);

	/* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashablePatternList_hashmap_iter_get_data(iter);
        if (dict->deepDelete == IUF_TRUE)
            iufPatternListDeepDelete(iterElement->patternList);
        free(iterElement);
    }
	hashmap_destroy(&dict->map);
    iufPatternListDictDeleteKeys(dict);
	free(dict);
	return IUF_E_OK;
}


static int iufPatternListDictSourceInTarget
(
	iupald_t source,
	iupald_t target
)
{
	iupal_t targetElement;
	iupal_t sourceElement;
	HashablePatternList *iterElement;

	IUF_BOOL sourceInTarget = IUF_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashablePatternList_hashmap_iter_get_data(iter);
		sourceElement = iterElement->patternList;
		targetElement = iufPatternListDictGet(target, iterElement->key);
		if (targetElement == IUPAL_INVALID)
			return IUF_FALSE;

		if (iufPatternListCompare(sourceElement, targetElement) == IUF_FALSE)
			return IUF_FALSE;
		sourceInTarget = IUF_TRUE;
	}

	return sourceInTarget;
}

// operations
int iufPatternListDictCompare
(
	iupald_t reference,
	iupald_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUF_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUF_TRUE;

	// check if elements of source hash are in target hash
	if (iufPatternListDictSourceInTarget(reference, actual) == IUF_FALSE)
		return IUF_FALSE;

	// check if elements of target hash are in source hash
	return iufPatternListDictSourceInTarget(actual, reference);
}


size_t iufPatternListDictGetSize
(
	iupald_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return (int)hashmap_size(&dict->map);
}

iupal_t iufPatternListDictGet
(
	iupald_t dict,
	char * key
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUPAL_INVALID);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUPAL_INVALID);
	HashablePatternList * search;
	search = HashablePatternList_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUPAL_INVALID;
	}

	return search->patternList;
}

char **iufPatternListDictGetKeys
(
    iupald_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufPatternListDictUpdateKeys
(
    iupald_t dict
)
{
    iufPatternListDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufPatternListDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashablePatternList *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashablePatternList_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufPatternListDictSet
(
	iupald_t dict,
	char * key,
	iupal_t member
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
	HashablePatternList *newMember = calloc(1, sizeof(HashablePatternList));
	newMember->patternList = member;
	strcpy(newMember->key, key);
	if (HashablePatternList_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUF_ERR_VALUE;
	}
	return iufPatternListDictUpdateKeys(dict);
}


// serialization
int iufPatternListDictSave
(
	iupald_t dict,
	hid_t handle
)
{
	int status = 0;
	struct hashmap_iter *iter;

	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	hid_t group_id;
	status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_PATTERNLISTDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_PATTERNLISTDICT);
        return IUF_ERR_VALUE;
    }

	status = 0;
	HashablePatternList *patternListDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == IUF_E_OK; iter = hashmap_iter_next(&dict->map, iter))
	{
		patternListDictItem = HashablePatternList_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, patternListDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (subgroup_id <= 0)
        {
            H5Gclose(group_id);
            return IUF_ERR_VALUE;
        }

		iufPatternListSave(patternListDictItem->patternList, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}
	status |= H5Gclose(group_id);
	return status;
}


#define MAX_NAME 1024

iupald_t iufPatternListDictLoad
(
	hid_t handle
)
{
    hsize_t i;
	int status;
	char memb_name[MAX_NAME];

	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPALD_INVALID);
	hid_t grpid = H5Gopen(handle, IUF_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_PATTERNLISTDICT);
        return IUPALD_INVALID;
    }


	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iupald_t dict = iufPatternListDictCreate();
	for (i = 0; i < nobj && status == IUF_E_OK; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, memb_name, (size_t)MAX_NAME);
		hid_t patternList_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
		iupal_t patternList = iufPatternListLoad(patternList_id);
		status |= H5Gclose(patternList_id);
		status |= iufPatternListDictSet(dict, memb_name, patternList);
	}
	H5Gclose(grpid);
	if (status != IUF_E_OK)
	{
		return IUPALD_INVALID;
	}
	dict->deepDelete = IUF_TRUE;
	return dict;
}
