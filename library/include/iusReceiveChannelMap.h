//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H

#include <iusTypes.h>

// ADT
struct IusReceiveChannelMap;
typedef  struct IusReceiveChannelMap IusReceiveChannelMap;
/** The receive channel map is a list of with \p numChannels containing the indices of the transducer elements 
 * and a channel delay of each channel.
 * During an acquisition not all transducer elements might be digitized, i.e. due to bandwidth limitations and 
 * might not all be synchronous. If only numChannels are digitized in parallel, the receivechannelmap indicates 
 * which transducers elements are selected (and there order). 
 */
typedef  IusReceiveChannelMap    * iurcm_t;
#define  IURCM_INVALID (iurcm_t) NULL

/** \brief Create an empty #IusReceiveChannelMap with a size of \p numChannels.
 * \return Returns the created map or IURCM_INVALID in case of an error.
 */ 
iurcm_t  iusReceiveChannelMapCreate
(
	int numChannels
);

/** \brief Delete an #IusReceiveChannelMap 
 * \return Returns #IUS_E_OK if successful or #IUS_ERR_VALUE otherwise  
 */ 
int iusReceiveChannelMapDelete
(
	iurcm_t channelMap ///< the map of interest
);

/** \brief Compare two #IusReceiveChannelMap object with each other.
 * \return Returns #IUS_TRUE in case the two maps are equal and #IUS_FALSE otherwise
 */ 
IUS_BOOL  iusReceiveChannelMapCompare
(
	iurcm_t reference,  ///< The #IusReceiveChannelMap to compare to     
	iurcm_t actual      ///< The #IusReceiveChannelMap to compare with
);

/** \brief Get the number of channels that the map has.
 * \return Returns the map size
 */
int iusReceiveChannelMapGetNumChannels
(
    iurcm_t channelMap ///< The #IusReceiveChannelMap of interest
);

/** \brief Get the index value (the transducer element number) at channelMap \p mapIdx.
 * \return Returns the index or -1 in case it was not found
 */
int iusReceiveChannelMapGetChannel
(
	iurcm_t channelMap, ///< The #IusReceiveChannelMap of interest
	int index                 ///< The channel number of the map.
);

/** \brief Same as iusReceiveChannelMapGetNumChannels
 * \return Returns the number of channels which is equal to the number of start delays.
 */
int iusReceiveChannelMapGetNumDelays
(
	iurcm_t channelMap ///< the #IusReceiveChannelMap of interest
);

/** \brief Gets the (digitization) delay of each channel.
 * \return Returns the delay value in seconds.
 */
float iusReceiveChannelMapGetStartDelay
(
	iurcm_t channelMap, ///< the #IusReceiveChannelMap of interest
    int index                  ///< the channel index
);

/** \brief Sets the transducer index for a channel. 
 * \return Returns #IUS_E_OK when succesful or -1 in case of an invalid argument.
 */
int iusReceiveChannelMapSetChannel
(
	iurcm_t channelMap,  ///< the #IusReceiveChannelMap of interest
	int index,                 ///< the channel index
	int transducerIdx           ///< the transducer index
);

/** \brief Sets a transducer index for all channels at once. 
 * \return Returns #IUS_E_OK when succesful or -1 in case of an invalid argument.
 */
int iusReceiveChannelMapSetMap
(
	iurcm_t channelMap,  ///< the #IusReceiveChannelMap of interest
	int *map                    ///< the array of transducer indices.
);

/** \brief Sets the (digitization) delay of a channel [seconds]. If not set the value is 0.0f
 * \return Returns #IUS_E_OK when succesful or -1 in case of an invalid argument.
 */
int iusReceiveChannelMapSetStartDelay
(
	iurcm_t ch, ///< the #IusReceiveChannelMap of interest
    int index,                 ///< the channel index
    float delay                ///< the channel delay in seconds
);


#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H

