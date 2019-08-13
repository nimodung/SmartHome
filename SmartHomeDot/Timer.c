/*
 * Timer.c
 *
 * Created: 2019-04-10 오전 11:04:01
 *  Author:  SmartHome by HJE
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Timer.h"

extern uint32_t dotmatrix[8];

volatile int msec;
volatile char i, sec, min;


ISR(TIMER0_COMPA_vect){
	
   msec++;
   if(msec >= 1000){
	   msec = 0;
	   sec++;
	   if(sec >= 60){
		   sec = 0;
		   min++;
		   if(min >= 60)min = 0;
	   }
   }
	i++;
	if(i>7)i=0;
	Dotmatrix_shift_out(dotmatrix[i]);
}


void Timer0_init(void){
   TCCR0A |= 1 << WGM01;            //CTC Mode
   TCCR0B |= 1 << CS00 | 1 << CS01; //64분주
   OCR0A = 249;                // 1ms 주기
   TIMSK0 |= 1 << OCIE0A;           //OC0A 비교 매치 인터럽트 활성화
   return;
}
