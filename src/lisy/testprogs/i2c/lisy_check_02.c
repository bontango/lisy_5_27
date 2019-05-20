/* lisy_check
   based on i2cdetect
   Version 0.2
   bontango 01.2018
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>


/*
  Hardware Version 3.11 has eeprom at I2C address 0x50 & 0x51
  Hardware Version 3.21 has eeprom at I2C address 0x54 & 0x55
  Hardware Version for SYstem1 has eeprom at I2C address 0x52 & 0x53
  Hardware Version for Bally has eeprom at I2C address 0x56 & 0x57
  we probe all three, starting with 0x54 for 3.21
  return is:
  0 -> no lisy80 detected
  1 -> lisy80 HW Version 3.11
  2 -> lisy80 HW Version 3.21
  3 -> lisy1 baed on Lisy80 HW Version 3.21
  4 -> lisy35 for Bally/Stern  based on Lisy80 HW Version 3.21
 -1 -> on error

  use option -v (or anything else as second argument) for being verbose testing
*/

#define I2C_DEVICE "/dev/i2c-1"
#define DISP_PIC_ADDR 0x40              // Address of PIC for displays
#define LS80DPCMD_GET_SW_VERSION_MAIN 2
#define LS80DPCMD_GET_HW_REV 100


//globale var, used in most routines
union three {
    unsigned char byte;
    struct {
    unsigned COMMAND_BYTE:7, IS_CMD:1;
    //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
        } bitv;
    struct {
    unsigned DISPLAY:3, DIGIT:3, FREE:1, IS_CMD:1;
    //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
        } bitv2;   //for 80A
    struct {
    unsigned ROW:2, POSITION:4, COOKED:1, IS_CMD:1;
    //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
        } bitv3;   //for 80B
    } mydata_display;

int main(int argc, char *argv[])
{
 int fd_disp_pic, address, file, res;
 unsigned char data;
 int verbose = 0;

 if (argc >= 2) 
 {
  verbose = 1;
  printf("we are in verbose mode\n");
 } 

 // new: we check software version of display pic
 // with sw >=4  we have the ID in the PIC, no 24C04 anymore!

 if ((fd_disp_pic = open( I2C_DEVICE,O_RDWR)) < 0)
  { 
   printf("could not open I2C device\n");
   return -1;
 }
 // Set the port options and set the address of the device we wish to speak to
 if (ioctl(fd_disp_pic, I2C_SLAVE, DISP_PIC_ADDR) < 0)
  { 
   printf("could not do ioctl on I2C device\n");
   return -1;
 }

 //read main sw version

 /* build control byte */
 mydata_display.bitv.COMMAND_BYTE = LS80DPCMD_GET_SW_VERSION_MAIN;
 mydata_display.bitv.IS_CMD = 1;        //we are sending a command here

 if ( write(fd_disp_pic,&mydata_display.byte,1) != 1) //success
  { 
   printf("could not write to I2C device\n");
   return -1;
 }

 //wait a bit, PIC migth be slow
 usleep(200); //wait 200ms secs

 if (read(fd_disp_pic, &data, 1) != 1)
  { 
   printf("could not write to I2C device\n");
   return -1;
 }

 //we now have main sw version in data
 if ( data >= 4)
 {
    if (verbose) printf("we have SW Version >=4; getting HW ID now \n");

    /* build control byte */
    mydata_display.bitv.COMMAND_BYTE = LS80DPCMD_GET_HW_REV;
    mydata_display.bitv.IS_CMD = 1;        //we are sending a command here

 if ( write(fd_disp_pic,&mydata_display.byte,1) != 1) //success
  { 
   printf("could not write to I2C device\n");
   return -1;
 }

 //wait a bit, PIC migth be slow
 usleep(200); //wait 200ms secs

 if (read(fd_disp_pic, &data, 1) != 1)
  { 
   printf("could not write to I2C device\n");
   return -1;
 }

 //we now have HW ID in data
 if (verbose) printf("HW ID is %d\n",data);

  switch(data)
    {
        case 1 ... 40: 
                       if (verbose) printf("LISY80 HW 3.21 or newer detected\n");
		       return(2);
                       break;
        case 41 ... 80: 
		       if (verbose)  printf("LISY1 detected\n");
		       return(3);
                       break;
        case 81 ... 120:
		       if (verbose) printf("LISY35(Bally) detected\n");
		       return(4);
                       break;
        default: return(-1);
                       break;
    }

 }
  else
 {
    if (verbose) printf("we have SW Version <4; probing for I2C dev now\n");
 }

 

 //otherwise probe for I2C-eeprom 
 close(fd_disp_pic);

 file = open(I2C_DEVICE, O_RDWR);
 if ( file < 0 )
  { 
   printf("could not open bus at /dev/i2c-1\n");
   return -1;
 }

 //we start with probing for HW 3.21 -> 0x54
 address = 0x54;
 /* Set slave address */
 if (ioctl(file, I2C_SLAVE, address) < 0) {
	fprintf(stderr, "Error: Could not set "
			"address to 0x%02x: %s\n", address,
				strerror(errno));
		return -1;
 }

 if (verbose) printf("check i2c bus /dev/i2c-1 at address 0x54\n");
 res = i2c_smbus_read_byte(file);
 if (res >= 0 ) 
  { 
    if (verbose) printf("HW 3.21 detected\n");
    close(file);
    exit(2) ;
  }

 //next we probe for Bally/LISY35 -> 0x56
 address = 0x56;
 /* Set slave address */
 if (ioctl(file, I2C_SLAVE, address) < 0) {
	fprintf(stderr, "Error: Could not set "
			"address to 0x%02x: %s\n", address,
				strerror(errno));
		return -1;
 }

 if (verbose) printf("check i2c bus /dev/i2c-1 at address 0x56\n");
 res = i2c_smbus_read_byte(file);
 if (res >= 0 ) 
  { 
    if (verbose) printf("LISY35(Bally) detected\n");
    close(file);
    exit(4) ;
  }



 //now probe for HW 3.11 -> 0x50
 address = 0x50;
 /* Set slave address */
 if (ioctl(file, I2C_SLAVE, address) < 0) {
	fprintf(stderr, "Error: Could not set "
			"address to 0x%02x: %s\n", address,
				strerror(errno));
		return -1;
 }

 if (verbose) printf("check i2c bus /dev/i2c-1 at address 0x50\n");
 res = i2c_smbus_read_byte(file);
 if (res >= 0 ) 
  { 
    if (verbose) printf("HW 3.11 detected\n");
    close(file);
    exit(1) ;
  }

 //now probe for System1 -> 0x52
 address = 0x52;
 /* Set slave address */
 if (ioctl(file, I2C_SLAVE, address) < 0) {
	fprintf(stderr, "Error: Could not set "
			"address to 0x%02x: %s\n", address,
				strerror(errno));
		return -1;
 }

 if (verbose) printf("check i2c bus /dev/i2c-1 at address 0x52\n");
 res = i2c_smbus_read_byte(file);
 if (res >= 0 ) 
  { 
    if (verbose) printf("LISY1 detected\n");
    close(file);
    exit(3) ;
  }

 //OK, looks like lisy80 Hardware is not connected
 fprintf(stderr,"Could not find LISY80/LISY1 Hardware, please contact bontango at www.lisy80.com to buy it ;-)\n");
 close(file);
 exit(0) ;

}
