#ifndef IUFLIBRARY_IUFHLACQUISITIONADT_H
#define IUFLIBRARY_IUFHLACQUISITIONADT_H

// ADT
/** \brief An Ultrasound acquisition is identified by a date and a description, also the speed of sound has been determined */
struct IufAcquisition
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * description;    /**< Acquisition notes */
} ;

// SWIG binding
#ifdef SWIG
%extend IufAcquisition {             // Attach these functions to struct Vector
        IufAcquisition(float speedOfSound, int date, char *description) {
            return iufAcquisitionCreate(speedOfSound,date,description);
        }
        ~IufAcquisition() {
            iufAcquisitionDelete($self);
        }

        char *__str__() {
            static char temp[256];
            sprintf(temp, "IufAcquisition [%g, %d, %s]\n", $self->speedOfSound, $self->date, $self->description);
            return &temp[0];
        }
};
#endif

#endif //IUFLIBRARY_IUFHLACQUISITIONADT_H
