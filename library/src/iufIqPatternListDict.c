//
// Created by nlv12901 on 08/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufIqPatternListDictADT.h>
#include <iufIqPatternListPrivate.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableIqPatternList, const char, struct HashableIqPatternList)

// ADT
iuiqpald_t iufIqPatternListDictCreate
(
)
{
	iuiqpald_t dict = calloc(1, sizeof(IufIqPatternListDict));
	IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufIqPatternListDict, IUIQPALD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUF_FALSE;
	dict->kys = NULL;
	return dict;
}



int iufIqPatternListDictDeepDelete
(
    iuiqpald_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	dict->deepDelete = IUF_TRUE;
	return iufIqPatternListDictDelete(dict);
}

static void iufIqPatternListDictDeleteKeys
(
	iuiqpald_t dict
)
{
	if (dict->kys != NULL)
		free(dict->kys);
}

int iufIqPatternListDictDelete
(
	iuiqpald_t dict
)
{
	HashableIqPatternList *iterElement;
	struct hashmap_iter *iter;
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);

	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
		if (dict->deepDelete == IUF_TRUE)
			iufIqPatternListDeepDelete(iterElement->iqPatternList);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
	iufIqPatternListDictDeleteKeys(dict);
	free(dict);
	return IUF_E_OK;
}


static int iufIqPatternListDictSourceInTarget
(
	iuiqpald_t source,
	iuiqpald_t target
)
{
	iuiqpal_t targetElement;
	iuiqpal_t sourceElement;
	HashableIqPatternList *iterElement;

	IUF_BOOL sourceInTarget = IUF_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
		sourceElement = iterElement->iqPatternList;
		targetElement = iufIqPatternListDictGet(target, iterElement->key);
		if (targetElement == IUIQPAL_INVALID)
			return IUF_FALSE;

		if (iufIqPatternListCompare(sourceElement, targetElement) == IUF_FALSE)
			return IUF_FALSE;
		sourceInTarget = IUF_TRUE;
	}

	return sourceInTarget;
}

// operations
int iufIqPatternListDictCompare
(
	iuiqpald_t reference,
	iuiqpald_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUF_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUF_TRUE;

	// check if elements of source hash are in target hash
	if (iufIqPatternListDictSourceInTarget(reference, actual) == IUF_FALSE)
		return IUF_FALSE;

	// check if elements of target hash are in source hash
	return iufIqPatternListDictSourceInTarget(actual, reference);
}


size_t iufIqPatternListDictGetSize
(
	iuiqpald_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t)-1);
	return (int)hashmap_size(&dict->map);
}

char **iufIqPatternListDictGetKeys
(
	iuiqpald_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
	return dict->kys;
}

iuiqpal_t iufIqPatternListDictGet
(
	iuiqpald_t dict,
	char * key
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUIQPAL_INVALID);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUIQPAL_INVALID);
	HashableIqPatternList * search;
	search = HashableIqPatternList_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUIQPAL_INVALID;
	}
	return search->iqPatternList;
}

static int iufIqPatternListDictUpdateKeys
(
	iuiqpald_t dict
)
{
	iufIqPatternListDictDeleteKeys(dict);
	// allocate memory for the keys
	int keyIndex;
	size_t size = iufIqPatternListDictGetSize(dict);
	dict->kys = calloc(size + 1, sizeof(char*));
	IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

	struct hashmap_iter *iter;
	HashableIqPatternList *iterElement;
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map), keyIndex = 0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
	{
		iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
		dict->kys[keyIndex] = iterElement->key;
	}
	dict->kys[keyIndex] = NULL;
	return IUF_E_OK;
}

int iufIqPatternListDictSet
(
	iuiqpald_t dict,
	char * key,
	iuiqpal_t member
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);

	HashableIqPatternList *newMember = calloc(1, sizeof(HashableIqPatternList));
	newMember->iqPatternList = member;
	strcpy(newMember->key, key);
	if (HashableIqPatternList_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUF_ERR_VALUE;
	}
	return iufIqPatternListDictUpdateKeys(dict);;
}


// serialization
int iufIqPatternListDictSave
(
	iuiqpald_t dict,
	hid_t handle
)
{
	int status = 0;
	struct hashmap_iter *iter;

	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	hid_t group_id;
	status = H5Gget_objinfo(handle, IUF_PATH_PATTERNLISTDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_PATH_PATTERNLISTDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	}
	if (group_id == H5I_INVALID_HID)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_PATH_PATTERNLISTDICT);
		return IUF_ERR_VALUE;
	}
	status = 0;
	HashableIqPatternList *iqPatternListDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == IUF_E_OK; iter = hashmap_iter_next(&dict->map, iter))
	{
		iqPatternListDictItem = HashableIqPatternList_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, iqPatternListDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		if (subgroup_id <= 0)
		{
			H5Gclose(group_id);
			return IUF_ERR_VALUE;
		}
		iufIqPatternListSave(iqPatternListDictItem->iqPatternList, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}
	status |= H5Gclose(group_id);
	return status;
}


#define MAX_NAME 1024

iuiqpald_t iufIqPatternListDictLoad
(
	hid_t handle
)
{
    hsize_t i;
	int status;
	char memb_name[MAX_NAME];

	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUIQPALD_INVALID);
	hid_t grpid = H5Gopen(handle, IUF_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	if (grpid == H5I_INVALID_HID)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_PATH_PATTERNLISTDICT);
		return IUIQPALD_INVALID;
	}

	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iuiqpald_t dict = iufIqPatternListDictCreate();
	for (i = 0; i < nobj && status == IUF_E_OK; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, memb_name, (size_t)MAX_NAME);
		hid_t patternList_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
		iuiqpal_t patternList = iufIqPatternListLoad(patternList_id);
		status |= H5Gclose(patternList_id);
		status |= iufIqPatternListDictSet(dict, memb_name, patternList);
	}
	H5Gclose(grpid);
	if (status != IUF_E_OK)
	{
		return IUIQPALD_INVALID;
	}
	dict->deepDelete = IUF_TRUE;
	return dict;
}
