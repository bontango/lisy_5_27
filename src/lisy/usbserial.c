/*
 USB serial routines for LISY
 April 2019
 bontango
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "lisy_api.h"
#include "fileio.h"
#include "hw_lib.h"
#include "displays.h"
#include "coils.h"
#include "fadecandy.h"
#include "opc.h"
#include "utils.h"
#include "lisy_home.h"
#include "externals.h"
#include "usbserial.h"


//local vars
static int lisy_usb_serfd;

//subroutine for serial com
int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1; //timeout is 0.1secs
    //tty.c_cc[VTIME] = 0; //RTH: no timimng here, input is not a keyboard

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}


//send command cmd
//read \0 terminated string and store into string content
//return -2 in case we had problems to send  cmd
//return -1 in case we had problems to receive string
int lisy_api_read_string(unsigned char cmd, char *content)
{

  char nextbyte;
  int i,n,ret;

 //send command
 if ( write( lisy_usb_serfd,&cmd,1) != 1)
    {
        printf("Error writing to serial %s\n",strerror(errno));
        return -1;
    }
 
 //receive answer
 i=0;
 do {
  if ( ( ret = read(lisy_usb_serfd,&nextbyte,1)) != 1)
    {
        printf("Error reading from serial, return:%d %s\n",ret,strerror(errno));
        return -1;
    }
  content[i] = nextbyte;
  i++;
  } while ( nextbyte != '\0');
 
 return(i);

}

//read one byte, and return data into *data 
//return -2 in case we had problems to send  cmd
//return -1 in case we had problems to receive byte
//return 0 otherwise
unsigned char lisy_api_read_byte(unsigned char cmd, unsigned char *data)
{

 //send command
 if ( write( lisy_usb_serfd,&cmd,1) != 1) return (-2);

 //receive answer
 if ( read(lisy_usb_serfd,data,1) != 1) return (-1);

 return(0);

}

//this command has an option
//read answer of two byte, and return data into *data1 and *data2
//return -2 in case we had problems to send  cmd
//return -1 in case we had problems to receive byte
//return 0 otherwise
unsigned char lisy_api_read_2bytes(unsigned char cmd, unsigned char option, unsigned char *data1, unsigned char *data2)
{

 //send command
 if ( write( lisy_usb_serfd,&cmd,1) != 1) return (-2);
 if ( write( lisy_usb_serfd,&option,1) != 1) return (-2);

 //receive answer
 if ( read(lisy_usb_serfd,data1,1) != 1) return (-1);
 if ( read(lisy_usb_serfd,data2,1) != 1) return (-1);

 return(0);

}


#define ARDUINO_NO_TRIES 9
int lisy_usb_init()
{

   int i,j,n,ret,tries;
   char *portname = "/dev/ttyACM0";
   unsigned char data,cmd,data1,data2,no_of_displays,no;
   char answer[80];

    //open interface
    lisy_usb_serfd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (lisy_usb_serfd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }

    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(lisy_usb_serfd, B115200);


    //start with asking for Hardware string
    //for init we reapeat that up to ten times in case
    //the other side ( Arduino) needs time to resset
    //set the command first
    cmd = LISY_G_HW;
    //and flush buffer
    sleep(1); //need to wait a bit beofre doing that
    tcflush(lisy_usb_serfd,TCIOFLUSH);
    //now send and try to read answer (one byte)
    ret = tries = 0;
    do
    {
	//send cmd
     if ( write( lisy_usb_serfd,&cmd,1) != 1)
      {
        printf("Error writing to serial %s\n",strerror(errno));
        return -1;
      }

      //read answer
	if ( ( ret = read(lisy_usb_serfd,&data,1)) != 1)
         {
	   tries++; //count tries
	   sleep(1); //wait a second
	   tcflush(lisy_usb_serfd,TCIOFLUSH); //flush buffers
	   fprintf(stderr,"send cmd to %s, %d times\n",portname,tries);
	 }
    }
    while( (ret == 0) & ( tries < ARDUINO_NO_TRIES));


    //look if we exceeded tries
    if (tries >= ARDUINO_NO_TRIES) 
    {
      fprintf(stderr,"USBSerial: Init failed\n");
      return (-1);
    }
    else { n=0; answer[n] = data; n++; }

    
    //now read the rest
    do {
    if ( ( ret = read(lisy_usb_serfd,&data,1)) != 1)
    {
        printf("Error reading from serial, return:%d %s\n",ret,strerror(errno));
        return -1;
    }
    answer[n] = data;
    n++;
    } while (( data != '\0') & ( n < 10));

    fprintf(stderr,"LISY_Mini: HW Client is: %s \n",answer);


    //number of displays
    if ( lisy_api_read_byte(LISY_G_NO_DISP, &no_of_displays) < 0) return (-1);

  if ( ls80dbg.bitv.basic ) 
  {
    if ( lisy_api_read_string(LISY_G_LISY_VER, answer) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client has SW version: %s",answer);
    lisy80_debug(debugbuf);

    if ( lisy_api_read_string(LISY_G_API_VER, answer) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client uses API Version: %s",answer);
    lisy80_debug(debugbuf);

    if ( lisy_api_read_byte(LISY_G_NO_LAMPS, &data) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client supports %d lamps",data);
    lisy80_debug(debugbuf);

    if ( lisy_api_read_byte(LISY_G_NO_SOL, &data) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client supports %d solenoids",data);
    lisy80_debug(debugbuf);
/*  
    if ( read_byte(LISY_G_NO_SOUNDS, &data) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client supports %d sounds",data);
    lisy80_debug(debugbuf);
*/
    sprintf(debugbuf,"LISY_Mini: Client supports %d displays",data);
    lisy80_debug(debugbuf);
    lisy80_debug("settings now:");

    for(no=0; no<no_of_displays; no++)
	{
	  //query each display
          if ( lisy_api_read_2bytes(LISY_G_DISP_DETAIL, no, &data1, &data2) < 0) return (-1);
	  // verbose type
	  switch(data1)
	  {
		case 0: strcpy(answer,"Display index is invalid or does not exist in machine."); break;
		case 1: strcpy(answer,"BCD7, BCD Code for 7 Segment Displays without comma"); break;
		case 2: strcpy(answer,"BCD8, BCD Code for 8 Segment Displays (same as BCD7 but with comma"); break;
		case 3: strcpy(answer,"SEG7, Fully addressable 7 Segment Display (with comma)"); break;
		case 4: strcpy(answer,"SEG14,Fully addressable 14 Segment Display (with comma)"); break;
		case 5: strcpy(answer,"ASCII, ASCII Code"); break;
		case 6: strcpy(answer,"ASCII_DOT, ASCII Code with comma"); break;
	  }
    	  sprintf(debugbuf,"Display no:%d has type:%d (%s) with %d segments",no,data1,answer,data2);
    	  lisy80_debug(debugbuf);
	}

  }//debug basic

  //set displays to ASCII
  for(no=0; no<no_of_displays; no++) lisy_usb_display_set_prot( no, 5);



  if ( ls80dbg.bitv.basic ) 
  {

    lisy80_debug("\nsettings after init");

    for(no=0; no<no_of_displays; no++)
        {
          //query each display
          if ( lisy_api_read_2bytes(LISY_G_DISP_DETAIL, no, &data1, &data2) < 0) return (-1);
          // verbose type
          switch(data1)
          {
                case 0: strcpy(answer,"Display index is invalid or does not exist in machine."); break;
                case 1: strcpy(answer,"BCD7, BCD Code for 7 Segment Displays without comma"); break;
                case 2: strcpy(answer,"BCD8, BCD Code for 8 Segment Displays (same as BCD7 but with comma"); break;
                case 3: strcpy(answer,"SEG7, Fully addressable 7 Segment Display (with comma)"); break;
                case 4: strcpy(answer,"SEG14,Fully addressable 14 Segment Display (with comma)"); break;
                case 5: strcpy(answer,"ASCII, ASCII Code"); break;
                case 6: strcpy(answer,"ASCII_DOT, ASCII Code with comma"); break;
          }
          sprintf(debugbuf,"Display no:%d has type:%d (%s) with %d segments",no,data1,answer,data2);
          lisy80_debug(debugbuf);
	}


    if ( lisy_api_read_byte(LISY_G_NO_SW, &data) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client supports %d switches",data);
    lisy80_debug(debugbuf);

    //print status of switches
    printf("Switch Statusn 1..8;9..16;17..24; ...:\n");
    for(i=0; i<=8; i++)
     {
       for(j=1; j<=8; j++)
	{
	 n = j + i*8;
	 printf("%d ",lisy_usb_get_switch_status(n));
	}
      printf("\n");
     }//i

/*  
    if ( read_string(LISY_G_GAME_INFO, answer) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Game Info: %s",answer);
    lisy80_debug(debugbuf);
*/
   }

  return 0;

}


