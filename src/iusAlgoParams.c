//=============================================================================
//    COPYRIGHT 2018 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusAlgoParams.c
//                  IUS algorithm parameter access
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusAlgoParams.h"
#include "iusTypes.h"
#include "iusError.h"
#include "iusHDF5.h"

#include <hdf5.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <H5LTpublic.h>

// <Local functions>


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusAlgorithmParameter * LF_CreateAlgorithmParameter
(
    const char * const pName
)
{
    IusAlgorithmParameter * pInst;

    pInst = (IusAlgorithmParameter *)calloc( 1, sizeof(IusAlgorithmParameter) );

    strcpy( pInst->pName, pName );
    pInst->length = 0;
    pInst->type   = IUS_ALGOPARAMT_NONE;
    pInst->pValue  = NULL;

    return pInst;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_DestroyAlgorithmParameter
(
    IusAlgorithmParameter * pInst
)
{
    if ( pInst == NULL )
    {
        return;
    }
    free( pInst->pName );

    if ( pInst->length > 0 )
    {
        free( pInst->pValue );
        pInst->pValue = NULL;
        pInst->length = 0;
    }
    free( pInst );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusAlgorithmParameter * LF_AddNewParam
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName
)
{
    int i;
    IusAlgorithmParameter * pParam;

    if ( pInst->numberOfParameters == MAX_ALGO_PARAMETERS )
    {
        fprintf( stderr,
            "AlgorithmParameters: Cannot add parameter. Reached max length!" );
        return NULL;
    }

    i = pInst->numberOfParameters;

    pParam = LF_CreateAlgorithmParameter( pName );

    pInst->pParameters[i] = pParam;
    pInst->numberOfParameters++;

    return pParam;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusAlgorithmParameter * LF_FindParamByName
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName
)
{
    int i;

    for ( i = 0; i < pInst->numberOfParameters; i++)
    {
        if( strcmp( pName, pInst->pParameters[i]->pName) == 0 )
        {
            return pInst->pParameters[i];
        }
    }

    fprintf( stderr,
             "AlgorithmParameters: Failed to find parameter by name: %s",
             pName );

    return NULL;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_setInt
(
    IusAlgorithmParameter * const pInst,
    int                           value
)
{
    int * pValue;

    if ( pInst == NULL )
    {
        return;
    }

    pValue = (int *)calloc( 1, sizeof(int) );
    IUS_ASSERT_MEMORY( pValue );

    (*pValue) = value;

    pInst->length = 1;
    pInst->type   = IUS_ALGOPARAMT_INT;
    pInst->pValue = (void *)pValue;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_setFloat
(
    IusAlgorithmParameter * const pInst,
    float                         value
)
{
    float * pValue;

    if ( pInst == NULL )
    {
        return;
    }

    pValue = (float *)calloc( 1, sizeof(float) );
    IUS_ASSERT_MEMORY( pValue );

    (*pValue) = value;

    pInst->length = 1;
    pInst->type   = IUS_ALGOPARAMT_FLOAT;
    pInst->pValue  = (void *)pValue;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_setString
(
    IusAlgorithmParameter * const pInst,
    const char * const            pValue
)
{
    if ( pInst == NULL )
    {
        return;
    }

    pInst->length = strlen( pValue );
    pInst->type   = IUS_ALGOPARAMT_STRING;
    pInst->pValue = calloc( pInst->length + 1, sizeof(char) ); // + 1 for null-terminator, I think...

    strcpy( (char*)pInst->pValue, pValue );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_getInt
(
    const IusAlgorithmParameter * const pInst
)
{
    int * pValue;

    if ( pInst == NULL )
    {
        return 0;
    }
    if ( pInst->type != IUS_ALGOPARAMT_INT )
    {
        fprintf( stderr, "iusAlgorithmParameters: type mismatch" );
        return 0;
    }

    pValue = (int*)pInst->pValue;

    return *pValue;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float LF_getFloat
(
    const IusAlgorithmParameter * const pInst
)
{
    float * pValue;

    if ( pInst == NULL )
    {
        return 0.0;
    }
    if ( pInst->type != IUS_ALGOPARAMT_FLOAT )
    {
        fprintf( stderr, "iusAlgorithmParameters: type mismatch" );
        return 0;
    }

    pValue = (float *)pInst->pValue;

    return *pValue;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_getString
(
    const IusAlgorithmParameter * const pInst,
    char * const                        pBuffer
)
{
    char * pValue;

    if ( pInst == NULL )
    {
        return;
    }
    if ( pInst->type != IUS_ALGOPARAMT_STRING )
    {
        fprintf( stderr, "iusAlgorithmParameters: type mismatch" );
        return;
    }

    strcpy( pBuffer, (char*)pInst->pValue );
}


//------------------------------------------------------------------------------
// <Create/Destroy>
//------------------------------------------------------------------------------
IusAlgorithmParameters * iusAlgorithmParametersCreate
(
    const char * const pId,
    const char * const pName,
    const char * const pDescription
)
{
    int i;
    IusAlgorithmParameters * pInst;

    if ( pId == NULL )
    {
        fprintf( stderr, "AlgorithmParameters: id is missing\n" );
        return NULL;
    }
    if ( pName == NULL )
    {
        fprintf( stderr, "AlgorithmParameters: name is missing\n" );
        return NULL;
    }
    if ( pDescription == NULL )
    {
        fprintf( stderr, "AlgorithmParameters: description is missing\n" );
        return NULL;
    }

    pInst = (IusAlgorithmParameters *)calloc( 1, sizeof(IusAlgorithmParameters) );
    if( pInst == NULL )
    {
        fprintf( stderr, "AlgorithmParameters: allocation failed - pInst\n" );
        return NULL;
    }

    pInst->pId = (char *)calloc( strlen(pId) + 1, sizeof(char) );
    if( pInst->pId == NULL )
    {
        fprintf( stderr, "AlgorithmParameters: allocation failed - id\n" );
        free( pInst );
        return NULL;
    }

    pInst->pName = (char *)calloc( strlen(pName) + 1, sizeof(char) );
    if ( pInst->pName == NULL )
    {
        fprintf( stderr, "AlgorithmParameters: allocation failed - name\n" );
        free( pInst->pId );
        free( pInst );
        return NULL;
    }

    pInst->pDescription = (char *)calloc( strlen(pDescription) + 1, sizeof(char) );
    if ( pInst->pDescription == NULL )
    {
        fprintf( stderr,
            "AlgorithmParameters: allocation failed - description\n" );
        free( pInst->pName );
        free( pInst->pId );
        free( pInst );
        return NULL;
    }

    strcpy( pInst->pId, pId );
    strcpy( pInst->pName, pName );
    strcpy( pInst->pDescription, pDescription );
    
    pInst->numberOfParameters = 0;
    for ( i = 0; i < MAX_ALGO_PARAMETERS; i++ )
    {
        pInst->pParameters[i] = NULL;
    }

    return pInst;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusAlgorithmParametersDestroy
(
    IusAlgorithmParameters * pInst
)
{
    int i;

    free( pInst->pId );
    free( pInst->pName );
    free( pInst->pDescription );

    for ( i = 0; i < pInst->numberOfParameters; i++ )
    {
        LF_DestroyAlgorithmParameter( pInst->pParameters[i] );
        pInst->pParameters[i] = NULL;
    }

    free( pInst );
}


//------------------------------------------------------------------------------
// <Add Values>
//------------------------------------------------------------------------------
void iusAlgorithmParametersAddInt
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName,
    int                            value
)
{
    LF_setInt( LF_AddNewParam( pInst, pName ), value );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusAlgorithmParametersAddFloat
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName,
    float                          value
)
{
    LF_setFloat( LF_AddNewParam( pInst, pName ), value );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusAlgorithmParametersAddString
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName,
    const char * const             pValue
)
{
    LF_setString( LF_AddNewParam( pInst, pName ), pValue );
}


//------------------------------------------------------------------------------
// <Read Values>
//------------------------------------------------------------------------------
int iusAlgorithmParametersReadInt
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName
)
{
    return LF_getInt( LF_FindParamByName( pInst, pName) );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float iusAlgorithmParametersReadFloat
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName
)
{
    return LF_getFloat( LF_FindParamByName( pInst, pName ) );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusAlgorithmParametersReadString
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName,
    char * const                         pBuffer
)
{
    LF_getString( LF_FindParamByName( pInst, pName ), pBuffer );
}


//------------------------------------------------------------------------------
// <Serialization>
//------------------------------------------------------------------------------
enum IusAlgorithmParameterType LF_DetermineValueType
(
    hid_t paramGroup
)
{
    enum IusAlgorithmParameterType result;
    hid_t                          valueDataset;
    hid_t                          dataType;
    H5T_class_t                    typeClass;
    //int typeSize;
    //sign_t typeSign;

    result = IUS_ALGOPARAMT_NONE;

    // Determining the type of 'value' dataset:
    valueDataset = H5Dopen( paramGroup, "value", H5P_DEFAULT );
    dataType     = H5Dget_type( valueDataset );
    typeClass    = H5Tget_class( dataType );
    //typeSize   = (int)H5T.get_size( dataType );
    //typeSign   = H5T.get_sign( dataType );
    
    // to correctly determine int32, we should check sign =! NONE and size == 4.
    if ( typeClass == H5T_INTEGER )
    {
        result = IUS_ALGOPARAMT_INT;
    }

    // if size == 4, float. if size == 8, double.
    if ( typeClass == H5T_FLOAT )
    {
        result = IUS_ALGOPARAMT_FLOAT;
    }
    
    if ( typeClass == H5T_STRING )
    {
        result = IUS_ALGOPARAMT_STRING;
    }

    H5Tclose( dataType );
    H5Dclose( valueDataset );

    return result;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_ReadParameter
(
    IusAlgorithmParameters * pInst,
    hid_t                    paramGroup,
    int verbose
)
{
    enum IusAlgorithmParameterType type;
    char *                         pName;
    int                            intValue;
    float                          floatValue;
    char *                         pStringValue;

    iusHdf5ReadString( paramGroup, "name", &pName, verbose );
    strcpy( pInst->pName, pName );

    type = LF_DetermineValueType( paramGroup );

    if ( type == IUS_ALGOPARAMT_NONE )
    {
        fprintf( stderr,
            "iusAlgorithmParameters Read: Failed to determine type of Hdf dataset for parameter value" );
    }

    if ( type == IUS_ALGOPARAMT_INT )
    {
        iusHdf5ReadInt( paramGroup, "value", &intValue, verbose );
        iusAlgorithmParametersAddInt( pInst, pName, intValue );
    }

    if ( type == IUS_ALGOPARAMT_FLOAT )
    {
        iusHdf5ReadFloat( paramGroup, "value", &floatValue, verbose );
        iusAlgorithmParametersAddFloat( pInst, pName, floatValue );
    }

    if ( type == IUS_ALGOPARAMT_STRING )
    {
        iusHdf5ReadString( paramGroup, "value", &pStringValue, verbose );
        iusAlgorithmParametersAddString( pInst, pName, pStringValue );
        free( pStringValue );
    }

    free( pName );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void LF_WriteParameter
(
    IusAlgorithmParameter * pInst,
    hid_t paramGroup
)
{
    int    intValue;
    float  floatValue;
    char * pStringValue;
    hsize_t dims[1]={1};

    H5LTmake_dataset_string (paramGroup, "name", pInst->pName );

    if ( pInst->type == IUS_ALGOPARAMT_INT )
    {
        intValue = LF_getInt( pInst );
        H5LTmake_dataset_int( paramGroup, "value", 1, dims, &intValue );
    }
    if ( pInst->type == IUS_ALGOPARAMT_FLOAT )
    {
        floatValue = LF_getFloat( pInst );
        H5LTmake_dataset_float( paramGroup, "value", 1, dims, &floatValue );
    }
    if ( pInst->type == IUS_ALGOPARAMT_STRING )
    {
        pStringValue = (char*)calloc( pInst->length, sizeof(char) );
        LF_getString( pInst, pStringValue );
        H5LTmake_dataset_string( paramGroup, "value", pStringValue );
        free( pStringValue );
    }
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusAlgorithmParameters * iusAlgorithmParametersRead
(
    hid_t handle,
    int verbose
)
{
    herr_t                   status;
    hid_t                    algoGroup;
    hid_t                    paramGroup;
    char *                   pId;
    char *                   pName;
    char *                   pDescription;
    char                     pParamName[20];
    IusAlgorithmParameters * pInst;
    int                      i;
    int                      numberOfParams;

    if ( handle < 1 )
    {
        fprintf(stderr,
            "iuAlgorithmParameters Read: Error hid_t handle is invalid\n");
        return NULL;
    }

    algoGroup = H5Gopen( handle, "AlgorithmParameters", H5P_DEFAULT );

    iusHdf5ReadString( algoGroup, "id", &pId, verbose );
    iusHdf5ReadString( algoGroup, "name", &pName, verbose );
    iusHdf5ReadString( algoGroup, "description", &pDescription, verbose );

    pInst = iusAlgorithmParametersCreate( pId, pName, pDescription );

    if ( pInst == NULL )
    {
        fprintf( stderr,
            "iusAlgorithmParameters Read: Failed to allocate pInst" );
        return NULL;
    }

    iusHdf5ReadInt( algoGroup, "numberOfParameters", &numberOfParams, verbose );

    for ( i = 0; i < numberOfParams; i++ )
    {
        sprintf( pParamName, "parameter%d", i );
        paramGroup = H5Gopen( algoGroup, pParamName, H5P_DEFAULT );

        LF_ReadParameter( pInst, paramGroup, verbose );

        H5Gclose( paramGroup );
    }

    H5Gclose( algoGroup );

    return pInst;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusAlgorithmParametersWrite
(
    const IusAlgorithmParameters * const pInst, 
    hid_t                                handle
)
{
    hid_t   algoGroup;
    hid_t   paramGroup;
    char    paramName[20];
    hsize_t dims[1] = {1};
    int     i;

    algoGroup = H5Gcreate( handle, "AlgorithmParameters", H5P_DEFAULT,
                           H5P_DEFAULT, H5P_DEFAULT );

    H5LTmake_dataset_string( algoGroup, "id", pInst->pId );
    H5LTmake_dataset_string( algoGroup, "name", pInst->pName );
    H5LTmake_dataset_string( algoGroup, "description", pInst->pDescription );

    H5LTmake_dataset_int( algoGroup, "numberOfParameters", 1, dims,
                          &(pInst->numberOfParameters) );

    for ( i = 0; i < pInst->numberOfParameters; i++ )
    {
        sprintf( paramName, "parameter%d", i );
        paramGroup = H5Gcreate( algoGroup, paramName, H5P_DEFAULT,
                                H5P_DEFAULT, H5P_DEFAULT );

        LF_WriteParameter( pInst->pParameters[i], paramGroup );

        H5Gclose( paramGroup );
    }

    H5Gclose( algoGroup );
}


