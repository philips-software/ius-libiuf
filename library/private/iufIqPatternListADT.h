#ifndef IUFLIBRARY_IUFIQPATTERNLISTADT_H
#define IUFLIBRARY_IUFIQPATTERNLISTADT_H

// ADT
struct IufIqPatternList
{
    int numPatterns;
    iuiqpa_t *pIqPatterns ;
    IUF_BOOL deepDelete;
    iudmd_t demodulationDict;
    iurcmd_t receiveChannelMapDict;

};

// SWIG binding
#ifdef SWIG
%extend IufIqPatternList {
        IufIqPatternList() {
            int numPatterns = 1;
            iudmd_t demodulationDict = NULL;
            iurcmd_t receiveChannelMapDict = NULL;
            return iufIqPatternListCreate(numPatterns, demodulationDict, receiveChannelMapDict);
        }

        ~IufIqPatternList() {
            iufIqPatternListDelete($self);
        }

};
#endif

#endif // IUFLIBRARY_IUFIQPATTERNLISTADT_H