//send a string to a display
//new with api 0.09: we send 'len' number of bytes, not a null terminated string
int lisy_usb_send_str_to_disp(unsigned char num, char *str)
{

 unsigned char cmd, len;

 cmd = 255;
 switch(num)
 {
	case 0: cmd = LISY_S_DISP_0; break;
	case 1: cmd = LISY_S_DISP_1; break;
	case 2: cmd = LISY_S_DISP_2; break;
	case 3: cmd = LISY_S_DISP_3; break;
	case 4: cmd = LISY_S_DISP_4; break;
	case 5: cmd = LISY_S_DISP_5; break;
	case 6: cmd = LISY_S_DISP_6; break;
 }

 if ( ls80dbg.bitv.displays )
   {
    sprintf(debugbuf,"send cmd %d to Display %d: str: %s",cmd,num,str);
    lisy80_debug(debugbuf);
   }


 if( cmd != 255 )
 {
 //send command
 if ( write( lisy_usb_serfd,&cmd,1) != 1) { fprintf(stderr,"ERROR write cmd\n"); return (-1); }
 //send length of byte sequence
 len = strlen(str);
 if ( write( lisy_usb_serfd,&len,1) != 1) { fprintf(stderr,"ERROR write lenght\n"); return (-1); }
 //send bytes of string
 if ( write( lisy_usb_serfd,str,len) != len) { fprintf(stderr,"ERROR write display\n"); return (-2); }
 }

 return (len+2);
}

