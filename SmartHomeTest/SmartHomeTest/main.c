/*
 * SmartHomeTest.c
 *
 * Created: 2019-06-25 오전 10:17:36
 * Author : KIM HEE RAM
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Uart.h"
#include "Bluetooth.h"
#include "Timer.h"
#include "DHT11.h"
#include "ADC.h"

#define LED_ROOM 2
#define LED_LIVING 4
#define LED_KITCHEN 5
#define LED_BATH 6
#define LED_DOOR 7

#define LED_WHOLE 1
#define LED_WHOLE_AUTO 2
#define LED_WHOLE_AUTO_BATH 3
#define LED_WHOLE_AUTO_DOOR 4

void SmartHome_Init(void); // 초기화
void WholeLED_On(char mode); // led 전체를 mode에 따라 on
void WholeLED_Off(char mode); // led 전체를 mode에 따라 on


extern volatile char RX_cmd_count;
extern char buffer[COMMAND_MAX][BUFFER_MAX]; //BUFFER_MAX글자 안에서 명령어 입력

extern volatile char check_DHT11_flag, delay_flag;
volatile char doorAutoFlag = 1, bathroomAutoFlag = 1, doorOpenFalg = 0;
int lux;

int main(void)
{
	char cmd_idx = 0;
	
	SmartHome_Init();
	
	while (1) 
    {
		
		if(RX_cmd_count) {
			RX_cmd_count--;
			
			// 조명 관리
			if(!strcmp(buffer[cmd_idx], "led room on")) { //strcmp() : 비교해서 같으면 return 0
				PORTD |= 1 << LED_ROOM;
				printf("led : room on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led room off")) {
				PORTD &= ~(1 << LED_ROOM);
				printf("led : room off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led living on")) {
				PORTD |= 1 << LED_LIVING;
				printf("led : living on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led living off")) {
				PORTD &= ~(1 << LED_LIVING);
				printf("led : living off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led kitchen on")) {
				PORTD |= 1 << LED_KITCHEN;
				printf("led : kitchen on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led kitchen off")) {
				PORTD &= ~(1 << LED_KITCHEN);
				printf("led : kitchen off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led bathroom on")) {
				PORTD |= 1 << LED_BATH;
				printf("led : bathroom on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led bathroom off")) {
				PORTD &= ~(1 << LED_BATH);
				printf("led : bathroom off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led door on")) {
				PORTD |= 1 << LED_DOOR;
				printf("led : door on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led door off")) {
				PORTD &= ~(1 << LED_DOOR);
				printf("led : door off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole on")) {
				WholeLED_On(LED_WHOLE);
				printf("led : whole on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole off")) {
				WholeLED_Off(LED_WHOLE);
				printf("led : whole off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole on auto")) {
				WholeLED_On(LED_WHOLE_AUTO);
				printf("led : whole on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole off auto")) {
				WholeLED_Off(LED_WHOLE_AUTO);
				printf("led : whole off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole on bathroom")) {
				WholeLED_On(LED_WHOLE_AUTO_BATH);
				printf("led : wholeb on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole off bathroom")) {
				WholeLED_Off(LED_WHOLE_AUTO_BATH);
				printf("led : wholeb off\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole on door")) {
				WholeLED_On(LED_WHOLE_AUTO_DOOR);
				printf("led : wholed on\n");
			}
			else if(!strcmp(buffer[cmd_idx], "led whole off door")) {
				WholeLED_Off(LED_WHOLE_AUTO_DOOR);
				printf("led : wholed off\n");
			}
			
			// 환경 관리 
			else if(!strcmp(buffer[cmd_idx], "fan on")) {
				OCR2B = 200;
			}
			else if(!strcmp(buffer[cmd_idx], "fan off")) {
				OCR2B = 0;
			}
			else if(!strcmp(buffer[cmd_idx], "door open")){
				OCR1A = 2900;
			}
			else if(!strcmp(buffer[cmd_idx], "door close")){
				OCR1A = 4900;
			}
			else if(!strcmp(buffer[cmd_idx], "window open")){
				OCR1B = 1300;
			}
			
			else if(!strcmp(buffer[cmd_idx], "window close")){
				OCR1B = 4900;
				
			}
			
			// LED AUTO 상태 변경
			else if(!strcmp(buffer[cmd_idx], "doorLedAutoStatetrue")){
				doorAutoFlag = 1;
			}
			else if(!strcmp(buffer[cmd_idx], "doorLedAutoStatefalse")){
				doorAutoFlag = 0;
			}
			else if(!strcmp(buffer[cmd_idx], "bathroomLedAutoStatetrue")){
				bathroomAutoFlag = 1;
			}
			else if(!strcmp(buffer[cmd_idx], "bathroomLedAutoStatefalse")){
				bathroomAutoFlag = 0;
			}
			
			cmd_idx++;
			cmd_idx = cmd_idx % COMMAND_MAX;
		}
		
	
		if(check_DHT11_flag) {
			check_DHT11_flag = 0;
			check_DHT11();
			if(bathroomAutoFlag){check_Sharp();}
		}
		
	}
}

void WholeLED_On(char mode){
	
	switch(mode){
		case LED_WHOLE:
			PORTD |= 1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN | 1 << LED_BATH | 1 << LED_DOOR;
			break;
		case LED_WHOLE_AUTO:
			PORTD |= 1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN;
			break;
		case LED_WHOLE_AUTO_BATH:
			PORTD |= 1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN | 1 << LED_DOOR;
			break;
		case LED_WHOLE_AUTO_DOOR:
			PORTD |= 1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN | 1 << LED_BATH;
			break;
	}
	return;
}

void WholeLED_Off(char mode){
	
	switch(mode){
		case LED_WHOLE:
			PORTD &= ~(1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN | 1 << LED_BATH | 1 << LED_DOOR);
			break;
		case LED_WHOLE_AUTO:
			PORTD &= ~(1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN);
			break;
		case LED_WHOLE_AUTO_BATH:
			PORTD &= ~(1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN | 1 << LED_DOOR);
			break;
		case LED_WHOLE_AUTO_DOOR:
			PORTD &= ~(1 << LED_ROOM | 1 << LED_LIVING | 1 << LED_KITCHEN | 1 << LED_BATH);
			break;
	}
	return;
}

void SmartHome_Init(void){
	DDRD |= (1 << PORTD2) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);
	PORTD &= ~((1 << PORTD2) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7));
	DDRB |= 1 << PORTB5;
	PORTB &= ~(1<< PORTB5);
	Bluetooth_init();
		
	Timer0_init();
	Timer1_init_fast_PWM_outA(); // OCR1A : DOOR, OCR1B : WINDOW
	Timer2_init_fast_PWM_outA_Motor(); // OCR2B : FAN
		
	DHT11_init(); // 온,습도 측정
	ADC_init(); // 적외선 센서, 조도 센서
		

		
	sei();
}



