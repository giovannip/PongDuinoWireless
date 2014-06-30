#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Uno
#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 105
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void SetPause();
void CheckPause();
void SetBallFirstPosition();
void MoveBall();
void SetRacketsFirstPosition();
byte AdjustRacketValue(int ReadValue);
void ReadRacketsValues();
void SetCurrentRacketValue();
void MoveRackets();
void DrawWinner(int pWinner);
void CheckWinner();
void Start();
void BeginGame();
void DrawHeader();
void DrawWelcome();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Users\gprudencio\Documents\GitHub\Arduino\Projects\PongDuinoWireless\PongDuinoWireless.ino"
#endif
