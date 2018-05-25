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

iue_t iusHLExperimentCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    char *pDescription  /**< Experiment notes */
);

int iusHLDeleteExperiment
(
    iue_t experiment
);

float iusHLExperimentGetSpeedOfSound
(
    iue_t experiment
);


int iusHLExperimentGetDate
(
    iue_t experiment
);

char *iusHLExperimentGetDescription
(
    iue_t experiment
);

IUS_BOOL iusHLExperimentCompare
(
    iue_t reference,
    iue_t actual
);

#endif //IUSLIBRARY_IUSHLEXPERIMENT_H
