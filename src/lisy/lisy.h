#ifndef LISY_H
#define LISY_H

// here we have all the xternals for lisy1.c and lisy80.c
extern int lisy_hardware_revision;
extern int lisy_hardware_ID;
extern ls80dbg_t ls80dbg;
extern ls80opt_t ls80opt;
extern int lisy_time_to_quit_flag;
extern struct timeval lisy_start_t;

int lisy_time_to_quit(void);
void lisy_shutdown(void);
void lisy_hw_init(int lisy_variant);
int lisy_set_gamename( char *arg_from_main, char *lisy_gamename);
void lisy_nvram_write_to_file( void );
void lisy_get_sw_version( unsigned char *sw_main, unsigned char *sw_sub, unsigned char *commit);

//the LISY HW revisions
#define LISY_HW_LISY1   100             //LISY1
#define LISY_HW_LISY80_311  311         //LISY80, old HW Version 3.11 (discontinue?)
#define LISY_HW_LISY35  350             //LISY35 Bally
#define LISY_HW_LISY80  80              //LISY80 HW320 & LISY_Home
#define LISY_HW_LISY_W  121             //LISYx_W (Williams) based on LISY_MINI1


#endif  /* LISY_H */

