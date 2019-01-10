#ifndef IUS_UUID_H
#define IUS_UUID_H

/** \brief Generates a unique string representing a UUID code in the format like e.g. 1b4e28ba-2fa1-11d2-883f-0016d3cca123 
 * \return character array containing a UUID 
 * */
char *iusUuidCreate(void);

/** \brief Create a UUID code in the format like e.g. 1b4e28ba-2fa1-11d2-883f-0016d3cca123 and assign it to \p c.
 * Sufficient memory needs to b e allocated in advance for \p c
 */
void setIusUuidCreate
(
    char* c              /// The character array the will hold the UUID string.
);

#endif
