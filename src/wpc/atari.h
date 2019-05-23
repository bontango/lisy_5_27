#ifndef INC_ATARI
#define INC_ATARI
#if !defined(__GNUC__) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)	// GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "core.h"
#include "wpcsam.h"
#include "sim.h"
#include "sndbrd.h"

/*-------------------------
/ Machine driver constants
/--------------------------*/
#define ATARI_ROMEND	ROM_END

/*-- Common Inports for ATARIGames --*/
#define ATARI1_COMPORTS \
  PORT_START /* 0 */ \
    /* switch column 1 */ \
    COREPORT_BIT(     0x0001, "Left Coin", KEYCODE_3) \
    COREPORT_BIT(     0x0002, "Right Coin", KEYCODE_5) \
    COREPORT_BIT(     0x0004, "Game", KEYCODE_1) \
    COREPORT_BIT(     0x0008, "Slam Tilt", KEYCODE_HOME) \
    /* switch column 1 or 3, depending on the generation */ \
    COREPORT_BIT(     0x0100, "Cabinet Tilt", KEYCODE_DEL) \
    COREPORT_BIT(     0x0200, "Pendulum Tilt", KEYCODE_INSERT) \
    /* switch column 0, as it's doubled from the dip switches */ \
    COREPORT_BIT(     0x8000, "Test", KEYCODE_7) \
  PORT_START /* 1 */ \
    COREPORT_DIPNAME( 0x0800, 0x0000, "S1/1") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0800, "1" ) \
    COREPORT_DIPNAME( 0x0400, 0x0000, "S1/2") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0400, "1" ) \
    COREPORT_DIPNAME( 0x0200, 0x0000, "S1/3") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0200, "1" ) \
    COREPORT_DIPNAME( 0x0100, 0x0000, "S1/4") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0100, "1" ) \
    COREPORT_DIPNAME( 0x8000, 0x8000, "S1/5") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x8000, "1" ) \
    COREPORT_DIPNAME( 0x4000, 0x4000, "S1/6") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x4000, "1" ) \
    COREPORT_DIPNAME( 0x2000, 0x0000, "S1/7") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x2000, "1" ) \
    COREPORT_DIPNAME( 0x1000, 0x1000, "S1/8") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x1000, "1" ) \
    COREPORT_DIPNAME( 0x0008, 0x0000, "S2/1") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0008, "1" ) \
    COREPORT_DIPNAME( 0x0004, 0x0004, "S2/2") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0004, "1" ) \
    COREPORT_DIPNAME( 0x0002, 0x0000, "S2/3") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0002, "1" ) \
    COREPORT_DIPNAME( 0x0001, 0x0000, "S2/4") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0001, "1" ) \
    COREPORT_DIPNAME( 0x0080, 0x0000, "S2/5") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0080, "1" ) \
    COREPORT_DIPNAME( 0x0040, 0x0000, "S2/6") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0040, "1" ) \
    COREPORT_DIPNAME( 0x0020, 0x0020, "S2/7") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0020, "1" ) \
    COREPORT_DIPNAME( 0x0010, 0x0000, "S2/8") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0010, "1" ) \
  PORT_START /* 2 */ \
    COREPORT_DIPNAME( 0x000f, 0x0005, "Replay Settings") \
      COREPORT_DIPSET(0x0000, "#0" ) \
      COREPORT_DIPSET(0x0001, "#1" ) \
      COREPORT_DIPSET(0x0002, "#2" ) \
      COREPORT_DIPSET(0x0003, "#3" ) \
      COREPORT_DIPSET(0x0004, "#4" ) \
      COREPORT_DIPSET(0x0005, "#5" ) \
      COREPORT_DIPSET(0x0006, "#6" ) \
      COREPORT_DIPSET(0x0007, "#7" ) \
      COREPORT_DIPSET(0x0008, "#8" ) \
      COREPORT_DIPSET(0x0009, "#9" ) \
      COREPORT_DIPSET(0x000a, "#10" ) \
      COREPORT_DIPSET(0x000b, "#11" ) \
      COREPORT_DIPSET(0x000c, "#12" ) \
      COREPORT_DIPSET(0x000d, "#13" ) \
      COREPORT_DIPSET(0x000e, "#14" ) \
      COREPORT_DIPSET(0x000f, "#15" )

