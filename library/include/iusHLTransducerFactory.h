//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERFACTORY__H
#define IUSLIBRARY_IUSHLTRANSDUCERFACTORY__H

#include <iusHLTransducer.h>

iut_t iusHLTransducerFactoryCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
);

#endif // IUSLIBRARY_IUSHLTRANSDUCERFACTORY__H