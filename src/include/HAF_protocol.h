#ifndef HAF_PROTOCOL_H
#define HAF_PROTOCOL_H

#include <stdint.h>

#define MAX_NODES (6)

typedef enum pkg_type{
	HEARTBEAT,
	LOCALIZATION_REQUEST,
	LOCALIZATION_REPLY,
	CALL_FOR_HELP
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
	uint8_t node_list[MAX_NODES];
}call_for_help_t;

#endif /* HAF_PROTOCOL_H */
