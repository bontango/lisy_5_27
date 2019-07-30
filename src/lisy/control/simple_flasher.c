 /*
  bontango 07.2019
  lisy simple flasher, all plattforms
*/

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#define TESTSWITCH 22
#define DIP2 12
#define DIP7 23
#define DIP8 21
#define LED1 14 //red LED
#define LED2 5 //yellow LED
#define LED3 4 //green LED


//simple debounce and 'long hold' routine
//returns:
//0 - switch not pressed or bounce detected
//1 - switch pressed short
//2 - switch pressed long period
int myswitch( int pin)
{

unsigned int time_pressed, time_passed;

//switch closed?
if (digitalRead(pin)) return 0; //we have a pull up resistor, so status!=0 means switch open

//switch is closed, lets do a debounce
delay(50);
if (digitalRead(pin)) return 0; //closed to short
time_pressed = millis(); //store the time

//OK, switch is closed, lets wait for open again
while ( digitalRead(pin) == 0);

//lookhow long the switch was pressed
time_passed = millis() - time_pressed;

if ( time_passed > 800) return 2; else return 1;


}

//toggle all leds
//will set to 0 with first call
void toggle_all_leds(void)
{
 static unsigned char status = 1;

 if(status)
 {
   digitalWrite ( LED1, 0);
   digitalWrite ( LED2, 0);
   digitalWrite ( LED3, 0);
   status = 0;
 }
 else
 {
   digitalWrite ( LED1, 1);
   digitalWrite ( LED2, 1);
   digitalWrite ( LED3, 1);
   status = 1;
 }


}

int main (int argc, char *argv[])
{

int status, counts;
int lisy_variant;

// wiringPi library
   wiringPiSetup();

//init switches
pinMode ( TESTSWITCH, INPUT);
pinMode ( DIP2, INPUT);
pinMode ( DIP7, INPUT);
pinMode ( DIP8, INPUT);
pullUpDnControl (TESTSWITCH, PUD_UP);
pullUpDnControl (DIP2, PUD_UP);
pullUpDnControl (DIP7, PUD_UP);
pullUpDnControl (DIP8, PUD_UP);
//init LEDs
pinMode ( LED1, OUTPUT);
pinMode ( LED2, OUTPUT);
pinMode ( LED3, OUTPUT);
toggle_all_leds();

//wait for actions, lets blink LEDs meanwhile
counts = 0;
do {

	delay(1);
	if (counts++ > 1000) //one sec passed
	{
	 toggle_all_leds();
	 counts = 0;
	}
} while ( ( status = myswitch(TESTSWITCH) ) == 0 );

//switch presed (long or short)
//set LEDs to 0
digitalWrite ( LED1, 0);
digitalWrite ( LED2, 0);
digitalWrite ( LED3, 0);

//determine variant we are running on
lisy_variant = ( digitalRead(DIP2) + ( 2*digitalRead(DIP7)) + ( 4*digitalRead(DIP8)));
//LISY1 all off == 7
//LISY80 dip2=ON == 6
//LISY35 dip2=ON, dip8=ON == 2
//LISYHOME dip7=ON == 5

printf("LISY Variant is: %d\n", lisy_variant);

}//main
