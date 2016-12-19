#ifndef HAF_BUTTON_H
#define HAF_BUTTON_H

typedef void (*haf_button_cb_t)(void);

void haf_button_cb_node(void);
void haf_button_cb_monitor(void);
void haf_button_cb_monitored_item(void);
int haf_button_init(haf_button_cb_t cb);

#endif /* HAF_BUTTON_H */
