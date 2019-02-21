// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERADT_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERADT_H

struct Ius3DTransducer
{
    struct IusTransducer     baseTransducer;
    iu3dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT
#endif //IUSLIBRARY_IUSHL3DTRANSDUCERADT_H
