// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H

#include <ius3DAngle.h>
#include <ius3DSize.h>
#include <iusPosition.h>

// ADT
typedef struct Ius3DTransducerElement Ius3DTransducerElement;
typedef Ius3DTransducerElement *iu3dte_t;
#define  IU3DTE_INVALID (iu3dte_t) NULL

iu3dte_t ius3DTransducerElementCreate
(
    iu3dp_t pos,
    iu3da_t ang,
    iu3ds_t siz
);

//
//int ius3DTransducerElementDeepDelete
//(
//	iu3dte_t ius3DTransducerElement
//);

int ius3DTransducerElementDelete
(
    iu3dte_t ius3DTransducerElement
);


// operations
int ius3DTransducerElementCompare
(
    iu3dte_t reference,
    iu3dte_t actual
);

// Getters
iu3dp_t ius3DTransducerElementGetPosition
(
	iu3dte_t ius3DTransducerElement
);

iu3da_t ius3DTransducerElementGetAngle
(
	iu3dte_t ius3DTransducerElement
);

iu3ds_t ius3DTransducerElementGetSize
(
	iu3dte_t ius3DTransducerElement
);
#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H
