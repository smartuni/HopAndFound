#ifndef HAF_PROTOCOL_H
#define HAF_PROTOCOL_H

#include <stdint.h>
#include <net/gnrc/ipv6/netif.h>
#include "global.h"

typedef enum pkg_type{
	HEARTBEAT,
	LOCALIZATION_REQUEST,
	LOCALIZATION_REPLY,
	CALL_FOR_HELP,
	BIND
} pkg_type_t;

typedef struct __attribute__((packed)) {
	uint8_t type;
} heartbeat_t;

typedef struct __attribute__((packed)) {
	uint8_t type;
} localization_request_t;

typedef struct __attribute__((packed)) {
	uint8_t type;
	uint8_t node_id;
} localization_reply_t;

typedef struct __attribute__((packed)) {
	uint8_t type;
	uint32_t seq_nr;
	uint8_t mi_id;
	uint8_t node_list[MAX_NODES];		// nodes which are in range of the monitored item
#ifdef TEST_PRESENTATION
	uint8_t node_list_path[MAX_NODES];	// nodes which forwarded THIS call for help
#endif /* TEST_PRESENTATION */
}call_for_help_t;

typedef struct __attribute__((packed)) {
	uint8_t type;
	ipv6_addr_t* mi;
} bind_t;

#endif /* HAF_PROTOCOL_H */
