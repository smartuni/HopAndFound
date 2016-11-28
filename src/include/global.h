#ifndef GLOBAL_H
#define GLOBAL_H

#define NODE_ID (4)
#define MONITORED_ITEM_ID (1)
#define MAX_NODES (6)

//#define MONITORED_ITEM_IP { 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \                            0xf8, 0xe3, 0x4e, 0x62, 0x71, 0xba, 0x60, 0x0a }
#define MONITORED_ITEM_IP ("fe80::f8e3:4e62:71ba:600a")

#define SIGNAL_STRENGTH_MONITOR (-35)
#define SIGNAL_STRENGTH_MONITORED_ITEM (-27)
#define SIGNAL_STRENGTH_NODE (-21)

#define NETIF_CHANNEL (11)

#define STARTUP_SLEEPTIME_SEC (1)

#define HAF_DEBUG
#define HAF_DEBUG_NODE_MAP
#define HAF_DEBUG_DISPATCH
#define HAF_DEBUG_DONT_PRINT_EMPTY_MAP

#define TEST_PRESENTATION

#endif /* GLOBAL_H */
