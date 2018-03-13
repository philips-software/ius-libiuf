
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(HAVE_INTTYPES_H)
#include <inttypes.h>
#endif

/* set the following to the number of 100ns ticks of the actual
resolution of your system's clock */
#define UUIDS_PER_TICK 1024

#ifdef _WIN32
#include <Windows.h>
#define snprintf _snprintf

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#if HAVE_SYS_SYSINFO_H
#include <sys/sysinfo.h>
#endif

#define I64(C) C

typedef uint64_t uuid_time_t;

typedef struct {
    char nodeID[6];
} uuid_node_t;

typedef struct {
    uint32_t  time_low;
    uint16_t  time_mid;
    uint16_t  time_hi_and_version;
    uint8_t   clock_seq_hi_and_reserved;
    uint8_t   clock_seq_low;
    uint8_t   node[6];
} netperf_uuid_t;

static void get_system_time(uuid_time_t *uuid_time)
{
    ULARGE_INTEGER time;

    /* NT keeps time in FILETIME format which is 100ns ticks since
    Jan 1, 1601. UUIDs use time in 100ns ticks since Oct 15, 1582.
    The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
    + 18 years and 5 leap days. */
    GetSystemTimeAsFileTime((FILETIME *)&time);
    time.QuadPart +=

        (unsigned __int64)(1000 * 1000 * 10)       // seconds
        * (unsigned __int64)(60 * 60 * 24)       // days
        * (unsigned __int64)(17 + 30 + 31 + 365 * 18 + 5); // # of days
    *uuid_time = time.QuadPart;
}

/* true_random -- generate a crypto-quality random number.
**This sample doesn't do that.** */
static uint16_t true_random(void)
{
    static int inited = 0;
    uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_now);
        time_now = time_now / UUIDS_PER_TICK;
        srand((unsigned int)
            (((time_now >> 32) ^ time_now) & 0xffffffff));
        inited = 1;
    }

    return (uint16_t)rand();
}

/* Sample code, not for use in production; see RFC 1750 */
static void get_random_info(char seed[16])
{
    uint16_t myrand;
    int i;

    i = 0;
    do {
        myrand = true_random();
        seed[i++] = myrand & 0xff;
        seed[i++] = myrand >> 8;
    } while (i < 14);

}


/* get-current_time -- get time as 60-bit 100ns ticks since UUID epoch.
Compensate for the fact that real clock resolution is
less than 100ns. */
static void get_current_time(uuid_time_t *timestamp)
{
    static int inited = 0;
    static uuid_time_t time_last;
    static uint16_t uuids_this_tick;
    uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_now);
        uuids_this_tick = UUIDS_PER_TICK;
        inited = 1;
    }

    for (; ; ) {
        get_system_time(&time_now);

        /* if clock reading changed since last UUID generated, */
        if (time_last != time_now) {
            /* reset count of uuids gen'd with this clock reading */
            uuids_this_tick = 0;
            time_last = time_now;
            break;
        }
        if (uuids_this_tick < UUIDS_PER_TICK) {
            uuids_this_tick++;
            break;
        }
        /* going too fast for our clock; spin */
    }
    /* add the count of uuids to low order bits of the clock reading */
    *timestamp = time_now + uuids_this_tick;
}


/* system dependent call to get IEEE node ID.
This sample implementation generates a random node ID. */
/* netperf mod - don't bother trying to read or write the nodeid */
static void get_ieee_node_identifier(uuid_node_t *node)
{
    static int inited = 0;
    static uuid_node_t saved_node;
    char seed[16];

    if (!inited) {
        get_random_info(seed);
        seed[0] |= 0x01;
        memcpy(&saved_node, seed, sizeof saved_node);
    }
    inited = 1;

    *node = saved_node;
}

/* snpuid -- print a UUID in the supplied buffer */
void snpuid(char *str, size_t size, netperf_uuid_t u) {
    int i;
    char *tmp = str;

    if (size < 38) {
        snprintf(tmp, size, "%s", "uuid string too small");
        return;
    }

    /* perhaps this is a trifle optimistic but what the heck */
    sprintf(tmp,
        "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-",
        u.time_low,
        u.time_mid,
        u.time_hi_and_version,
        u.clock_seq_hi_and_reserved,
        u.clock_seq_low);
    tmp += 24;
    for (i = 0; i < 6; i++) {
        sprintf(tmp, "%2.2x", u.node[i]);
        tmp += 2;
    }
    *tmp = 0;
}

/* format_uuid_v1 -- make a UUID from the timestamp, clockseq,
and node ID */
static void format_uuid_v1(netperf_uuid_t* uuid, uint16_t clock_seq,
    uuid_time_t timestamp, uuid_node_t node)
{
    /* Construct a version 1 uuid with the information we've gathered
    plus a few constants. */
    uuid->time_low = (unsigned long)(timestamp & 0xFFFFFFFF);
    uuid->time_mid = (unsigned short)((timestamp >> 32) & 0xFFFF);
    uuid->time_hi_and_version =
        (unsigned short)((timestamp >> 48) & 0x0FFF);
    uuid->time_hi_and_version |= (1 << 12);
    uuid->clock_seq_low = clock_seq & 0xFF;
    uuid->clock_seq_hi_and_reserved = (clock_seq & 0x3F00) >> 8;
    uuid->clock_seq_hi_and_reserved |= 0x80;
    memcpy(&uuid->node, &node, sizeof uuid->node);
}

char *iusUuidCreate()
{
    netperf_uuid_t uuid;
    uuid_time_t timestamp;
    uint16_t clockseq;
    uuid_node_t node;
    char *uuid_str = (char *)calloc(38, sizeof(char));

    /* get time, node ID, saved state from non-volatile storage */
    get_current_time(&timestamp);
    get_ieee_node_identifier(&node);

    /* for us clockseq is always to be random as we have no state */
    clockseq = true_random();

    /* stuff fields into the UUID */
    format_uuid_v1(&uuid, clockseq, timestamp, node);
    snpuid(uuid_str, 38, uuid);

    return uuid_str;
}

#else

#include <uuid/uuid.h>

char *iusUuidCreate()
{
  char *uuid_str;
  uuid_t uuid;

  // generate
  // uuid_generate_time_safe(uuid);
#ifdef __APPLE__
  uuid_generate_time(uuid);
#else
  uuid_generate_time_safe(uuid);
#endif
  // unparse (to string)
  uuid_str = (char *)calloc(37, sizeof(char));    // ex. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + "\0"
  uuid_unparse_lower(uuid, uuid_str);

  return uuid_str; 
}
#endif

void setIusUuidCreate(char* c)
{
  char* id = iusUuidCreate();
  
  strcpy(c, id);

  free(id);
}

