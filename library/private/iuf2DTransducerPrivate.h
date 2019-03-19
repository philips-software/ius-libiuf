// Created by nlv09165 on 17/08/2018.
#ifndef IUFLIBRARY_IUFHL2DTRANSDUCERIMP_H
#define IUFLIBRARY_IUFHL2DTRANSDUCERIMP_H

herr_t iuf2DTransducerSave
(
    iu2dt_t transducer,
    hid_t handle
);

iu2dt_t iuf2DTransducerLoad
(
    hid_t handle
);

IufTransducerShape iuf2DTransducerGetShape(
	iu2dt_t transducer
);

char *iuf2DTransducerGetName
(
	iu2dt_t transducer
);

float iuf2DTransducerGetCenterFrequency
(
	iu2dt_t transducer
);


#endif //IUFLIBRARY_IUFHL2DTRANSDUCERIMP_H
