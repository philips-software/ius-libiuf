#ifndef IUSLIBRARY_IUSHLPATTERNLISTADT_H
#define IUSLIBRARY_IUSHLPATTERNLISTADT_H

// ADT
struct IusPatternList
{
    int numPatterns;
    iupa_t *   pPatterns ;
    IUS_BOOL deepDelete;
    iursd_t receiveSettingsDict;
    iurcmd_t receiveChannelMapDict;

};

// SWIG binding
#ifdef SWIG
%extend IusPatternList {
        IusPatternList() {
            int numPatterns = 1;
            iursd_t receiveSettingsDict = NULL;
            iurcmd_t receiveChannelMapDict = NULL;
            return iusPatternListCreate(numPatterns, receiveSettingsDict, receiveChannelMapDict);
        }

        ~IusPatternList() {
            iusPatternListDelete($self);
        }

};
#endif

#endif // IUSLIBRARY_IUSHLPATTERNLISTADT_H
