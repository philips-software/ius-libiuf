// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERADT_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERADT_H

#include <ius2DTransducerElementList.h>
#include <iusTransducerADT.h>

struct Ius2DTransducer
{
    struct IusTransducer      baseTransducer;
    iu2dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
};


#endif //IUSLIBRARY_IUSHL2DTRANSDUCERADT_H
