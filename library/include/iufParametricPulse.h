#ifndef IUFLIBRARY_IUFHLPARAMETRICPULSE_H
#define IUFLIBRARY_IUFHLPARAMETRICPULSE_H
#include <iufTypes.h>

// ADT
typedef struct IufParametricPulse IufParametricPulse;
typedef IufParametricPulse *iupp_t;
#define  IUPP_INVALID (iupp_t) NULL

/** Create the parametric pulse that is described by a sinusoid with the three parameters: frequency, amplitude and 
 * number of periods
*
* /return the created pulse
*/
iupp_t iufParametricPulseCreate
(
    float pulseFrequency,				///< The frequency in Hz of the transmitted sinusoid
    float pulseAmplitude,				///< The Amplitude of the pulse in Volt 
    int numPulses                       ///< The number of periods the pulse.
);

/** Deletes a parametric pulse 
* 
*/
int iufParametricPulseDelete
(
    iupp_t pulse						///< The paramteric pulse to be deleted
);


/** Compares two parametric pulses. It doesn't matter which is the reference and the actual.
* /return 
*/
IUF_BOOL iufParametricPulseCompare
(
    iupp_t reference,					///< when comparing two parametric pulses: the reference  
    iupp_t actual                       ///< when comparing two parametric pulses: the pulse to compare with the reference
);

// Getters
float iufParametricPulseGetFrequency
(
    iupp_t pulse                        ///< Returns the frequency in Hz of this pararmetric pulse
);

float iufParametricPulseGetPulseAmplitude
(
    iupp_t pulse                        ///< Returns the amplitude in Hz of this pararmetric pulse
);

int iufParametricPulseGetNumPulses
(
    iupp_t transmitPulse                ///< Returns the number of periods that this pararmetric pulse has
);

#endif //IUFLIBRARY_IUFHLPARAMETRICPULSE_H
