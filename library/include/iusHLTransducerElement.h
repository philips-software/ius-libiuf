
// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct IusTransducerElement IusTransducerElement;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct IusTransducerElement IusTransducerElement;
typedef IusTransducerElement *iute_t;
#define  IUTE_INVALID (iute_t) NULL

iute_t iusHLTransducerElementCreate
(
  IusShape shape
);

int iusHLTransducerElementDelete
(
  iute_t iusTransducerElement
);

// operations
int iusHLTransducerElementCompare
(
  iute_t reference,
  iute_t actual
);

// Getters
IusShape iusHLTransducerElementGetShape
(
  iute_t element
);


#endif //IUSLIBRARY_IUSHLTRANSDUCERELEMENT_H
