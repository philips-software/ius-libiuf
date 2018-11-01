/** ==============================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ==============================================================================
 *
 * \file nodeConfig.h
 * \brief File containing nodeConfig declarations.
 *
 *  ==============================================================================
 */
#ifndef IUS_EXAMPLE_NODE_CONFIG_H
#define IUS_EXAMPLE_NODE_CONFIG_H

#include "ius.h"

int ncInputFileNodeConfig
(
    iuif_t inputFile,
    int numFrames,
    char *transducerName,
    char *label
);
#endif //IUS_EXAMPLE_NODE_CONFIG_H
