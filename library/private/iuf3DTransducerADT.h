#ifndef IUFLIBRARY_IUFHL3DTRANSDUCERADT_H
#define IUFLIBRARY_IUFHL3DTRANSDUCERADT_H

struct Iuf3DTransducer
{
    struct IufTransducer     baseTransducer;
    iu3dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT
#endif //IUFLIBRARY_IUFHL3DTRANSDUCERADT_H