#define ATARI2_COMPORTS \
  PORT_START /* 0 */ \
    /* switch column 1 */ \
    COREPORT_BIT(     0x0004, "Game", KEYCODE_1)  \
    COREPORT_BIT(     0x0008, "Left Coin", KEYCODE_3)  \
    COREPORT_BIT(     0x0010, "Right Coin", KEYCODE_5)  \
    COREPORT_BIT(     0x0081, "Test", KEYCODE_7)  \
  PORT_START /* 1 */ \
    COREPORT_DIPNAME( 0x0001, 0x0000, "S1") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0001, "1" ) \
    COREPORT_DIPNAME( 0x0002, 0x0000, "S2") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0002, "1" ) \
    COREPORT_DIPNAME( 0x0004, 0x0000, "S3") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0004, "1" ) \
    COREPORT_DIPNAME( 0x0008, 0x0008, "S4") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0008, "1" ) \
    COREPORT_DIPNAME( 0x0010, 0x0010, "S5") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0010, "1" ) \
    COREPORT_DIPNAME( 0x0020, 0x0020, "S6") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0020, "1" ) \
    COREPORT_DIPNAME( 0x0040, 0x0040, "S7") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0040, "1" ) \
    COREPORT_DIPNAME( 0x0080, 0x0080, "S8") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0080, "1" ) \
    COREPORT_DIPNAME( 0x0100, 0x0000, "S9") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0100, "1" ) \
    COREPORT_DIPNAME( 0x0200, 0x0000, "S10") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0200, "1" ) \
    COREPORT_DIPNAME( 0x0400, 0x0000, "S11") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0400, "1" ) \
    COREPORT_DIPNAME( 0x0800, 0x0000, "S12") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0800, "1" ) \
    COREPORT_DIPNAME( 0x1000, 0x0000, "S13") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x1000, "1" ) \
    COREPORT_DIPNAME( 0x2000, 0x2000, "S14") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x2000, "1" ) \
    COREPORT_DIPNAME( 0x4000, 0x4000, "S15") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x4000, "1" ) \
    COREPORT_DIPNAME( 0x8000, 0x0000, "S16") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x8000, "1" ) \
  PORT_START /* 2 */ \
    COREPORT_DIPNAME( 0x0001, 0x0001, "S17") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0001, "1" ) \
    COREPORT_DIPNAME( 0x0002, 0x0000, "S18") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0002, "1" ) \
    COREPORT_DIPNAME( 0x0004, 0x0004, "S19") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0004, "1" ) \
    COREPORT_DIPNAME( 0x0008, 0x0000, "S20") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0008, "1" ) \
    COREPORT_DIPNAME( 0x0010, 0x0010, "S21") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0010, "1" ) \
    COREPORT_DIPNAME( 0x0020, 0x0000, "S22") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0020, "1" ) \
    COREPORT_DIPNAME( 0x0040, 0x0040, "S23") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0040, "1" ) \
    COREPORT_DIPNAME( 0x0080, 0x0080, "S24") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0080, "1" ) \
    COREPORT_DIPNAME( 0x0100, 0x0100, "S25") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0100, "1" ) \
    COREPORT_DIPNAME( 0x0200, 0x0000, "S26") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0200, "1" ) \
    COREPORT_DIPNAME( 0x0400, 0x0400, "S27") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0400, "1" ) \
    COREPORT_DIPNAME( 0x0800, 0x0800, "S28") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0800, "1" ) \
    COREPORT_DIPNAME( 0x1000, 0x1000, "S29") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x1000, "1" ) \
    COREPORT_DIPNAME( 0x2000, 0x0000, "S30") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x2000, "1" ) \
    COREPORT_DIPNAME( 0x4000, 0x4000, "S31") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x4000, "1" ) \
    COREPORT_DIPNAME( 0x8000, 0x8000, "S32") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x8000, "1" )

