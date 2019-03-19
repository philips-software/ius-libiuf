// Created by nlv09165 on 17/08/2018.
#ifndef IUFLIBRARY_IUFHL3DTRANSDUCERIMP_H
#define IUFLIBRARY_IUFHL3DTRANSDUCERIMP_H

#include <iuf3DTransducer.h>

herr_t iuf3DTransducerSave
(
    iu3dt_t transducer,
    hid_t handle
);

iu3dt_t iuf3DTransducerLoad
(
    hid_t handle
);

IufTransducerShape iuf3DTransducerGetShape(
	iu3dt_t transducer
);

char *iuf3DTransducerGetName
(
	iu3dt_t transducer
);

float iuf3DTransducerGetCenterFrequency
(
	iu3dt_t transducer
);


#endif //IUFLIBRARY_IUFHL3DTRANSDUCERIMP_H
