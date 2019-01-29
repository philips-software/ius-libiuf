// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H

// ADT
typedef struct IusTransducerElement IusTransducerElement;
/** Abstract type of a transducer's element. The shape of the transducer #IusShape defines if it is 2D or 3D transducer.
 */
typedef IusTransducerElement *iute_t;
#define  IUTE_INVALID (iute_t) NULL

/* Create a single transducer element of a certain shape */
iute_t iusTransducerElementCreate
(
    IusShape shape
);

int iusTransducerElementDelete
(
    iute_t transducerElement
);

// operations
int iusTransducerElementCompare
(
    iute_t reference,
    iute_t actual
);

// Getters
IusShape iusTransducerElementGetShape
(
    iute_t element
);

#endif //IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H
