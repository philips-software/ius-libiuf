//
// Created by Ruijzendaal on 14/03/2018.
//
#ifndef IUSLIBRARY_IUSHLACQUISITION_H
#define IUSLIBRARY_IUSHLACQUISITION_H

#include <iusTypes.h>

// ADT
struct IusAcquisition;
typedef  struct IusAcquisition IusAcquisition;
typedef  IusAcquisition    * iua_t;
#define IUE_INVALID (iua_t) NULL

iua_t iusAcquisitionCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    const char *pDescription  /**< Acquisition notes */
);

int iusAcquisitionDelete
(
    iua_t acquisition
);

float iusAcquisitionGetSpeedOfSound
(
    iua_t acquisition
);


int iusAcquisitionGetDate
(
    iua_t acquisition
);

char *iusAcquisitionGetDescription
(
    iua_t acquisition
);

IUS_BOOL iusAcquisitionCompare
(
    iua_t reference,
    iua_t actual
);

#endif //IUSLIBRARY_IUSHLACQUISITION_H
