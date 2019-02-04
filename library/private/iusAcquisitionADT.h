#ifndef IUSLIBRARY_IUSHLACQUISITIONADT_H
#define IUSLIBRARY_IUSHLACQUISITIONADT_H

// ADT
/** \brief An Ultrasound acquisition is identified by a date and a description, also the speed of sound has been determined */
struct IusAcquisition
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * pDescription;    /**< Acquisition notes */
} ;

#endif //IUSLIBRARY_IUSHLACQUISITIONADT_H
