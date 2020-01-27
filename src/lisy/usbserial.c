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

//lisy routine for writing to usb serial device
//we do it here in order to beable to log all bytes send to APC
int lisy_api_write( unsigned char *data, int count, int debug  )
{

    int i;
    char helpstr[10];

   if ( debug != 0)
    {
     sprintf(debugbuf,"USB_write:");
     for(i=0; i<count; i++)
     {
       sprintf(helpstr," 0x%02x",data[i]);
       strcat(debugbuf,helpstr);
     }
    lisy80_debug(debugbuf);
   }

   return( write( lisy_usb_serfd,data,count));
}


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
 if ( lisy_api_write( &cmd,1,ls80dbg.bitv.basic) != 1)
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

  //USB debug?
  if(ls80dbg.bitv.basic)
  {
    sprintf(debugbuf,"USB_read_string: %s",content);
    lisy80_debug(debugbuf);
  }

 return(i);

}

//read one byte, and return data into *data 
//return -2 in case we had problems to send  cmd
//return -1 in case we had problems to receive byte
//return 0 otherwise
unsigned char lisy_api_read_byte(unsigned char cmd, unsigned char *data)
{

 //send command
 if ( lisy_api_write( &cmd,1,ls80dbg.bitv.basic) != 1) return (-2);

 //receive answer
 if ( read(lisy_usb_serfd,data,1) != 1) return (-1);

  //USB debug?
  if(ls80dbg.bitv.basic)
  {
    sprintf(debugbuf,"USB_read_byte: 0x%02x",*data);
    lisy80_debug(debugbuf);
  }

 return(0);

}

//this command has an option
//read answer of two byte, and return data into *data1 and *data2
//return -2 in case we had problems to send  cmd
//return -1 in case we had problems to receive byte
//return 0 otherwise
unsigned char lisy_api_read_2bytes(unsigned char cmd, unsigned char option, unsigned char *data1, unsigned char *data2)
{

 unsigned char cmd_data[2];
 //send command
 cmd_data[0] = cmd;
 cmd_data[1] = option;
 if ( lisy_api_write( cmd_data,2,ls80dbg.bitv.basic) != 2) return (-2);

 //receive answer
 if ( read(lisy_usb_serfd,data1,1) != 1) return (-1);
 if ( read(lisy_usb_serfd,data2,1) != 1) return (-1);

  //USB debug?
  if(ls80dbg.bitv.basic)
  {
    sprintf(debugbuf,"USB_read_2bytes: 0x%02x 0x%02x",*data1,*data2);
    lisy80_debug(debugbuf);
  }

 return(0);

}


#define ARDUINO_NO_TRIES 9
int lisy_usb_init()
{

   int i,j,n,ret,tries;
   char *portname = "/dev/ttyACM0";
   unsigned char data,cmd;
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

  return 0;
}


