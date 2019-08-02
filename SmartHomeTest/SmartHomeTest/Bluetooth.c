/*
 * Bluetooth.c
 *
 * Created: 2019-05-14 오후 2:47:19
 *  Author: KIM HEE RAM
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Bluetooth.h"
#include "Uart.h"





void Bluetooth_init(void) {
	UART0_init(9600);
	
	return;
}

void setName(char *name){
	TX0_string("AT+NAME");
	TX0_string(name);
	
	return;
}

void setPIN(char *pin) { //네 자리 숫자
	TX0_string("AT+PIN");
	TX0_string(pin);
	return;
}

 void connectTest(void) {
	 //TX0_string("AT");
	 printf("AT");
	 return;
 }