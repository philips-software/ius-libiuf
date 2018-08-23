// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H

// ADT
typedef struct IusTransducerElement IusTransducerElement;
typedef IusTransducerElement *iute_t;
#define  IUTE_INVALID (iute_t) NULL

iute_t iusTransducerElementCreate
(
    IusShape shape
);

int iusTransducerElementDelete
(
    iute_t iusTransducerElement
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
