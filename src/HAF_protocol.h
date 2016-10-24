
#ifndef HAF_PROTOCOL_H
#define HAF_PROTOCOL_H

#define MAX_NODES 6

typedef enum pkg_type{
	HEARTBEAT,
	LOCALIZATION_REQUEST,
	LOCALIZATION_REPLY,
	CALL_FOR_HELP
}pkg_type_t;


typedef struct heartbeat{
	char type;
}heartbeat_t;


typedef struct localization_request{
	char type;
}localization_request_t;


typedef struct localization_reply{
	char type;
	char node_id;
}localization_reply_t;


typedef struct call_for_help{
	char type;
	int seq_nr;
	char mi_id;
	char node_list[MAX_NODES];
}call_for_help_t;


#endif //HAF_PROTOCOL_H
