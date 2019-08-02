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


int ADC_main(void)
{
	int value = 0;
	int distance = 0;
	ADC_init();
	_delay_us(200); //ADC 준비 딜레이
	
	UART0_init(9600);
	
	while(1)
	{
		/*
		//가변 저항
		value = ADC_converting_value(0);
		//TX0_4Digit(value);
		//TX0_char('\n');
		printf("Potentionmeter : %d.%d%d \t", value/100%10, value/10%10, value%10); 
				
		//CDS
		value = ADC_converting_value(1);
		printf("CDS value : %d.%d%d \n", value/100%10, value/10%10, value%10);
		*/
		
		//적외선 거리 측성
		value = ADC_converting_value(2); //전압에 *100 한 값
		distance = Volt_to_cm(value);
		printf("Distance : %d.%d%d \n", distance/100%10, distance/10%10, distance%10);
		
		_delay_ms(500);
		
	}
	
	return 0;
}

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
	//value = 0;
	//ADC 값 value에 저장하기
		/* //컴파일러 문제인듯 // 수식에는 문제 없으나 변경되는 값이 출력안됨
			value = ADCH;
			value <<= 8;
			value += ADCL;  //value |= ADCL; 
		*/
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

int Volt_to_temperature(int value) 
{
	int temp = 0;
	//temp = 2.5 * value + 1.5; //임의로 대충 적은것
	return temp;
}