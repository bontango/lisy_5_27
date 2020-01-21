#ifndef LISY_USBSER_H
#define LISY_USBSER_H

int lisy_usb_init(void);
int lisy_usb_send_str_to_disp(unsigned char num, char *str);
int lisy_usb_send_SEG14_to_disp(unsigned char disp, int num, uint16_t *data);
unsigned char lisy_usb_ask_for_changed_switch(void);
unsigned char lisy_usb_get_switch_status( unsigned char number);
void lisy_usb_lamp_ctrl(int lamp_no,unsigned char action);
void lisy_usb_sol_ctrl(int sol_no,unsigned char action);
void lisy_usb_sol_pulse(int sol_no);
int lisy_api_read_string(unsigned char cmd, char *content);
unsigned char lisy_api_read_byte(unsigned char cmd, unsigned char *data);
void lisy_usb_sol_set_hwrule(int sol_no, int special_switch);
void lisy_usb_display_set_prot(unsigned char display_no,unsigned char protocol);
void lisy_usb_sound_play_file( char *filename );
int lisy_usb_print_hw_info(void);
void lisy_usb_show_boot_message(char *software_version,char *system_id, int game_no, char *gamename);


#endif  /* LISY_USBSER_H */

