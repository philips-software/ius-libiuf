//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERFACTORY__H
#define IUSLIBRARY_IUSHLTRANSDUCERFACTORY__H

#include <iusTransducer.h>

iut_t iusTransducerFactoryCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
);

#endif // IUSLIBRARY_IUSHLTRANSDUCERFACTORY__H