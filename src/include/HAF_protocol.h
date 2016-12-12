#ifndef HAF_PROTOCOL_H
#define HAF_PROTOCOL_H

#include <stdint.h>

#include "global.h"


typedef struct __attribute__((packed)) {
	uint8_t mac_adr; //Datentyp prüfen 12 Hex Zeichen
	uint8_t hops;
	//uint8_t rtt; //RoundTripTime - Zeitangabe
	uint8_t next_hop_adr; //Datentyp prüfen wie mac_adr
	uint32_t exp_time; //Expiration Time - Zeitstempel
}routing_tbl_t;

typedef enum pkg_type{
	HEARTBEAT,
	LOCALIZATION_REQUEST,
	LOCALIZATION_REPLY,
	CALL_FOR_HELP,
	UPDATE
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
	uint8_t mi_id; //CALL FOR HELP ÜBER MAX ADR
	uint8_t node_list[MAX_NODES];
	uint8_t ttl; //time to live
	uint8_t dest_adr;
#ifdef TEST_PRESENTATION
	uint8_t node_list_path[MAX_NODES];	// nodes which forwarded THIS call for help
#endif /* TEST_PRESENTATION */
}call_for_help_t;
 
typedef struct __attribute__((packed)) { //mac adr from source adr
	uint8_t type;
	uint8_t source_adr;
	routing_tbl_t routing_tbl[MAX_DEVICES];
} update_t;

#endif /* HAF_PROTOCOL_H */
