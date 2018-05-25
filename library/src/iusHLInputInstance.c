
//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <include/iusHLNode.h>
#include <iusHLReceiveSettings.h>
#include <iusError.h>
#include <iusHLInputInstance.h>

// ADT
struct IusInputInstance
{
    iun_t              iusNode;
    iue_t              pExperiment;      /**< experiment settings */
    iut_t              pTransducer;      /**< transducer that has been used */
    iurs_t             pReceiveSettings; /**< data receive settings */
    iuds_t             pDrivingScheme;   /**< data transmit settings */

    int                numFrames;        /**< The number of frames in the data */
    int                IusVersion;       /**< version of input file format */
} ;


iuii_t iusHLInputInstanceCreate
(
void
)
{
    iuii_t created = calloc(1,sizeof(IusInputInstance));
    if( created == IUII_INVALID ) return IUII_INVALID;
    created->iusNode = iusHLNodeCreate(IUS_INPUT_TYPE, 0);
    if( created->iusNode == IUN_INVALID )
    {
        free(created);
        return IUII_INVALID;
    }
    created->numFrames = IUS_DEFAULT_NUM_FRAMES;
    created->IusVersion = iusGetVersionMajor();
    created->pReceiveSettings = NULL;
    created->pDrivingScheme = NULL;
    created->pTransducer = NULL;
    created->pExperiment = NULL;
    return created;
}

int iusHLInputInstanceDelete
(
iuii_t iusInputInstance
)
{
    int status = IUS_ERR_VALUE;
    if(iusInputInstance != NULL)
    {
        iusHLNodeDelete(iusInputInstance->iusNode);
        free(iusInputInstance);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLInputInstanceCompare
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

    if(iusHLExperimentCompare(reference->pExperiment, actual->pExperiment) == IUS_FALSE ){
        return IUS_FALSE;
    }

    if(iusHLTransducerCompare(reference->pTransducer, actual->pTransducer) == IUS_FALSE ){
        return IUS_FALSE;
    }

    if(iusHLDrivingSchemeCompare(reference->pDrivingScheme, actual->pDrivingScheme) == IUS_FALSE ){
        return IUS_FALSE;
    }

    if(iusHLReceiveSettingsCompare(reference->pReceiveSettings, actual->pReceiveSettings) == IUS_FALSE ){
        return IUS_FALSE;
    }
    return IUS_TRUE;
}



// Getters
int iusHLInputInstanceGetNumFrames
(
    iuii_t instance
)
{
    if( instance == NULL ) return -1;
    return instance->numFrames;
}

iun_t iusHLInputInstanceGetNode
(
    iuii_t instance
)
{
    if( instance == NULL ) return IUN_INVALID;
    return instance->iusNode;
}


iue_t iusHLInputInstanceGetExperiment
(
    iuii_t instance
)
{
    if( instance == NULL ) return IUE_INVALID;
    return instance->pExperiment;
}

iut_t iusHLInputInstanceGetTransducer
(
    iuii_t instance
)
{
    if( instance == NULL ) return IUT_INVALID;
    return instance->pTransducer;
}


iuds_t iusHLInputInstanceGetDrivingScheme
(
    iuii_t instance
)
{
    if( instance == NULL ) return IUDS_INVALID;
    return instance->pDrivingScheme;
}

// setters
int iusHLHeaderSetExperiment(iuii_t instance, iue_t experiment)
{
    if( instance == NULL ) return IUS_ERR_VALUE;
    instance->pExperiment = experiment;
    return IUS_E_OK;
}

int iusHLInputInstanceSetTransducer
(
    iuii_t instance,
    iut_t transducer
)
{
    if( instance == NULL ) return IUS_ERR_VALUE;
    instance->pTransducer = transducer;
    return IUS_E_OK;
}

int iusHLInputInstanceSetDrivingScheme
(
    iuii_t instance,
    iuds_t scheme
)
{
    if( instance == NULL ) return IUS_ERR_VALUE;
    instance->pDrivingScheme = scheme;
    return IUS_E_OK;
}

int iusHLInputInstanceSetNode
(
    iuii_t instance,
    iun_t node
)
{
    if( instance == NULL ) return IUS_ERR_VALUE;
    instance->iusNode = node;
    return IUS_E_OK;
}