#ifndef HAF_BUTTON_H
#define HAF_BUTTON_H

typedef void (*haf_button_cb_t)(void);

/* 
 * Callbackfunction for node button.
 */
void haf_button_cb_node(void);

/* 
 * Callbackfunction for monitor button.
 */
void haf_button_cb_monitor(void);

/* 
 * Callbackfunction for monitored_item button.
 */
void haf_button_cb_monitored_item(void);

/* 
 * Initializes the button.
 */
int haf_button_init(haf_button_cb_t cb);

#endif /* HAF_BUTTON_H */
