#ifndef IUFLIBRARY_IUFHLRECEIVECHANNELMAP_H
#define IUFLIBRARY_IUFHLRECEIVECHANNELMAP_H

#include <iufTypes.h>

// ADT
typedef  struct IufReceiveChannelMap IufReceiveChannelMap;
/** The receive channel map is a list of with \p numChannels containing the indices of the transducer elements 
 * and a channel delay of each channel.
 * During an acquisition not all transducer elements might be digitized, i.e. due to bandwidth limitations and 
 * might not all be synchronous. If only numChannels are digitized in parallel, the receivechannelmap indicates 
 * which transducers elements are selected (and there order). 
 */
typedef  IufReceiveChannelMap    * iurcm_t;
#define  IURCM_INVALID (iurcm_t) NULL

/** \brief Create an empty #IufReceiveChannelMap with a size of \p numChannels.
 * \return Returns the created map or IURCM_INVALID in case of an error.
 */ 
iurcm_t  iufReceiveChannelMapCreate
(
	int numChannels
);

/** \brief Delete an #IufReceiveChannelMap
 * \return Returns #IUF_E_OK if successful or #IUF_ERR_VALUE otherwise
 */ 
int iufReceiveChannelMapDelete
(
	iurcm_t channelMap ///< the map of interest
);

/** \brief Compare two #IufReceiveChannelMap object with each other.
 * \return Returns #IUF_TRUE in case the two maps are equal and #IUF_FALSE otherwise
 */ 
IUF_BOOL  iufReceiveChannelMapCompare
(
	iurcm_t reference,  ///< The #IufReceiveChannelMap to compare to
	iurcm_t actual      ///< The #IufReceiveChannelMap to compare with
);

/** \brief Get the number of channels that the map has.
 * \return Returns the map size
 */
int iufReceiveChannelMapGetNumChannels
(
    iurcm_t channelMap ///< The #IufReceiveChannelMap of interest
);

/** \brief Get the index value (the transducer element number) at channelMap \p mapIdx.
 * \return Returns the index or -1 in case it was not found
 */
int iufReceiveChannelMapGetChannel
(
	iurcm_t channelMap, ///< The #IufReceiveChannelMap of interest
	int index                 ///< The channel number of the map.
);

/** \brief Same as iufReceiveChannelMapGetNumChannels
 * \return Returns the number of channels which is equal to the number of start delays.
 */
int iufReceiveChannelMapGetNumDelays
(
	iurcm_t channelMap ///< the #IufReceiveChannelMap of interest
);

/** \brief Gets the (digitization) delay of each channel.
 * \return Returns the delay value in seconds.
 */
float iufReceiveChannelMapGetStartDelay
(
	iurcm_t channelMap, ///< the #IufReceiveChannelMap of interest
    int index                  ///< the channel index
);

/** \brief Sets the transducer index for a channel. 
 * \return Returns #IUF_E_OK when succesful or -1 in case of an invalid argument.
 */
int iufReceiveChannelMapSetChannel
(
	iurcm_t channelMap,  ///< the #IufReceiveChannelMap of interest
	int index,                 ///< the channel index
	int transducerIdx           ///< the transducer index
);

/** \brief Sets a transducer index for all channels at once. 
 * \return Returns #IUF_E_OK when succesful or -1 in case of an invalid argument.
 */
int iufReceiveChannelMapSetMap
(
	iurcm_t channelMap,  ///< the #IufReceiveChannelMap of interest
	int *map                    ///< the array of transducer indices.
);

/** \brief Sets the (digitization) delay of a channel [seconds]. If not set the value is 0.0f
 * \return Returns #IUF_E_OK when succesful or -1 in case of an invalid argument.
 */
int iufReceiveChannelMapSetStartDelay
(
	iurcm_t channelMap, ///< the #IufReceiveChannelMap of interest
    int index,                 ///< the channel index
    float delay                ///< the channel delay in seconds
);


#endif //IUFLIBRARY_IUFHLRECEIVECHANNELMAP_H