#define ATARI3_COMPORTS \
  PORT_START /* 3 */ \
    COREPORT_DIPNAME( 0x0001, 0x0000, "S33") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0001, "1" ) \
    COREPORT_DIPNAME( 0x0002, 0x0000, "S34") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0002, "1" ) \
    COREPORT_DIPNAME( 0x0004, 0x0000, "S35") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0004, "1" ) \
    COREPORT_DIPNAME( 0x0008, 0x0000, "S36") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0008, "1" ) \
    COREPORT_DIPNAME( 0x0010, 0x0000, "S37") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0010, "1" ) \
    COREPORT_DIPNAME( 0x0020, 0x0000, "S38") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0020, "1" ) \
    COREPORT_DIPNAME( 0x0040, 0x0000, "S39") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0040, "1" ) \
    COREPORT_DIPNAME( 0x0080, 0x0000, "S40") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0080, "1" ) \
    COREPORT_DIPNAME( 0x0100, 0x0000, "S41") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0100, "1" ) \
    COREPORT_DIPNAME( 0x0200, 0x0000, "S42") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0200, "1" ) \
    COREPORT_DIPNAME( 0x0400, 0x0000, "S43") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0400, "1" ) \
    COREPORT_DIPNAME( 0x0800, 0x0000, "S44") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0800, "1" ) \
    COREPORT_DIPNAME( 0x1000, 0x0000, "S45") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x1000, "1" ) \
    COREPORT_DIPNAME( 0x2000, 0x0000, "S46") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x2000, "1" ) \
    COREPORT_DIPNAME( 0x4000, 0x0000, "S47") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x4000, "1" ) \
    COREPORT_DIPNAME( 0x8000, 0x0000, "S48") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x8000, "1" ) \
  PORT_START /* 4 */ \
    COREPORT_DIPNAME( 0x0001, 0x0000, "S49") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0001, "1" ) \
    COREPORT_DIPNAME( 0x0002, 0x0000, "S50") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0002, "1" ) \
    COREPORT_DIPNAME( 0x0004, 0x0000, "S51") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0004, "1" ) \
    COREPORT_DIPNAME( 0x0008, 0x0000, "S52") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0008, "1" ) \
    COREPORT_DIPNAME( 0x0010, 0x0000, "S53") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0010, "1" ) \
    COREPORT_DIPNAME( 0x0020, 0x0000, "S54") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0020, "1" ) \
    COREPORT_DIPNAME( 0x0040, 0x0000, "S55") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0040, "1" ) \
    COREPORT_DIPNAME( 0x0080, 0x0000, "S56") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0080, "1" ) \
    COREPORT_DIPNAME( 0x0100, 0x0000, "S57") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0100, "1" ) \
    COREPORT_DIPNAME( 0x0200, 0x0000, "S58") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0200, "1" ) \
    COREPORT_DIPNAME( 0x0400, 0x0000, "S59") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0400, "1" ) \
    COREPORT_DIPNAME( 0x0800, 0x0000, "S60") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x0800, "1" ) \
    COREPORT_DIPNAME( 0x1000, 0x0000, "S61") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x1000, "1" ) \
    COREPORT_DIPNAME( 0x2000, 0x0000, "S62") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x2000, "1" ) \
    COREPORT_DIPNAME( 0x4000, 0x0000, "S63") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x4000, "1" ) \
    COREPORT_DIPNAME( 0x8000, 0x0000, "S64") \
      COREPORT_DIPSET(0x0000, "0" ) \
      COREPORT_DIPSET(0x8000, "1" )

/*-- Standard input ports --*/
#define ATARI1_INPUT_PORTS_START(name,balls) \
  INPUT_PORTS_START(name) \
    CORE_PORTS \
    SIM_PORTS(balls) \
    ATARI1_COMPORTS

#define ATARI2_INPUT_PORTS_START(name,balls) \
  INPUT_PORTS_START(name) \
    CORE_PORTS \
    SIM_PORTS(balls) \
    ATARI2_COMPORTS

