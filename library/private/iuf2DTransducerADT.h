#ifndef IUFLIBRARY_IUFHL2DTRANSDUCERADT_H
#define IUFLIBRARY_IUFHL2DTRANSDUCERADT_H

#include <iuf2DTransducerElementList.h>
#include <iufTransducerADT.h>

struct Iuf2DTransducer
{
    struct IufTransducer      baseTransducer;
    iu2dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
};

// SWIG binding
#ifdef SWIG
%extend Iuf2DTransducer {
        Iuf2DTransducer
        (
            char *name,
            IufTransducerShape shape,
            float centerFrequency,
            int numElements
        )
        {
            return iuf2DTransducerCreate(name, shape, centerFrequency, numElements);
        }

        ~Iuf2DTransducer() {
            iuf2DTransducerDelete($self);
        }
}
#endif

#endif //IUFLIBRARY_IUFHL2DTRANSDUCERADT_H
