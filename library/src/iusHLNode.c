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
#include <iusNode.h>
#include <iusHLNode.h>
#include <iusHLInputInstance.h>

#define IUSLIBRARY_IMPLEMENTATION
#define MAX_TYPE_LENGTH 40
#define MAX_ID_LENGTH   40
#define MAX_PARENTS     10

// ADT
struct IusNode
{
    char                pId[MAX_ID_LENGTH];
    char                pType[MAX_TYPE_LENGTH];
    int                 numberOfParents;
    struct IusNode * pParents[MAX_PARENTS];
} ;

iun_t iusHLNodeCreate
(
char *pNodeType,
int parents
)
{
    IusNode *pIusNode = calloc(1, sizeof( IusNode ));
    // Uuid should be generated for it.
    setIusUuidCreate( pIusNode->pId );
    strncpy( pIusNode->pType, pNodeType, MAX_TYPE_LENGTH );
    return pIusNode;
}

int iusHLNodeDelete
(
iun_t node
)
{
    if( node == NULL ) return IUS_ERR_VALUE;
    free((IusNode *) node);
    return IUS_E_OK;
}

IUS_BOOL iuHLNodeCompare(IusNode *pReferenceNode, IusNode *pActualNode)
{
    if( pReferenceNode->numberOfParents != pActualNode->numberOfParents ){
        return IUS_FALSE;
    }
    if( strcmp( pReferenceNode->pType, pActualNode->pType ) != 0 ){
        return IUS_FALSE;
    }
    if( strcmp( pReferenceNode->pId, pActualNode->pId ) != 0 ){
        return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHLNodeGetNumParents( iun_t node ) {
    return node->numberOfParents;
}

char *iusHLNodeGetId(iun_t node) {
    return node->pId;
}

char *iusHLNodeGetType(iun_t node) {
    return node->pType;
}



//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
IusNode * iusNodeLoadInputType
    (
        hid_t handle
    )
{
// Method for reading IusInputInstance struct from handle is in iusInput.h/c
//    IusInputInstance * pInst = iusInputRead( handle, 0 );

//    return (IusNode *)pInst;
    // nlv09165: TODO: Implement
    IUS_ASSERT(handle != handle);
    return NULL;
}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void iusNodeSaveInputType
    (
        hid_t              handle,
        iuii_t             pInst
    )
{
    // Method for writing iusInputIsntance into hdf handle. Also in iusInput.h/c
    // nlv09165: TODO: Implement
    IUS_ASSERT(handle != handle);
    IUS_ASSERT(pInst != pInst);

}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
IusNode * iusNodeLoadCwcType
    (
        hid_t handle
    )
{
    IUS_ASSERT(handle != handle);
    // nlv09165: TODO: Implement
    return NULL; // This method belongs to the iusCwxFile.h/c. Use the handle to
    // load the data, and call HandleParents load the parent data.
    // Then return the struct.
}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
//void iusNodeSaveCwcType
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
IusNode * iusNodeLoadAnyType
    (
        hid_t handle
    )
{
    char type[MAX_TYPE_LENGTH];
    IusNode * pNode = NULL;

    H5LTread_dataset_string( handle, "type", type );

    if ( strcmp(type, IUS_INPUT_TYPE) == 0 )
    {
        pNode = iusNodeLoadInputType( handle );
    }
    // if ( strcmp(type, IUS_CWC_TYPE) == 0 )
    //{
    //    pNode = LoadCwcType(handle);
    //}

    if ( pNode == NULL )
    {
        fprintf (stderr,
                 "Failed to load data structure for type: <hdf type here> \n");
    }

    return pNode;
}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void iusNodeSaveAnyType
    (
        hid_t     handle,
        IusNode * pNode
    )
{
    if ( strcmp( pNode->pType, IUS_INPUT_TYPE) == 0 )
    {
        iusNodeSaveInputType( handle, (IusInputInstance *)pNode );
    }
    //if ( strcmp( node->pType, Ius_CWC_TYPE) == 0 )
    //{
    //    SaveCwcType( handle, (IusCWCInstance *)pNode );
    //}
}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void iusNodeLoadParents
    (
        IusNode * pNode,
        hid_t     handle
    )
{
    int   numberOfParents = 0;
    int   i = 0;
    char  groupName[20];
    hid_t group;

    H5LTread_dataset_int( handle, "numberOfParents", &numberOfParents );

    if ( numberOfParents > MAX_PARENTS )
    {
        fprintf( stderr, "ERROR: Node claims to exceed maximum number of "
                         "parents. Node: %d, Max: %d.\n", numberOfParents, MAX_PARENTS );
        return;
    }

    for ( i = 0; i < numberOfParents; i++ )
    {
        sprintf( groupName, "parent%d", i );

        group = H5Gopen( handle, groupName, H5P_DEFAULT );
        pNode->pParents[i] = iusNodeLoadAnyType( group );
        H5Gclose(group);
    }
}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void iusNodeSaveParents
    (
        IusNode * pNode,
        hid_t     handle
    )
{
    int       i = 0;
    char      groupName[20];
    IusNode * pParent;
    hid_t     group;
    hsize_t dims[1] = {1};

    H5LTmake_dataset_int(handle, "numberOfParents", 1, dims, &(pNode->numberOfParents));

    for (i = 0; i < pNode->numberOfParents; i++)
    {
        sprintf(groupName, "parent%d", i);

        group   = H5Gcreate(handle, groupName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        pParent = pNode->pParents[i];
        iusNodeSaveAnyType(group, pParent);
        H5Gclose(group);
    }
}


int iusReadNode(IusNode *pNode, hid_t handle, int verbose) {
    char *pString;
    int status = 0;
    status |= iusHdf5ReadString( handle, "/ID", &pString, verbose );
    if( status == 0 ) strcpy(pNode->pId,pString);
    status |= iusHdf5ReadString( handle, "/type", &pString, verbose );
    if( status == 0 ) strcpy(pNode->pType,pString);
    iusNodeLoadParents((IusNode *)pNode, handle);
    return status;
}



int iusWriteNode(IusNode *pNode, hid_t handle, int verbose) {
    herr_t status = 0;
    status |= iusHdf5WriteString( handle , "/ID",    pNode->pId, 1,   verbose );
    status |= iusHdf5WriteString( handle , "/type",    pNode->pType, 1,   verbose );
    iusNodeSaveParents( pNode, handle );
    return status;
}