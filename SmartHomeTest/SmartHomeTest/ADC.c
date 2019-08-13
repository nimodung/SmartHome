/*
 * ADC.c
 *
 * Created: 2019-04-24 오전 10:01:04
 *  Author: Kim Hee Ram
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "Uart.h"
#include "ADC.h"


void ADC_init(void)
{
	ADMUX |= (1 << REFS0); //Voltage Reference Selection : AVCC with external capacitor at AREF pin
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //ADC enable, prescaler 128분주
	
	return;
}

int ADC_converting_value(char channel)
{
	int value = 0;
	
	ADMUX &= 0b11110000; //Analog Channel Selection Bits 초기화
	ADMUX |= channel; //채널 설정
	_delay_us(200); //제대로 동작하는데 약간의 시간 필요
	
	ADCSRA |= (1 << ADSC); //Start Conversion //When the conversion is complete, it returns to zero
	while(!(ADCSRA  & (1 << ADIF))); //ADIF : conversing 이 완료되면 set //ADIF가 0인동안 기다려주기
	
	value = ADCL + ADCH * 256;
	value = (value + 1) * 500.0 / 1024 + 3; //+3 : 오차 보정 //5 * 100 / 1024 : 0 ~ 5V 사이의 값 , 소숫점을 위해서 *100
	
	return value;
}

int Volt_to_cm(int value)  //SHARP 2Y0A21 적외선 거리센서
{
	int cm = 0;
	cm = 20.0 / ((value / 100.0) - 0.3); //데이터 시트 보고 대충 계산한 값 //제품마다 다르기때문에 완전 정확하지않음
	
	return cm; 
}

void check_Sharp(void){
	int cm = 0;
	
	cm = Volt_to_cm(ADC_converting_value(2));
	// printf("%d\n", cm);
	if(cm <= 9) printf("bathroom : on\n");
	else printf("bathroom : off\n");
	
	
	return;
}