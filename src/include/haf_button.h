#ifndef HAF_BUTTON_H
#define HAF_BUTTON_H

typedef void (*haf_button_cb_t)(void *arg);

void haf_button_cb_node(void *arg);
void haf_button_cb_monitor(void *arg);
void haf_button_cb_monitored_item(void *arg);
int haf_button_init(haf_button_cb_t cb);

#endif /* HAF_BUTTON_H */
