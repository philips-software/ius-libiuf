#ifndef IUFLIBRARY_IUFHLTRANSDUCERELEMENT_H
#define IUFLIBRARY_IUFHLTRANSDUCERELEMENT_H

// ADT
typedef struct IufTransducerElement IufTransducerElement;
/** Abstract type of a transducer's element. The shape of the transducer #IufShape defines if it is 2D or 3D transducer.
 */
typedef IufTransducerElement *iute_t;
#define  IUTE_INVALID (iute_t) NULL

/* Create a single transducer element of a certain shape */
iute_t iufTransducerElementCreate
(
    IufShape shape
);

int iufTransducerElementDelete
(
    iute_t transducerElement
);

// operations
int iufTransducerElementCompare
(
    iute_t reference,
    iute_t actual
);

// Getters
IufShape iufTransducerElementGetShape
(
    iute_t element
);

#endif //IUFLIBRARY_IUFHLTRANSDUCERELEMENT_H
