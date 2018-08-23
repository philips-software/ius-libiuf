
//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <include/iusNode.h>
#include <iusReceiveSettings.h>
#include <iusError.h>
#include <iusInputInstance.h>

// ADT
struct IusInputInstance
{
    iun_t              iusNode;
//    iue_t              pExperiment;      /**< experiment settings */
//    iut_t              pTransducer;      /**< transducer that has been used */
//    iurs_t             pReceiveSettings; /**< data receive settings */
//    iuds_t             pDrivingScheme;   /**< data transmit settings */

    int                numFrames;        /**< The number of frames in the data */
    int                IusVersion;       /**< version of input file format */
} ;


iuii_t iusInputInstanceCreate
(
    void
)
{
    iuii_t created = calloc(1,sizeof(IusInputInstance));
    if( created == IUII_INVALID ) return IUII_INVALID;
    created->iusNode = iusNodeCreate(IUS_INPUT_TYPE, 0);
    if( created->iusNode == IUN_INVALID )
    {
        free(created);
        return IUII_INVALID;
    }
    created->numFrames = IUS_DEFAULT_NUM_FRAMES;
    created->IusVersion = iusGetVersionMajor();
//    created->pReceiveSettings = NULL;
//    created->pDrivingScheme = NULL;
//    created->pTransducer = NULL;
//    created->pExperiment = NULL;
    return created;
}

int iusInputInstanceDelete
(
    iuii_t iusInputInstance
)
{
    int status = IUS_ERR_VALUE;
    if(iusInputInstance != NULL)
    {
        iusNodeDelete(iusInputInstance->iusNode);
        free(iusInputInstance);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusInputInstanceCompare
(
iuii_t reference,
iuii_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->numFrames != actual->numFrames ) return IUS_FALSE;
    if( reference->IusVersion != actual->IusVersion ) {
        return IUS_FALSE;
    }

    if(iuHLNodeCompare(reference->iusNode, actual->iusNode) == IUS_FALSE ){
        return IUS_FALSE;
    }

//    if(iusExperimentCompare(reference->pExperiment, actual->pExperiment) == IUS_FALSE ){
//        return IUS_FALSE;
//    }
//
//    if(iusTransducerCompare(reference->pTransducer, actual->pTransducer) == IUS_FALSE ){
//        return IUS_FALSE;
//    }
//
//    if(iusDrivingSchemeCompare(reference->pDrivingScheme, actual->pDrivingScheme) == IUS_FALSE ){
//        return IUS_FALSE;
//    }
//
//    if(iusReceiveSettingsCompare(reference->pReceiveSettings, actual->pReceiveSettings) == IUS_FALSE ){
//        return IUS_FALSE;
//    }
    return IUS_TRUE;
}



// Getters
int iusInputInstanceGetNumFrames
(
    iuii_t instance
)
{
    if( instance == NULL ) return -1;
    return instance->numFrames;
}

iun_t iusInputInstanceGetNode
(
    iuii_t instance
)
{
    if( instance == NULL ) return IUN_INVALID;
    return instance->iusNode;
}

//
//iue_t iusInputInstanceGetExperiment
//(
//    iuii_t instance
//)
//{
//    if( instance == NULL ) return IUE_INVALID;
//    return instance->pExperiment;
//}
//
//iut_t iusInputInstanceGetTransducer
//(
//    iuii_t instance
//)
//{
//    if( instance == NULL ) return IUT_INVALID;
//    return instance->pTransducer;
//}
//
//
//iuds_t iusInputInstanceGetDrivingScheme
//(
//    iuii_t instance
//)
//{
//    if( instance == NULL ) return IUDS_INVALID;
//    return instance->pDrivingScheme;
//}
//
//// setters
//int iusHeaderSetExperiment(iuii_t instance, iue_t experiment)
//{
//    if( instance == NULL ) return IUS_ERR_VALUE;
//    instance->pExperiment = experiment;
//    return IUS_E_OK;
//}
//
//int iusInputInstanceSetTransducer
//(
//    iuii_t instance,
//    iut_t transducer
//)
//{
//    if( instance == NULL ) return IUS_ERR_VALUE;
//    instance->pTransducer = transducer;
//    return IUS_E_OK;
//}
//
//int iusInputInstanceSetDrivingScheme
//(
//    iuii_t instance,
//    iuds_t scheme
//)
//{
//    if( instance == NULL ) return IUS_ERR_VALUE;
//    instance->pDrivingScheme = scheme;
//    return IUS_E_OK;
//}

int iusInputInstanceSetNode
(
    iuii_t instance,
    iun_t node
)
{
    if( instance == NULL ) return IUS_ERR_VALUE;
    instance->iusNode = node;
    return IUS_E_OK;
}