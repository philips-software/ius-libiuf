//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLINPUT_H
#define IUSLIBRARY_IUSHLINPUT_H

#include <iusTypes.h>
#include <iusHLExperiment.h>
#include <iusHLTransducer.h>
#include <iusHLDrivingScheme.h>
#include <iusHLNode.h>

#define IUS_DEFAULT_NUM_FRAMES  1


// ADT
struct IusInputInstance;
typedef  struct IusInputInstance IusInputInstance;
typedef  IusInputInstance * iuii_t;
#define IUII_INVALID (iuii_t)NULL

iuii_t iusHLInputInstanceCreate
(
void
);

int iusHLInputInstanceDelete
(
iuii_t instance
);


// operations
int iusHLInputInstanceCompare
(
iuii_t reference,
iuii_t actual
);

// getters
int iusHLInputInstanceGetNumFrames
(
    iuii_t instance
);

iue_t iusHLInputInstanceGetExperiment
(
    iuii_t instance
);

iun_t iusHLInputInstanceGetNode
(
    iuii_t instance
);

iut_t iusHLInputInstanceGetTransducer
(
    iuii_t instance
);

iuds_t iusHLInputInstanceGetDrivingScheme
(
    iuii_t instance
);


int iusHLInputInstanceSetExperiment
(
    iuii_t instance,
    iue_t experiment
);

// setters
int iusHLInputInstanceSetTransducer
(
    iuii_t instance,
    iut_t transducer
);

int iusHLInputInstanceSetDrivingScheme
(
    iuii_t instance,
    iuds_t drivingScheme
);

int iusHLInputInstanceSetNode
(
    iuii_t instance,
    iun_t node
);

#endif //IUSLIBRARY_IUSHLINPUT_H
