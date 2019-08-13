/*
 * Dot_matrix.c
 *
 * Created: 2019-04-08 오전 10:51:17
 *  Author: SmartHome by HJE
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Dot_matrix.h"
#include "Timer.h"


//은	
char last_data[8] = {0b11100111,0b11011011,0b11100111,0b11111111,0b10000001,0b11111111,0b11011111,0b11000011};
//주	
const char middle_data[8] = {0b11000111,0b11101111,0b11010111,0b11111111,0b10000011,0b11101111,0b11101111,0b11101111};
//집	
const char first_data[8] = {0b11000101,0b11101101,0b11010101,0b11111101,0b11011011,0b11000011,0b11011011,0b11000011};

uint32_t dotmatrix[8];
uint32_t dotmatrixdata[8];

int Dot_matrix_main(void)
{
	Myhome_dotmatrix_init_shiftR();
	
	for(int i = 0; i<8; i++)
	{
		dotmatrixdata[i] = 0xffffffff;
		dotmatrixdata[i] = (dotmatrixdata[i]<<8) | last_data[i];
		dotmatrixdata[i] = (dotmatrixdata[i]<<8) | middle_data[i];
		dotmatrixdata[i] = (dotmatrixdata[i]<<8) | first_data[i];
	}
	for(int i=0;i<8;i++)
	{
		dotmatrix[i] = 0x01000000 << i | dotmatrixdata[i];

	}
	
	Timer0_init();
	sei();
	
	while(1)
	{		
		Dotmatrix_flow();
	}
	return ;
}

void Myhome_dotmatrix_init_shiftR(void){								//도트매트릭스 이름변경 포트c로 바꾸기 , DDRC,PORTC
	Myhome_DATA_DDR |= 1 << RClk | 1 << SRClk | 1 << SER;
	Myhome_DATA_PORT |= 1 << RClk;
	for(int i=0; i<8; i++)
	{
		dotmatrix[i] = 0;
	}
	return;
}


void Dotmatrix_flow_left_out(void){
	for(int j=0;j<24;j++){
		for(int i=0;i<8;i++){
			dotmatrix[i] = (0x01000000 << i) | ((~(~dotmatrixdata[i] << j) & 0x00ffffff));
		}
		_delay_ms(100);
	}
}

void Dotmatrix_flow_left_in(void){
	for(int j=0;j<24;j++){
		for(int i=0;i<8;i++){
			dotmatrix[i] = (0x01000000 << i) | ((~(~dotmatrixdata[i] >> (23-j)) & 0x00ffffff));
		}
		_delay_ms(100);
	}
	
}


void Dotmatrix_flow(void) {
	for(int j=0;j<24;j++){
		for(int i=0;i<8;i++){
			dotmatrix[i] = (0x01000000 << i) | ((~(~dotmatrixdata[i] >> (23-j)) & 0x00ffffff));
		}
		_delay_ms(100);
	}
	for(int j=0;j<24;j++){
		for(int i=0;i<8;i++){
			dotmatrix[i] = (0x01000000 << i) | ((~(~dotmatrixdata[i] << j) & 0x00ffffff));
		}
		_delay_ms(100);
	}
}
void Dotmatrix_shift_out(uint32_t data){
	PORTC &= ~(1 << RClk);
	for(int i=0;i<32;i++){
		PORTC &= ~(1 << SRClk);
		PORTC &= ~(1 << SER);
		PORTC |= ((data>>i) & 1) << SER;
		PORTC |= 1 << SRClk;
	}
	PORTC |= 1 << RClk;
	return;
}

