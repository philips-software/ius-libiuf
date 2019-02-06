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

#endif // IUSLIBRARY_IUSHLPATTERNLISTADT_H
