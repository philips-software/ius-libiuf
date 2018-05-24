//
// Created by Ruijzendaal on 14/03/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSDUCER_H
#define IUSLIBRARY_IUSHLTRANSDUCER_H

#include <include/iusTypes.h>
#include <iusHLPosition.h>
#include <iusHLTransducerElement.h>


// ADT
struct IusTransducer;
typedef struct IusTransducer IusTransducer;

typedef IusTransducer *iut_t;
typedef Ius3DAngle *iu3da_t;
typedef Ius3DSize *iu3ds_t;
typedef Ius2DSize *iu2ds_t;
#define IUT_INVALID (iut_t) NULL


iut_t iusHLTransducerAlloc
(
  void
);

iut_t iusHLTransducerCreate
(
char *name,
IusTransducerShape shape,
float centerFrequency,
int numElements
);

int iusHLTransducerDelete
(
iut_t transducer
);

int iusHLTransducerSetElement
(
    IusTransducer *pTransducer,
    int elementIndex,
    void *transducerElement
);

iute_t iusHLTransducerGetElement
(
    IusTransducer
    *transducer,
    int elementIndex
);


IUS_BOOL iusHLTransducerCompare
(
iut_t reference,
iut_t actual
);

float iusHLTransducerGetCenterFrequency
(
    iut_t transducer
);

int iusHLTransducerGetNumElements
(
    iut_t transducer
);

char *iusHLTransducerGetName
(
    iut_t transducer
);

IusTransducerShape iusHLTransducerGetShape
(
    iut_t transducer
);



Ius2DSize *iusHL2DSizeCreate
(
float sx,
float sz
);


iu3da_t iusHL3DAngleCreate
(
float theta,
float phi
);

iu3ds_t iusHL3DSizeCreate
(
float sx,
float sy,
float sz
);

IusShape iusHLTransducerGetShapeType
(
    iut_t baseElement
);

#endif //IUSLIBRARY_IUSHLTRANSDUCER_H