//print some usefull parameters
int lisy_usb_print_hw_info(void)
{
   unsigned char data,cmd,data1,data2,no_of_displays,no;
   char answer[80];
   int i,j,n,ret;

  //number of displays
  if ( lisy_api_read_byte(LISY_G_NO_DISP, &no_of_displays) < 0) return (-1);

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

    if ( lisy_api_read_byte(LISY_G_NO_DISP, &data) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Client supports %d displays",data);
    lisy80_debug(debugbuf);
    no_of_displays = data;

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
    fprintf(stderr,"Switch Status 1..8;9..16;17..24; ...:\n");
    for(i=0; i<=8; i++)
     {
       for(j=1; j<=8; j++)
	{
	 n = j + i*8;
	 fprintf(stderr,"%d ",lisy_usb_get_switch_status(n));
	}
      fprintf(stderr,"\n");
     }//i

/*  
    if ( read_string(LISY_G_GAME_INFO, answer) < 0) return (-1);
    sprintf(debugbuf,"LISY_Mini: Game Info: %s",answer);
    lisy80_debug(debugbuf);
*/


 return 0;

}


//send SEG14 data to a display
int lisy_usb_send_SEG14_to_disp(unsigned char disp, int num, uint16_t *data)
{

 unsigned char hi, lo;
 unsigned char cmd, i;
 int len,pos;
 unsigned char seg14_data[40];
 map_byte1_t apc1, pinmame1;
 map_byte2_t apc2, pinmame2;

 cmd = 255;
 switch(disp)
 {
        case 0: cmd = LISY_S_DISP_0; break;
        case 1: cmd = LISY_S_DISP_1; break;
        case 2: cmd = LISY_S_DISP_2; break;
        case 3: cmd = LISY_S_DISP_3; break;
        case 4: cmd = LISY_S_DISP_4; break;
        case 5: cmd = LISY_S_DISP_5; break;
        case 6: cmd = LISY_S_DISP_6; break;
 }

 if( cmd != 255 )
 {
  pos = 0;
  //construct data
  //command
  seg14_data[pos++] = cmd;
  //send length of byte sequence
  len = num * 2; //we send two byte per data
  seg14_data[pos++] = len;
  //send SEG14 data
  //LISYAPI 0.09
  //Unser Mapping ist d, c, b, a, e, f, g, Komma für's erste Byte 
  //und j, h, m, k, p, r , Punkt, n fuer's zweite. 
  //pinmame has
  //2 bytes (a-g encoded as bit 0 to 6 in first byte. 
  //h to r encoded as bit 0 to 6 in second byte. comma as bit 7 in second byte)
  //so we need to remap the segments
  for ( i=0; i<num; i++)
  {
    
    //low byte first
    pinmame1.byte = data[i] & 0xFF;
    //do map
    apc1.bitv_apc.a = pinmame1.bitv_pinmame.a;
    apc1.bitv_apc.b = pinmame1.bitv_pinmame.b;
    apc1.bitv_apc.c = pinmame1.bitv_pinmame.c;
    apc1.bitv_apc.d = pinmame1.bitv_pinmame.d;
    apc1.bitv_apc.e = pinmame1.bitv_pinmame.e;
    apc1.bitv_apc.f = pinmame1.bitv_pinmame.f;
    apc1.bitv_apc.g = pinmame1.bitv_pinmame.g;
    apc1.bitv_apc.comma = pinmame1.bitv_pinmame.FREE;
    //assign
    seg14_data[pos++] = apc1.byte;

    //high byte in second byte
    pinmame2.byte = data[i] >> 8;
    //do map
    apc2.bitv_apc.h = pinmame2.bitv_pinmame.h;
    apc2.bitv_apc.j = pinmame2.bitv_pinmame.j;
    apc2.bitv_apc.k = pinmame2.bitv_pinmame.k;
    apc2.bitv_apc.m = pinmame2.bitv_pinmame.m;
    apc2.bitv_apc.n = pinmame2.bitv_pinmame.n;
    apc2.bitv_apc.p = pinmame2.bitv_pinmame.p;
    apc2.bitv_apc.r = pinmame2.bitv_pinmame.r;
    apc2.bitv_apc.dot = pinmame2.bitv_pinmame.dot;


    seg14_data[pos++] = apc2.byte;
  }
 }

//debug?
if ( ls80dbg.bitv.displays )
   {
    sprintf(debugbuf,"send cmd %d to Display %d: %d bytes of SEG14 data",cmd,disp,len);
    lisy80_debug(debugbuf);
   }

 //send it all
 if ( lisy_api_write( seg14_data,len+2,ls80dbg.bitv.displays) != len+2) { fprintf(stderr,"ERROR write display\n"); return (-2); }

 return (len+2);

}


//send a string to a display
//new with api 0.09: we send 'len' number of bytes, not a null terminated string
int lisy_usb_send_str_to_disp(unsigned char num, char *str)
{

 unsigned char cmd,len,i;
 unsigned char cmd_data[20];

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
 cmd_data[0] = cmd;
 //send length of byte sequence
 len = cmd_data[1] = strlen(str);
 //send bytes of string
 for(i=0; i<len; i++) cmd_data[i+2] = str[i];

 if ( lisy_api_write( cmd_data,len+2,ls80dbg.bitv.displays) != len+2) { fprintf(stderr,"ERROR write display\n"); return (-2); }
 }

 return (len+2);
}

