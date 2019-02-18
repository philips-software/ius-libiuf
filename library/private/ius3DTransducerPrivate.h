// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERIMP_H

#include <ius3DTransducer.h>

herr_t ius3DTransducerSave
(
    iu3dt_t transducer,
    hid_t handle
);

iu3dt_t ius3DTransducerLoad
(
    hid_t handle
);

IusTransducerShape ius3DTransducerGetShape(
	iu3dt_t transducer
);

char *ius3DTransducerGetName
(
	iu3dt_t transducer
);

float ius3DTransducerGetCenterFrequency
(
	iu3dt_t transducer
);


#endif //IUSLIBRARY_IUSHL3DTRANSDUCERIMP_H
