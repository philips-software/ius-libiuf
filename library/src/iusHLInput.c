//
// Created by Ruijzendaal on 08/03/2018.
//
#include <library_config.h>


#define IUSLIBRARY_IMPLEMENTATION
#include <include/iusInput.h>
#include <include/iusHLNode.h>
#include <include/iusHLExperiment.h>
#include <include/iusHLInput.h>
#include <include/ius.h>


//#include <include/iusHLInput.h>


//#include "ius.h"
//#include "iusNode.h"
//#include <iusInputFile.h>
//#include <iusInput.h>
//#include <iusHLNode.h>
//#include <iusHLInput.h>
//#include <iusHLInputFile.h>
//#include <include/iusHLDrivingScheme.h>
//#include <memory.h>
//#include <stdlib.h>
//#include <include/iusInput.h>


int iusHLGetNumFrames(IusInputInstance *header)
{
    return header->numFrames;
}

IusInputInstance * iusHLCreateInputHeader(void)
{
//    IusExperiment      * pExperiment;      /**< experiment settings */
//    IusTransducer      * pTransducer;      /**< transducer that has been used */
//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */
//
//    IusNode              iusNode;
    /*
    * Properties that fully describe CWC data
    */
//    IusExperiment      * pExperiment;      /**< experiment settings */
//    IusTransducer      * pTransducer;      /**< transducer that has been used */
//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */
//
//    int                  numFrames;        /**< The number of frames in the data */
//
//    int                  IusVersion;       /**< version of input file format */
//    iusCreateExperiment();
//    iusCreateNode();
//    ius
//    iun_t node = iusHLCreateNode();
//    iuh_t pInstance = (iuh_t) iusInputCreate();
    // Create InputType Header, no parents
    IusNode *hSimpleNode = iusHLCreateNode( IUS_INPUT_TYPE, 0 );
    IusInputInstance *pInstance =  iusInputCreate( hSimpleNode, IUS_DEFAULT_NUM_FRAMES );
    return pInstance;

}

int iusHLHeaderSetExperiment(IusInputInstance * header, IusExperiment *experiment)
{
    header->pExperiment = experiment;
    return IUS_E_OK;
}

IusExperiment * iusHLHeaderGetExperiment
(
IusInputInstance * pInstance
)
{
    return pInstance->pExperiment;
}

int iusHLHeaderSetTransducer
(
IusInputInstance * header,
IusTransducer* transducer
)
{
    header->pTransducer = transducer;
    return IUS_E_OK;
}

IusTransducer* iusHLHeaderGetTransducer
(
IusInputInstance * pInstance
)
{
    return pInstance->pTransducer;
}

int iusHLHeaderSetDrivingScheme
(
IusInputInstance * header,
IusDrivingScheme* scheme
)
{
    header->pDrivingScheme = scheme;
    return IUS_E_OK;
}

IusDrivingScheme* iusHLHeaderGetDrivingScheme
    (
    IusInputInstance * pInstance
    )
{
    return pInstance->pDrivingScheme;
}


IUS_BOOL iusHLCompareHeader(IusInputInstance * referenceHeader, IusInputInstance * actualHeader)
{
    if( referenceHeader->IusVersion != actualHeader->IusVersion ) {
        fprintf( stderr, "iusHLCompareHeader version match %s@%d\n", __FILE__, __LINE__);
        return IUS_FALSE;
    }
    if( referenceHeader->numFrames != actualHeader->numFrames ) {
        fprintf(stderr, "iusHLCompareHeader numFrames match %s@%d\n", __FILE__, __LINE__);
        return IUS_FALSE;
    }
    if(iuHLNodeCompare(&referenceHeader->iusNode, &actualHeader->iusNode) == IUS_FALSE ){
        return IUS_FALSE;
    }

    if(iusHLExperimentCompare(referenceHeader->pExperiment, actualHeader->pExperiment) == IUS_FALSE ){
        return IUS_FALSE;
    }

    if(iusHLTransducerCompare(referenceHeader->pTransducer, actualHeader->pTransducer) == IUS_FALSE ){
        return IUS_FALSE;
    }

    if(iusHLCompareDrivingScheme(referenceHeader->pDrivingScheme, actualHeader->pDrivingScheme) == IUS_FALSE ){
        return IUS_FALSE;
    }

    // Todo: Add  receivesettings
    return IUS_TRUE;
}


int iusGetVersionMajor(void) {
    return IUS_VERSION_MAJOR;
}