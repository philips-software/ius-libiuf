//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERADT_H
#define IUSLIBRARY_IUSHLTRANSDUCERADT_H

struct IusTransducer
{
    IusShape               type;			  /**< 2D or 3D transducer? */
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    IusTransducerShape     shape;
    IUS_BOOL               deepDelete;
} ;

// SWIG binding
#ifdef SWIG
%extend IusTransducer {
        ~IusTransducer() {
            iusTransducerDelete($self);
        }
}
#endif

#endif //IUSLIBRARY_IUSHLTRANSDUCERADT_H
