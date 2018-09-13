//
// Created by Ruijzendaal on 12/03/2018.
//
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusUuid.h>
#include <iusTypes.h>
#include <iusHDF5.h>
#include <iusError.h>
#include <hdf5_hl.h>
#include <iusHistoryNodeList.h>
#include <iusHistoryNode.h>

#define MAX_TYPE_LENGTH 40
#define MAX_ID_LENGTH   40
#define MAX_PARENTS     10

// ADT
struct IusHistoryNode
{
    char                pId[MAX_ID_LENGTH];
    char                *pType;
    int                 numberOfParameters;
    int                 numberOfParents;
    iuhnl_t             parents;
    iupad_t             parameters;
} ;

iuhn_t iusHistoryNodeCreate
(
    char *pNodeType,
    int parents
)
{
    if ( pNodeType == NULL ) return IUHN_INVALID;
    if ( parents < 0 ) return IUHN_INVALID;
    IusHistoryNode *pIusNode = calloc(1, sizeof( IusHistoryNode ));
    // Uuid should be generated for it.
    setIusUuidCreate( pIusNode->pId );
    pIusNode->pType = strdup(pNodeType);
    pIusNode->numberOfParents = parents;
    pIusNode->parents = iusHistoryNodeListCreate(parents);
    return pIusNode;
}

int iusHistoryNodeDelete
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    free(node);
    return IUS_E_OK;
}

IUS_BOOL iusHistoryNodeCompareWithId
(
    iuhn_t reference,
    iuhn_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( strcmp( reference->pId, actual->pId ) != 0 )
    {
        return IUS_FALSE;
    }
    return iusHistoryNodeCompare(reference,actual);
}

IUS_BOOL iusHistoryNodeCompare
(
    iuhn_t reference,
    iuhn_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->numberOfParents != actual->numberOfParents )
    {
        return IUS_FALSE;
    }

    if ( strcmp( reference->pType, actual->pType ) != 0 )
    {
        return IUS_FALSE;
    }

    if( iusHistoryNodeListCompare(reference->parents,actual->parents) == IUS_FALSE )
    {
        return IUS_FALSE;
    }

    return IUS_TRUE;
}


int iusHistoryNodeGetNumParents
(
    iuhn_t node
)
{
    if ( node == NULL ) return -1;
    return node->numberOfParents;
}

int iusHistoryNodeGetNumParams
(
    iuhn_t node
)
{
    if ( node == NULL ) return -1;
    return node->numberOfParameters;
}

iupad_t iusHistoryNodeGetParameters
(
iuhn_t node
)
{
    if ( node == NULL ) return IUPAD_INVALID;
    return node->parameters;
}

char *iusHistoryNodeGetId(iuhn_t node) {
    if ( node == NULL ) return NULL;
    return node->pId;
}

char *iusHistoryNodeGetType(iuhn_t node) {
    if ( node == NULL ) return NULL;
    return node->pType;
}

iuhnl_t iusHistoryNodeGetParents
(
    iuhn_t node
)
{
    if ( node == NULL ) return IUHNL_INVALID;
    return node->parents;
}

int iusHistoryNodeSetParents
(
    iuhn_t node,
    iuhnl_t parents
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( parents == NULL ) return IUS_ERR_VALUE;
    node->parents = parents;
    return IUS_E_OK;
}

int iusHistoryNodeSetParameters
(
iuhn_t node,
iupad_t parameterDict
)
{
    if ( node == NULL ) return IUS_ERR_VALUE;
    if ( parameterDict == NULL ) return IUS_ERR_VALUE;
    node->parameters = parameterDict;
    return IUS_E_OK;
}

