#ifndef IUFLIBRARY_IUFHLPATTERNLISTADT_H
#define IUFLIBRARY_IUFHLPATTERNLISTADT_H

// ADT
struct IufPatternList
{
    int numPatterns;
    iupa_t *   pPatterns ;
    IUF_BOOL deepDelete;
    iursd_t receiveSettingsDict;
    iurcmd_t receiveChannelMapDict;

};

// SWIG binding
#ifdef SWIG
%extend IufPatternList {
        IufPatternList() {
            int numPatterns = 1;
            iursd_t receiveSettingsDict = NULL;
            iurcmd_t receiveChannelMapDict = NULL;
            return iufPatternListCreate(numPatterns, receiveSettingsDict, receiveChannelMapDict);
        }

        ~IufPatternList() {
            iufPatternListDelete($self);
        }

};
#endif

#endif // IUFLIBRARY_IUFHLPATTERNLISTADT_H
