#ifndef HAF_PROTOCOL_H
#define HAF_PROTOCOL_H

#include <stdint.h>

#include "global.h"

typedef enum pkg_type{
	HEARTBEAT,
	LOCALIZATION_REQUEST,
	LOCALIZATION_REPLY,
	CALL_FOR_HELP
} pkg_type_t;

typedef struct heartbeat{//__attribute__((packed)) {
	uint8_t type;
} heartbeat_t;

typedef struct localization_request{//__attribute__((packed)) {
	uint8_t type;
} localization_request_t;

typedef struct localization_reply{//__attribute__((packed)) {
	uint8_t type;
	uint8_t node_id;
} localization_reply_t;

typedef struct call_for_help{//__attribute__((packed)) {
	uint8_t type;
	uint32_t seq_nr;
	uint8_t mi_id;
	uint8_t node_list[MAX_NODES];
}call_for_help_t;

#endif /* HAF_PROTOCOL_H */
