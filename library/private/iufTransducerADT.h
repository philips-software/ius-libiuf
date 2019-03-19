//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUFLIBRARY_IUFHLTRANSDUCERADT_H
#define IUFLIBRARY_IUFHLTRANSDUCERADT_H

struct IufTransducer
{
    IufShape               type;			  /**< 2D or 3D transducer? */
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    IufTransducerShape     shape;
    IUF_BOOL               deepDelete;
} ;

// SWIG binding
#ifdef SWIG
%extend IufTransducer {
        ~IufTransducer() {
            iufTransducerDelete($self);
        }
}
#endif

#endif //IUFLIBRARY_IUFHLTRANSDUCERADT_H
