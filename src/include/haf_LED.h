#ifndef HAF_LED_H
#define HAF_LED_H

/* 
 * Enum for LEDs.
 * 
 * LED0	=	RED
 * LED1	=	GREEN
 * LED2	=	BLUE
 */
 
typedef enum {
	LED_RED,
	LED_GREEN,
	LED_BLUE
} led_t;

/* 
 * Initializes LED mutex and timer.
 */
void init_LED(void);

/* 
 * Starts timer for blinking LED.
 */
void start_LED_blink(led_t led, uint time);

#endif /* HAF_LED_H */
