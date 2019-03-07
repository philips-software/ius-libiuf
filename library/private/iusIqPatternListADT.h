#ifndef IUSLIBRARY_IUSIQPATTERNLISTADT_H
#define IUSLIBRARY_IUSIQPATTERNLISTADT_H

// ADT
struct IusIqPatternList
{
    int numPatterns;
    iuiqpa_t *pIqPatterns ;
    IUS_BOOL deepDelete;
    iudmd_t demodulationDict;
    iurcmd_t receiveChannelMapDict;

};

// SWIG binding
#ifdef SWIG
%extend IusIqPatternList {
        IusIqPatternList() {
            int numPatterns = 1;
            iudmd_t demodulationDict = NULL;
            iurcmd_t receiveChannelMapDict = NULL;
            return iusIqPatternListCreate(numPatterns, DemodulationDict, receiveChannelMapDict);
        }

        ~IusPatternList() {
            iusPatternListDelete($self);
        }

};
#endif

#endif // IUSLIBRARY_IUSIQPATTERNLISTADT_H