//ask for changed switches
unsigned char lisy_usb_ask_for_changed_switch(void)
{

 unsigned char my_switch;
 int ret;

 //ask APC via serial
 ret = lisy_api_read_byte(LISY_G_CHANGED_SW, &my_switch);
 if (ret < 0) 
  {
    fprintf(stderr,"ERROR: problem with switchreading: %d\n",ret);
    return 80;
  }

 return my_switch;

}

//get status of specific switch
unsigned char lisy_usb_get_switch_status( unsigned char number)
{

 int ret;
 unsigned char cmd,status;

      //send cmd
     cmd = LISY_G_STAT_SW;
     if ( write( lisy_usb_serfd,&cmd,1) != 1)
      {
        printf("Error writing to serial %s\n",strerror(errno));
        return -1;
      }

 //ask APC via serial
 ret = lisy_api_read_byte(number, &status);
 if (ret < 0) 
  {
    fprintf(stderr,"ERROR: problem with switch status reading: %d\n",ret);
  }

 return status;

}

//lamp control
void lisy_usb_lamp_ctrl(int lamp_no,unsigned char action)
{
 uint8_t cmd;

  if (action) cmd=LISY_S_LAMP_ON; else cmd=LISY_S_LAMP_OFF;

      //send cmd
     if ( write( lisy_usb_serfd,&cmd,1) != 1)
        fprintf(stderr,"Lamps Error writing to serial %s\n",strerror(errno));
     //send lamp number
      //send cmd
     if ( write( lisy_usb_serfd,&lamp_no,1) != 1)
        fprintf(stderr,"Lamps Error writing to serial %s\n",strerror(errno));
}

//SOLENOID CONTROL
//solenoid ON and OFF
void lisy_usb_sol_ctrl(int sol_no,unsigned char action)
{
 uint8_t cmd;

  if (action) cmd=LISY_S_SOL_ON; else cmd=LISY_S_SOL_OFF;

      //send cmd
     if ( write( lisy_usb_serfd,&cmd,1) != 1)
        fprintf(stderr,"Solenoids Error writing to serial %s\n",strerror(errno));
     //send solenoid number
      //send cmd
     if ( write( lisy_usb_serfd,&sol_no,1) != 1)
        fprintf(stderr,"Solenoids Error writing to serial %s\n",strerror(errno));


}

