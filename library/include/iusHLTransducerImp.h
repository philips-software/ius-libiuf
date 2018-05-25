//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHLTRANSDUCERIMP_H

struct IusTransducer
{
    IusShape               type; /**< 2D or 3D transducer? */
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    IusTransducerShape     shape;
} ;

#endif //IUSLIBRARY_IUSHLTRANSDUCERIMP_H
