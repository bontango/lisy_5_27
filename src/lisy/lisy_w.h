#ifndef LISY_W_H
#define LISY_W_H

void lisy_w_display_handler( void );
void lisy_w_throttle(void);
int  lisymini_get_gamename(char *gamename);
void lisy_w_solenoid_handler( void );
void lisy_w_switch_handler( void );
unsigned char lisy_w_switch_reader( unsigned char *action );
void lisy_w_lamp_handler(void);

#endif  /* LISY_W_H */

