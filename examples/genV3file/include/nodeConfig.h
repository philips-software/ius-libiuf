/** ==============================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ==============================================================================
 *
 * \file nodeConfig.h
 * \brief File containing nodeConfig declarations.
 *
 *  ==============================================================================
 */
#ifndef IUF_EXAMPLE_NODE_CONFIG_H
#define IUF_EXAMPLE_NODE_CONFIG_H

#include "iuf.h"

int ncInputFileNodeConfig
(
    iuif_t inputFile,
    int numFrames,
    char *transducerName,
    char *label
);
#endif //IUF_EXAMPLE_NODE_CONFIG_H
