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

