/*
 * Uart.c
 *
 * Created: 2019-04-16 오전 10:09:17
 *  Author: Kim Hee Ram
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL //시스템 클락 속도
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Uart.h"


volatile char RX_data;
volatile char RX_cmd_count;

char buffer[COMMAND_MAX][BUFFER_MAX]; //BUFFER_MAX글자 안에서 명령어 입력
/*
	CR : \R 커서 맨 앞으로
	LF : \N 줄 바꿈
	=> ENTER
	터미널 프로그램마다 설정 다름
*/
ISR(USART_RX_vect) { /* USART Rx Complete */
	static char idx = 0, buf_idx = 0; //명령어 배열 index 제어 
	RX_data = UDR0;
	
	if(idx < BUFFER_MAX && RX_cmd_count <= COMMAND_MAX) {
		if(RX_data == '\r' || RX_data == '\n') { //명령이 들어옴
			buffer[buf_idx][idx] = 0; //or '\0' //문자열의 마지막에 null 넣어주기
			idx = 0; //다음 명령어를 받기위한 clear
			RX_cmd_count++;
			
			buf_idx++;
			buf_idx = buf_idx % COMMAND_MAX;
		}
		else buffer[buf_idx][idx++] = RX_data;
	}
	
}

void UART0_init(unsigned long baud)
{
	char i;
	FILE* fpStdio = fdevopen(TX0_char, NULL);
	
	UCSR0B |= ((1 << RXCIE0) | (1<< RXEN0) | (1<< TXEN0));	//송신, 수신 , 수신 인터럽트 활성화
	/*
		//0b10011000; //bit7 : Rx complete interrupt enable, bit 4 : receiver enable , bit 3 : transmitter enable
		//7~2 bit 제어 레지스터 , 0~1 bit 데이터 레지스터
		//UCSZ 0~2 : char size / 2 -> UCSR0B , 0,1 -> UCSR0C / ASCII 8bit /9bit 통신 UCSR0B의 0, 1bit 사용(알아서된당)
	*/
	UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));				//8bit 통신
	/*
		//UCSR0C
		//7,6bit 0 0 -> Asynchronous 
	*/
	UBRR0H = ((F_CPU/16/baud)-1) >> 8; //상위 8bit
	UBRR0L =  ((F_CPU/16/baud)-1) & 0xFF; //하위 8bit//0x00FF//0b0000000011111111 //상위 8bit는 자리가 없어서 &안해줘도 안들어감
	
	//UBRR0 = (F_CPU/16/baud)-1); //컴파일러가 알아서 상위 하위 비트 나눠서 넣어줌 . 위에 나눠서 넣은건 해보라고~
	/*
		// 9600은 우리가 설정해준 속도
		//Asynchronous Normal mode(U2X0 = 0으로 설정했음)
		//UBRR0L(low), UBRR0H(high) : 보호 레이트 레지스터 , 송수신 속도 맞춰주는
	*/
	i = UDR0; //수신 버퍼 비우기 // 한번 읽으면 비워지기 때문에 읽어주는 것 
			//송신, 수신 레지스터 이름을 같이 쓰는데 컴파일러가 알아서 구분해줌
	
	_delay_ms(1); //약간의 딜레이
	
	return;
}

//한 문자 송신 TX
void TX0_char(char data) //ascii
{
	//상태 읽기 / UCSR0A
	//UDRE0 : USART 데이터 레지스터가 비어있으면 1로 set / initial value : 1 
	while(!(UCSR0A & (1 << UDRE0))); //0b00100000 //0이면 기다려서 1이 될때까지 (준비가 될 때까지)
	
	UDR0 = data; //데이터 보낸 것
}

