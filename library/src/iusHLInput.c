//
// Created by Ruijzendaal on 08/03/2018.
//
#include <library_config.h>
#include "library.h"

#define IUSLIBRARY_IMPLEMENTATION
#include "ius.h"
#include "iusNode.h"
#include <iusInputFile.h>
#include <iusInput.h>
#include <iusHLNode.h>
#include <iusHLInput.h>
#include <iusHLInputFile.h>


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



IUS_BOOL iusCompareHeader(iuh_t reference_header,iuh_t actual_header)
{
    return !IUS_TRUE;
}

int iusGetVersionMajor(void) {
    return IUS_VERSION_MAJOR;
}