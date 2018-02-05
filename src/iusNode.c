#include "iusNode.h"
//#include "iusInputFile.h"
#include "iusInput.h"
#include "iusError.h"
//#include "iusCWC.h"
//#include "iusCWCFile.h"
#include "iusHDF5.h"
#include <hdf5.h>
#include <H5LTpublic.h>
#include <H5Gpublic.h>
#include <string.h>


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
    IusInputInstance * pInst
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

    for ( i = 0; i < pNode->numberOfParents; i++ )
    {
        sprintf( groupName, "parent%d", i );

        group   = H5Gcreate( handle, groupName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
        pParent = pNode->pParents[i];
        iusNodeSaveAnyType( group, pParent );
        H5Gclose( group );
    }
}
