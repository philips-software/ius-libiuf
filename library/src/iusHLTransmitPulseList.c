//
// Created by Ruijzendaal on 05/04/2018.
//



#define IUSLIBRARY_IMPLEMENTATION
#include <stdarg.h>
#include <assert.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusInput.h>
#include <include/iusTypes.h>

#include <include/iusHLTransmitPulseList.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <include/iusHLParametricTransmitPulse.h>
#include <include/iusHLNonParametricTransmitPulseList.h>


// operations
//
//iutpl_t iusHLCreateTransmitPulseList
//(
//    int numTransmitPulses,
//    IusTransmitPulseType type,
//    ...
//)
//{
//    int index;
//    IusTransmitPulseList *list = NULL;
//    switch(type)
//    {
//        case IUS_PARAMETRIC_PULSETYPE:
//        {
//            list  = iusHLCreateParametricTransmitPulseList(numTransmitPulses);
//            break;
//        }
//        case IUS_NON_PARAMETRIC_PULSETYPE:
//        {
//            va_list ap;
//            va_start(ap, type);
//            int numPulseValues = va_arg(ap, int);
//            list = iusHLCreateNonParametricTransmitPulseList(numTransmitPulses,numPulseValues);
//            break;
//        }
//        default:
//        {
//            // New type?
//            free(list);
//            list = NULL;
//        }
//    }
//    return list;
//}
//

