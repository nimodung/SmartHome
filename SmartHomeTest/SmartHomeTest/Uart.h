/*
 * Uart.h
 *
 * Created: 2019-04-17 오전 9:49:14
 *  Author: user
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <string.h>

#define BUFFER_MAX 50 // 한 명령어의 최대 글자 수
#define COMMAND_MAX 10 //명령어 갯수
//명령어 처리 중에 다른 딜레이가 생겨 명령어를 놓치는 일이 없게 하기위해 명령어 저장해서 실행


int Uart_main(void);
void TX0_char(char data);
void UART0_init(unsigned long baud);
void TX0_string(char *string);
void TX0_4Digit(int data);


#endif /* UART_H_ */