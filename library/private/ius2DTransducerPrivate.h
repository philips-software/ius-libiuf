// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H

#include <iusTransducerPrivate.h>
#include <ius2DTransducer.h>
#include <ius2DTransducerElementList.h>

struct _Ius2DTransducer
{
    struct _IusTransducer     baseTransducer;
    iu2dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
};

herr_t ius2DTransducerSave
(
    iu2dt_t transducer,
    hid_t handle
);

iu2dt_t ius2DTransducerLoad
(
    hid_t handle
);

IusTransducerShape ius2DTransducerGetShape(
	iu2dt_t transducer
);

char *ius2DTransducerGetName
(
	iu2dt_t transducer
);

float ius2DTransducerGetCenterFrequency
(
	iu2dt_t transducer
);



#endif //IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H
