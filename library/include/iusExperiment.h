//
// Created by Ruijzendaal on 14/03/2018.
//
#ifndef IUSLIBRARY_IUSHLEXPERIMENT_H
#define IUSLIBRARY_IUSHLEXPERIMENT_H

// ADT
struct IusExperiment;
typedef  struct IusExperiment IusExperiment;
typedef  IusExperiment    * iue_t;
#define IUE_INVALID (iue_t) NULL

iue_t iusExperimentCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    const char *pDescription  /**< Experiment notes */
);

int iusExperimentDelete
(
    iue_t experiment
);

float iusExperimentGetSpeedOfSound
(
    iue_t experiment
);


int iusExperimentGetDate
(
    iue_t experiment
);

char *iusExperimentGetDescription
(
    iue_t experiment
);

IUS_BOOL iusExperimentCompare
(
    iue_t reference,
    iue_t actual
);

#endif //IUSLIBRARY_IUSHLEXPERIMENT_H