//
////--------------------------------------------------------------------------------
////
////--------------------------------------------------------------------------------
//IusNode * iusHistoryNodeLoadInputType
//    (
//        hid_t handle
//    )
//{
//// Method for reading IusInputInstance struct from handle is in iusInput.h/c
////    IusInputInstance * pInst = iusInputRead( handle, 0 );
//
////    return (IusNode *)pInst;
//    // nlv09165: TODO: Implement
//    IUS_ASSERT(handle != handle);
//    return NULL;
//}
//
//
////--------------------------------------------------------------------------------
////
////--------------------------------------------------------------------------------
//void iusHistoryNodeSaveInputType
//    (
//        hid_t              handle,
//        iuii_t             pInst
//    )
//{
//    // Method for writing iusInputIsntance into hdf handle. Also in iusInput.h/c
//    // nlv09165: TODO: Implement
//    IUS_ASSERT(handle != handle);
//    IUS_ASSERT(pInst != pInst);
//
//}
//
//
////--------------------------------------------------------------------------------
////
////--------------------------------------------------------------------------------
//IusNode * iusHistoryNodeLoadCwcType
//    (
//        hid_t handle
//    )
//{
//    IUS_ASSERT(handle != handle);
//    // nlv09165: TODO: Implement
//    return NULL; // This method belongs to the iusCwxFile.h/c. Use the handle to
//    // load the data, and call HandleParents load the parent data.
//    // Then return the struct.
//}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
//void iusHistoryNodeSaveCwcType
//(
//    hid_t            handle,
//    IusCWCInstance * pInst
//)
//{
// Method for writing IusCWCInstance into hdf handle.
// Found in iusCWCFile.h/c
//}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
//IusNode * iusHistoryNodeLoadAnyType
//    (
//        hid_t handle
//    )
//{
//    char type[MAX_TYPE_LENGTH];
//    IusNode * pNode = NULL;
//
//    H5LTread_dataset_string( handle, "type", type );
//
//    if ( strcmp(type, IUS_INPUT_TYPE) == 0 )
//    {
//        pNode = iusHistoryNodeLoadInputType( handle );
//    }
//    // if ( strcmp(type, IUS_CWC_TYPE) == 0 )
//    //{
//    //    pNode = LoadCwcType(handle);
//    //}
//
//    if ( pNode == NULL )
//    {
//        fprintf (stderr,
//                 "Failed to load data structure for type: <hdf type here> \n");
//    }
//
//    return pNode;
//}
//
//
////--------------------------------------------------------------------------------
////
////--------------------------------------------------------------------------------
//void iusHistoryNodeSaveAnyType
//    (
//        hid_t     handle,
//        IusNode * pNode
//    )
//{
//    if ( strcmp( pNode->pType, IUS_INPUT_TYPE) == 0 )
//    {
//        iusHistoryNodeSaveInputType( handle, (IusInputInstance *)pNode );
//    }
//    //if ( strcmp( node->pType, Ius_CWC_TYPE) == 0 )
//    //{
//    //    SaveCwcType( handle, (IusCWCInstance *)pNode );
//    //}
//}
//
//
////--------------------------------------------------------------------------------
////
////--------------------------------------------------------------------------------
//void iusHistoryNodeLoadParents
//    (
//        IusNode * pNode,
//        hid_t     handle
//    )
//{
//    int   numberOfParents = 0;
//    int   i = 0;
//    char  groupName[20];
//    hid_t group;
//
//    H5LTread_dataset_int( handle, "numberOfParents", &numberOfParents );
//
//    if ( numberOfParents > MAX_PARENTS )
//    {
//        fprintf( stderr, "ERROR: Node claims to exceed maximum number of "
//                         "parents. Node: %d, Max: %d.\n", numberOfParents, MAX_PARENTS );
//        return;
//    }
//
//    for ( i = 0; i < numberOfParents; i++ )
//    {
//        sprintf( groupName, "parent%d", i );
//
//        group = H5Gopen( handle, groupName, H5P_DEFAULT );
//        pNode->pParents[i] = iusHistoryNodeLoadAnyType( group );
//        H5Gclose(group);
//    }
//}
//
//
////--------------------------------------------------------------------------------
////
////--------------------------------------------------------------------------------
//void iusHistoryNodeSaveParents
//    (
//        IusNode * pNode,
//        hid_t     handle
//    )
//{
//    int       i = 0;
//    char      groupName[20];
//    IusNode * pParent;
//    hid_t     group;
//    hsize_t dims[1] = {1};
//
//    H5LTmake_dataset_int(handle, "numberOfParents", 1, dims, &(pNode->numberOfParents));
//
//    for (i = 0; i < pNode->numberOfParents; i++)
//    {
//        sprintf(groupName, "parent%d", i);
//
//        group   = H5Gcreate(handle, groupName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//        pParent = pNode->pParents[i];
//        iusHistoryNodeSaveAnyType(group, pParent);
//        H5Gclose(group);
//    }
//}
//
//
//int iusReadNode(IusNode *pNode, hid_t handle, int verbose) {
//    const char *pString;
//    int status = 0;
//    status |= iusHdf5ReadString( handle, "/ID", &pString );
//    if( status == 0 ) strcpy(pNode->pId,pString);
//    status |= iusHdf5ReadString( handle, "/type", &pString );
//    if( status == 0 ) strcpy(pNode->pType,pString);
//    iusHistoryNodeLoadParents((IusNode *)pNode, handle);
//    return status;
//}
//
//
//
//int iusWriteNode(IusNode *pNode, hid_t handle, int verbose) {
//    herr_t status = 0;
//    status |= iusHdf5WriteString( handle , "/ID",    pNode->pId, 1 );
//    status |= iusHdf5WriteString( handle , "/type",    pNode->pType, 1 );
//    iusHistoryNodeSaveParents( pNode, handle );
//    return status;
//}
