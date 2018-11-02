//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICPULSE_H
#define IUSLIBRARY_IUSHLPARAMETRICPULSE_H

// ADT
typedef struct IusParametricPulse IusParametricPulse;
typedef IusParametricPulse *iupp_t;
#define  IUPP_INVALID (iupp_t) NULL

/** Create the parametric pulse that is described by a sinusoid with the three parameters: frequence, ampltidue and numper of periods
*
* /return the created pulse
*/
iupp_t iusParametricPulseCreate
(
    float pulseFrequency,				///< The frequency in Hz of the transmitted sinusoid
    float pulseAmplitude,				///< The Amplitude of the pulse in Volt 
    int pulseCount                      ///< The number of periods the pulse.
);

/** Deletes a parametric pulse 
* 
*/
int iusParametricPulseDelete
(
    iupp_t pulse						///< The paramteric pulse to be deleted
);


/** Compares two parametric pulses. It doesn't matter which is the reference and the actual.
* /return 
*/
IUS_BOOL iusParametricPulseCompare
(
    iupp_t reference,					///< when comparing two parametric pulses: the reference  
    iupp_t actual                       ///< when comparing two parametric pulses: the pulse to compare with the reference
);

// Getters
float iusParametricPulseGetFrequency
(
    iupp_t pulse                        ///< Returns the frequency in Hz of this pararmetric pulse
);

float iusParametricPulseGetPulseAmplitude
(
    iupp_t pulse                        ///< Returns the amplitude in Hz of this pararmetric pulse
);

int iusParametricPulseGetCount
(
    iupp_t transmitPulse                ///< Returns the number of periods that this pararmetric pulse has
);

#endif //IUSLIBRARY_IUSHLPARAMETRICPULSE_H
