//
// Created by Ruijzendaal on 14/03/2018.
//

#ifndef IUSLIBRARY_IUSHLEXPERIMENT_H
#define IUSLIBRARY_IUSHLEXPERIMENT_H


#ifndef IUSLIBRARY_IMPLEMENTATION
struct IusExperiment;
typedef  struct IusExperiment IusExperiment;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusExperiment    * iue_t;
#define IUEX_INVALID (iue_t) NULL

iue_t iusHLCreateExperiment
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