//pulse solenoid
void lisy_usb_sol_pulse(int sol_no)
{
 uint8_t cmd;

  cmd=LISY_S_PULSE_SOL;

      //send cmd
     if ( write( lisy_usb_serfd,&cmd,1) != 1)
        fprintf(stderr,"Solenoids Error writing to serial %s\n",strerror(errno));
     //send solenoid number
      //send cmd
     if ( write( lisy_usb_serfd,&sol_no,1) != 1)
        fprintf(stderr,"Solenoids Error writing to serial %s\n",strerror(errno));


}

//set HW rule for solenoid
//RTH minimal version for the moment
void lisy_usb_sol_set_hwrule(int sol_no, int special_switch)
{
 uint8_t cmd;
 uint8_t error_occured = 0;

  uint8_t index; //Index c of the solenoid to configure
  uint8_t sw1; //Switch sw1. Set bit 7 to invert the switch.
  uint8_t sw2; //Switch sw2. Set bit 7 to invert the switch.
  uint8_t sw3; //Switch sw3. Set bit 7 to invert the switch.
  uint8_t pulse_time; //Pulse time in ms (0-255)
  uint8_t pulse_pwm_power; //Pulse PWM power (0-255). 0=0% power. 255=100% power
  uint8_t hold_pwm_power; //Hold PWM power (0-255). 0=0% power. 255=100% power
  uint8_t flag_sw1; //Flag for sw1
  uint8_t flag_sw2; //Flag for sw2
  uint8_t flag_sw3; //Flag for sw3

  
  cmd=LISY_S_SET_HWRULE;
  index = sol_no;
  sw1 = special_switch;
  sw2 = 127; //no sw2
  sw3 = 127; //no sw3
  pulse_time = 80;
  pulse_pwm_power = 191; //75% not used by APC
  hold_pwm_power = 64;  //25% not used by APC
  flag_sw1 = 3; //sw1 will enable the rule and disable it when released.
  flag_sw2 = 0; //do not use sw2
  flag_sw3 = 0; //do not use sw3

  if ( ls80dbg.bitv.basic ) 
  {
    sprintf(debugbuf,"LISY_Mini: HW Rule set for solnenoid:%d and switch:%d",sol_no,special_switch);
    lisy80_debug(debugbuf);
    sprintf(debugbuf," send: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",cmd,index,sw1,sw2,sw3,pulse_time,pulse_pwm_power,hold_pwm_power,flag_sw1,flag_sw2,flag_sw3 );
    lisy80_debug(debugbuf);
  }

//10bytes to follow
if ( write( lisy_usb_serfd,&cmd,1) != 1) error_occured++;
if ( write( lisy_usb_serfd,&index,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&sw1,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&sw2,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&sw3,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&pulse_time,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&pulse_pwm_power,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&hold_pwm_power,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&flag_sw1,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&flag_sw2,1) != 1) error_occured++; 
if ( write( lisy_usb_serfd,&flag_sw3,1) != 1) error_occured++; 

if ( error_occured) 
        fprintf(stderr,"Setting hW rules, Error writing to serial %s , %d times\n",strerror(errno),error_occured);


}

//display control
//set a protocol for a display
// displayno is 0 to ... max displays -1
// option is
// 1: BCD7, BCD Code for 7 Segment Displays without comma
// 2: BCD8, BCD Code for 8 Segment Displays (same as BCD7 but with comma
// 3: SEG7, Fully addressable 7 Segment Display (with comma)
// 4: SEG14,Fully addressable 14 Segment Display (with comma)
// 5: ASCII, ASCII Code
// 6: ASCII_DOT, ASCII Code with comma
void lisy_usb_display_set_prot(uint8_t display_no,uint8_t protocol)
{
	uint8_t cmd;

     //send cmd
     cmd = LISY_S_DISP_PROT;
     if ( write( lisy_usb_serfd,&cmd,1) != 1)
        fprintf(stderr,"display option Error writing to serial %s\n",strerror(errno));
     //send display number
     if ( write( lisy_usb_serfd,&display_no,1) != 1)
        fprintf(stderr,"display option Error writing to serial %s\n",strerror(errno));
     //send protocol number
     if ( write( lisy_usb_serfd,&protocol,1) != 1)
        fprintf(stderr,"display option Error writing to serial %s\n",strerror(errno));
}