#define ATARI3_INPUT_PORTS_START(name,balls) \
  INPUT_PORTS_START(name) \
    CORE_PORTS \
    SIM_PORTS(balls) \
    ATARI2_COMPORTS \
    ATARI3_COMPORTS

#define ATARI_INPUT_PORTS_END INPUT_PORTS_END

#define ATARI_COMINPORT       CORE_COREINPORT

#define ATARI_SOLSMOOTH       2 /* Smooth the Solenoids over this numer of VBLANKS */
#define ATARI_LAMPSMOOTH      3 /* Smooth the lamps over this number of VBLANKS */
#define ATARI_DISPLAYSMOOTH   2 /* Smooth the display over this number of VBLANKS */

/*-- To access C-side multiplexed solenoid/flasher --*/
#define ATARI_CSOL(x) ((x)+24)

/*-- ATARI switch numbers --*/

/*-- Memory regions --*/
#define ATARI_MEMREG_CPU	REGION_CPU1

/* CPUs */
#define ATARI_CPU	0

/*-- ATARI CPU regions and ROM, 3 game PROM version --*/
#define ATARI_3_ROMSTART(name, n1, chk1, n2, chk2, n3, chk3) \
   ROM_START(name) \
     NORMALREGION(0x10000, ATARI_MEMREG_CPU) \
       ROM_LOAD(n1, 0x2800, 0x0800, chk1) \
         ROM_RELOAD(0xa800, 0x0800) \
       ROM_LOAD(n2, 0x3000, 0x0800, chk2) \
         ROM_RELOAD(0xb000, 0x0800) \
       ROM_LOAD(n3, 0x3800, 0x0800, chk3) \
         ROM_RELOAD(0xb800, 0x0800) \
         ROM_RELOAD(0xf800, 0x0800)

/*-- ATARI CPU regions and ROM, 2 game PROM version --*/

/*NOTE: E00 should be loaded lower in memory,
	   so we load it first - easier than changing each entry in atarigames.c*/
#define ATARI_2_ROMSTART(name, n1, chk1, n2, chk2) \
   ROM_START(name) \
     NORMALREGION(0x10000, ATARI_MEMREG_CPU) \
       ROM_LOAD(n2, 0x7000, 0x0800, chk2) \
       ROM_LOAD(n1, 0x7800, 0x0800, chk1) \
         ROM_RELOAD(0xf800, 0x0800)

#define ATARI_4x4_ROMSTART(name, n1, chk1, n2, chk2, n3, chk3) \
   ROM_START(name) \
     NORMALREGION(0x10000, ATARI_MEMREG_CPU) \
       ROM_LOAD(n1, 0x8000, 0x2000, chk1) \
       ROM_LOAD(n2, 0xa000, 0x2000, chk2) \
       ROM_LOAD(n3, 0xc000, 0x2000, chk3) \
         ROM_RELOAD(0xe000, 0x2000)

#define ATARI_SNDSTART(n1, chk1) \
     NORMALREGION(0x1000, REGION_SOUND1) \
       ROM_LOAD_NIB_LOW(n1, 0x0000, 0x0200, chk1)

/*-- These are only here so the game structure can be in the game file --*/
extern MACHINE_DRIVER_EXTERN(ATARI0);
extern MACHINE_DRIVER_EXTERN(ATARI1);
extern MACHINE_DRIVER_EXTERN(ATARI1A);
extern MACHINE_DRIVER_EXTERN(ATARI2);
extern MACHINE_DRIVER_EXTERN(ATARI3);
extern MACHINE_DRIVER_EXTERN(ATARI4);
extern MACHINE_DRIVER_EXTERN(atari2s);
extern MACHINE_DRIVER_EXTERN(atari1s);

#define gl_mATARI0		ATARI0
#define gl_mATARI1		ATARI1
#define gl_mATARI1A		ATARI1A
#define gl_mATARI2		ATARI2
#define gl_mATARI3		ATARI3
#define gl_mATARI4		ATARI4

#endif /* INC_ATARI */