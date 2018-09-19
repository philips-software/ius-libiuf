// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHL3DANGLE_H
#define IUSLIBRARY_IUSHL3DANGLE_H

// ADT
typedef struct Ius3DAngle
{
	float theta; /**< rotation over z axis */
	float phi;   /**< rotation over x axis */
} Ius3DAngle;

typedef Ius3DAngle *iu3da_t;
#define  IU3DA_INVALID (iu3da_t) NULL

iu3da_t ius3DAngleCreate
(
    float theta,
    float phi
);

int ius3DAngleDelete
(
    iu3da_t ius3DAngle
);


// operations
int ius3DAngleCompare
(
    iu3da_t reference,
    iu3da_t actual
);

#endif //IUSLIBRARY_IUSHL3DANGLE_H