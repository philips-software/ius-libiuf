#ifndef IUSLIBRARY_IUSHLACQUISITIONADT_H
#define IUSLIBRARY_IUSHLACQUISITIONADT_H

// ADT
/** \brief An Ultrasound acquisition is identified by a date and a description, also the speed of sound has been determined */
struct IusAcquisition
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * description;    /**< Acquisition notes */
} ;

// SWIG binding
#ifdef SWIG
%extend IusAcquisition {             // Attach these functions to struct Vector
        IusAcquisition(float speedOfSound, int date, char *description) {
            return iusAcquisitionCreate(speedOfSound,date,description);
        }
        ~IusAcquisition() {
            iusAcquisitionDelete($self);
        }

        char *__str__() {
            static char temp[256];
            sprintf(temp, "IusAcquisition [%g, %d, %s]\n", $self->speedOfSound, $self->date, $self->description);
            return &temp[0];
        }
};
#endif

#endif //IUSLIBRARY_IUSHLACQUISITIONADT_H
