//
// Created by Ruijzendaal on 08/03/2018.
//
#include <library_config.h>

#define IUSLIBRARY_IMPLEMENTATION

#include "ius.h"
#include "iusNode.h"
#include <iusInputFile.h>
#include <iusInput.h>
#include <iusHLNode.h>
#include <iusHLInput.h>
#include <iusHLInputFile.h>
#include <include/iusInput.h>
#include <memory.h>


int iusHLGetNumFrames(iuh_t header)
{
    return header->numFrames;
}

iuh_t iusHLCreateInputHeader(void)
{
    IusExperiment      * pExperiment;      /**< experiment settings */
    IusTransducer      * pTransducer;      /**< transducer that has been used */
    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */
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

IUS_BOOL iusCompareNode(IusNode *pReferenceNode, IusNode *pActualNode)
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

int iusHLHeaderSetExperiment(iuh_t header, iue_t experiment)
{
    header->pExperiment = experiment;
    return IUS_E_OK;
}

IUS_BOOL iusHLCompareHeader(iuh_t referenceHeader, iuh_t actualHeader)
{
    if( referenceHeader->IusVersion != actualHeader->IusVersion ) {
        fprintf( stderr, "iusHLCompareHeader version match %s@%d\n", __FILE__, __LINE__);
        return IUS_FALSE;
    }
    if( referenceHeader->numFrames != actualHeader->numFrames ) {
        fprintf(stderr, "iusHLCompareHeader numFrames match %s@%d\n", __FILE__, __LINE__);
        return IUS_FALSE;
    }
    if( iusCompareNode(&referenceHeader->iusNode, &actualHeader->iusNode) == IUS_FALSE ){
        return IUS_FALSE;
    }
    // Todo: Add experiment, transducer, receivesettings, drivingscheme
    return IUS_TRUE;
}


int iusGetVersionMajor(void) {
    return IUS_VERSION_MAJOR;
}