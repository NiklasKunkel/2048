#ifndef BUTTONS_H_
#define BUTTONS_H_

void uplrToDpad(int Dpad[]);
void init_wdt(void);
void init_button(void);
void interrupt button_handler(void);
void interrupt WDT_interval_handler();

#endif /* BUTTONS_H_ */
