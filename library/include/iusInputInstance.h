//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLINPUT_H
#define IUSLIBRARY_IUSHLINPUT_H

#include <iusTypes.h>
#include <iusNode.h>
#include <iusExperiment.h>
#include <iusTransducer.h>

#define IUS_DEFAULT_NUM_FRAMES  1


// ADT
struct IusInputInstance;
typedef  struct IusInputInstance IusInputInstance;
typedef  IusInputInstance * iuii_t;
#define IUII_INVALID (iuii_t)NULL

iuii_t iusInputInstanceCreate
(
    void
);

int iusInputInstanceDelete
(
    iuii_t instance
);


// operations
int iusInputInstanceCompare
(
    iuii_t reference,
    iuii_t actual
);

// getters
int iusInputInstanceGetNumFrames
(
    iuii_t instance
);

iue_t iusInputInstanceGetExperiment
(
    iuii_t instance
);

iun_t iusInputInstanceGetNode
(
    iuii_t instance
);

iut_t iusInputInstanceGetTransducer
(
    iuii_t instance
);

iuds_t iusInputInstanceGetDrivingScheme
(
    iuii_t instance
);


int iusInputInstanceSetExperiment
(
    iuii_t instance,
    iue_t experiment
);

// setters
int iusInputInstanceSetTransducer
(
    iuii_t instance,
    iut_t transducer
);

int iusInputInstanceSetDrivingScheme
(
    iuii_t instance,
    iuds_t drivingScheme
);

int iusInputInstanceSetNode
(
    iuii_t instance,
    iun_t node
);

#endif //IUSLIBRARY_IUSHLINPUT_H