//ask for changed switches
unsigned char lisy_usb_ask_for_changed_switch(void)
{

 unsigned char my_switch,cmd;
 int ret;

 //send command
 cmd = LISY_G_CHANGED_SW;
 if ( write( lisy_usb_serfd,&cmd,1) != 1) return (-2);
//receive answer
 if ( read(lisy_usb_serfd,&my_switch,1) != 1) return (-1);

 //USB debug? only if reurn is not 0x7f == no switch changed
 if((ls80dbg.bitv.switches) & ( my_switch != 0x7f))
 {
    sprintf(debugbuf,"USB_write: 0x%02x",cmd);
    lisy80_debug(debugbuf);
    sprintf(debugbuf,"USB_read_byte: 0x%02x",my_switch);
    lisy80_debug(debugbuf);
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
     if ( lisy_api_write( &cmd,1,ls80dbg.bitv.switches) != 1)
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
 unsigned char cmd_data[2];

  if (action) cmd=LISY_S_LAMP_ON; else cmd=LISY_S_LAMP_OFF;

  //send cmd
  cmd_data[0] = cmd;
  //send lamp number
  cmd_data[1] = lamp_no;

  if ( lisy_api_write( cmd_data,2,ls80dbg.bitv.lamps) != 2)
        fprintf(stderr,"Lamps Error writing to serial %s\n",strerror(errno));
}

//SOLENOID CONTROL
//solenoid ON and OFF
void lisy_usb_sol_ctrl(int sol_no,unsigned char action)
{
 uint8_t cmd;
 unsigned char cmd_data[2];

  if (action) cmd=LISY_S_SOL_ON; else cmd=LISY_S_SOL_OFF;

  //send cmd
  cmd_data[0] = cmd;
  //send lamp number
  cmd_data[1] = sol_no;

  if ( lisy_api_write( cmd_data,2,ls80dbg.bitv.coils) != 2)
        fprintf(stderr,"Solenoids Error writing to serial %s\n",strerror(errno));


}

//pulse solenoid
void lisy_usb_sol_pulse(int sol_no)
{
 uint8_t cmd;
 unsigned char cmd_data[2];

  cmd=LISY_S_PULSE_SOL;

  //send cmd
  cmd_data[0] = cmd;
  //send lamp number
  cmd_data[1] = sol_no;

  if ( lisy_api_write( cmd_data,2,ls80dbg.bitv.coils) != 2)
        fprintf(stderr,"Solenoids Error writing to serial %s\n",strerror(errno));
}

//set HW rule for solenoid
//RTH minimal version for the moment
void lisy_usb_sol_set_hwrule(int sol_no, int special_switch)
{
 uint8_t cmd;
 unsigned char cmd_data[11];

  cmd_data[0] = LISY_S_SET_HWRULE;
  cmd_data[1] = sol_no; //Index c of the solenoid to configure
  cmd_data[2] = special_switch; //Switch sw1. Set bit 7 to invert the switch.
  cmd_data[3] = 127; //Switch sw2. Set bit 7 to invert the switch.
  cmd_data[4] = 127;  //Switch sw3. Set bit 7 to invert the switch.
  cmd_data[5] = 80; //Pulse time in ms (0-255)
  cmd_data[6] = 191; ///Pulse PWM power (0-255). 0=0% power. 255=100% power 75% not used by APC
  cmd_data[7] = 64;  //Hold PWM power (0-255). 0=0% power. 255=100% power 25% not used by APC
  cmd_data[8] = 3; //sw1 will enable the rule and disable it when released.
  cmd_data[9] = 0; //do not use sw2
  cmd_data[10] = 0; //do not use sw3

  if ( ls80dbg.bitv.basic ) 
  {
    sprintf(debugbuf,"LISY_Mini: HW Rule set for solenoid:%d and switch:%d",sol_no,special_switch);
    lisy80_debug(debugbuf);
  }

  //11 bytes to follow
  if ( lisy_api_write( cmd_data,11,ls80dbg.bitv.basic) != 11)
        fprintf(stderr,"Setting hW rules, Error writing to serial\n");
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
        unsigned char cmd_data[3];

   cmd = LISY_S_DISP_PROT;

    //send cmd
    cmd_data[0] = cmd;
    //send display number
    cmd_data[1] = display_no;
    //send protocol number
    cmd_data[2] = protocol;

     if ( lisy_api_write( cmd_data,3,ls80dbg.bitv.displays) != 3)
        fprintf(stderr,"display option Error writing to serial\n");
}

void lisy_usb_sound_play_file( char *filename )
{
	uint8_t cmd;
	int i,len;
        unsigned char cmd_data[80];

 if ( ls80dbg.bitv.sound )
  {
    sprintf(debugbuf,"play sound %s",filename);
    lisy80_debug(debugbuf);
  }

     cmd = LISY_S_PLAY_FILE;

     //send cmd
     cmd_data[0] = cmd;
     //track 1
     cmd_data[1] = 1;
     //no flags
     cmd_data[2] = 0;
     //filename plus \0
     len = strlen(filename) +1;
     for(i=0; i<=len; i++) cmd_data[i+3] = filename[i];

     //len+4 cmd.track,flags + trailing \0
     if ( lisy_api_write( cmd_data,len+4,ls80dbg.bitv.sound) != len+4)
        fprintf(stderr,"sound play file error writing to serial\n");
}


//this is the boot message for lisy Mini
void lisy_usb_show_boot_message(char *software_version,char *system_id, int game_no, char *gamename)
{
        int i;
        char buf[22];


  //show 'system_id' on display one -> "LISY_W"
  sprintf(buf,"%-6s",system_id);
  lisy_usb_send_str_to_disp( 1, buf);

  //show 'gamename' on display two
  sprintf(buf,"%-6s",gamename);
  lisy_usb_send_str_to_disp( 2, buf);

  //show S2 Setting on display three
  sprintf(buf,"S2 %03d",game_no);
  lisy_usb_send_str_to_disp( 3, buf);

  //show Version number on Display 4
  lisy_usb_send_str_to_disp( 4, software_version);


  //status display countdown
  for (i=5; i>=0; i--)
   {
     sprintf(buf,"  %02d",i);
     lisy_usb_send_str_to_disp( 0, buf);
     sleep(1);
    }

  //blank status display
  lisy_usb_send_str_to_disp( 0, "    ");
}

