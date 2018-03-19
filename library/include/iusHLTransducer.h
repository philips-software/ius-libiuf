//
// Created by Ruijzendaal on 14/03/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSDUCER_H
#define IUSLIBRARY_IUSHLTRANSDUCER_H

#include <include/iusTypes.h>


#ifndef IUSLIBRARY_IMPLEMENTATION

#define FORWARD_STRUCT_DECL(a)  struct a; typedef  struct a a;
struct IusTransducer;
typedef struct IusTransducer IusTransducer;
struct IusBaseTransducerElement;
typedef struct IusBaseTransducerElement IusBaseTransducerElement;
struct Ius2DTransducerElement;
typedef struct Ius2DTransducerElement Ius2DTransducerElement;
struct Ius3DTransducerElement;
typedef struct Ius3DTransducerElement Ius3DTransducerElement;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef IusTransducer *iut_t;
typedef Ius3DPosition *iu3dp_t;
typedef Ius2DPosition *iu2dp_t;
typedef Ius3DAngle *iu3da_t;
typedef Ius3DSize *iu3ds_t;
typedef Ius2DSize *iu2ds_t;
typedef Ius2DTransducerElement *iu2dte_t;
typedef Ius3DTransducerElement *iu3dte_t;
typedef IusBaseTransducerElement *iudte_t;
#define IUT_INVALID (iut_t) NULL
#define IUTE_INVALID (iudte_t) NULL

iut_t iusHLCreateTransducer
    (
        char *name,
        IusTransducerShape shape,
        float centerFrequency,
        int numElements
    );

int iusHLDeleteTransducer
    (
        iut_t transducer
    );

int iusTransducerSetElement
    (
        IusTransducer *transducer,
        int elementIndex,
        void *transducerElement
    );

iudte_t iusTransducerGetElement
(
    IusTransducer
    *transducer,
    int elementIndex
);

IUS_BOOL iusTransducerCompareElement
    (
        IusBaseTransducerElement *reference,
        IusBaseTransducerElement *actual
    );

IUS_BOOL iusCompareTransducer
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

IusTransducerShape iusHLTransducerShape
    (
        iut_t transducer
    );

Ius2DPosition *iusHLCreate2DElementPosition
    (
        float x,
        float z
    );

Ius2DSize *iusHLCreate2DElementSize
    (
        float sx,
        float sz
    );

Ius2DTransducerElement *iusHLCreate2DElement
    (
        iu2dp_t pos,
        float phi,
        iu2ds_t siz
    );

iu3dp_t iusHLCreate3DElementPosition
    (
        float x,
        float y,
        float z
    );

iu3da_t iusHLCreate3DElementAngle
    (
        float theta,
        float phi
    );

iu3ds_t iusHLCreate3DElementSize
    (
        float sx,
        float sy,
        float sz
    );

iu3dte_t iusHLCreate3DElement
    (
        iu3dp_t pos,
        iu3da_t ang,
        iu3ds_t siz
    );

IusTransducerElementType iusHLTransducerGetElementType
    (
        iudte_t baseElement
    );
#endif //IUSLIBRARY_IUSHLTRANSDUCER_H
