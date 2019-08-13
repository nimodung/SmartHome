/*
 * DHT11.c
 *
 * Created: 2019-04-22 오후 3:59:55
 *  Author: Kim Hee Ram
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "Uart.h"
#include "DHT11.h"

extern volatile char doorAutoFlag;

void DHT11_init(void)
{
   DHT11_DDR |= 1 << DHT11_PORT_num;
   DHT11_PORT |= 1 << DHT11_PORT_num;
   _delay_ms(3000);
   
   return;
}

char rx_byte(void)
{
   char data = 0;
   int count = 0;
   for(int i = 0; i < 8; i++)
   {
	   //
      while(!(DHT11_PIN & (1 << DHT11_PORT_num))) // low인동안 기다리기 // 50us
	  {
		  _delay_us(1);
		  count++;
		  if(count >= 200) break;
	  }
	  if(count >= 200) break;
	  count = 0;
     
	  _delay_us(30); // high 26-28 us : data "0"  // 데이터 값이 1이면 30us 지나도 여전히 high 이기때문에
     
	  data = data << 1;      // RH_integral <<= 1;
      if(DHT11_PIN & (1 << DHT11_PORT_num))//들어온게 1일때만 끝자리에 넣어준다
      {
         data |= 0b00000001;
      }
	  
      while(DHT11_PIN & (1 << DHT11_PORT_num)) //data 값이 1인 경우(high) 시간 보내주기
	  {
		  _delay_us(1);
		  count++;
		  if(count >= 200) break;
	  }
	  if(count >= 200) break;
	  count = 0;
   }
  
   if(count >= 200) return 255;
   else return data;
}

void data_output(void)//data 핀 아웃풋 설정
{
   //대기 상태로 만들어주기
   DHT11_DDR |= (1 << DHT11_PORT_num);   //출력 전환
   DHT11_PORT |= 1 << DHT11_PORT_num;
   
   //시작하기 전의 high의 길이가 충분해야함
   return;   
}

void data_input(void)
{
   //DHT11_PORT &= ~(1 << DHT11_PORT4); //내부 PULL UP 없이
   DHT11_DDR &= ~(1 << DHT11_PORT_num);  //입력 전환
  
   return;
}

void dumi_read(void) //비트 읽고 흘려보내기
{
	
	
	while(DHT11_PIN & (1 << DHT11_PORT_num));  //4번 핀의 값이 1인 동안 기다리기
	while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // 0인 동안, low인동안 80 마이크로 s 기다리는거
	while(DHT11_PIN & (1 << DHT11_PORT_num)); //1인 동안, high 인동안 80 마이크로s 기다리기
	
	// => 버리는 신호들 , 대기/ 5.2 MCU Sends out Start Signal to DHT
	
	return;
}

void DHT11_trigger(void)
{
   DHT11_PORT &= ~(1 << DHT11_PORT_num); //Trigger신호 , low
   
   _delay_ms(18);
   
   DHT11_PORT |= 1 << DHT11_PORT_num; //high
   
   _delay_us(5); //20~40이라 써있지만 실제로는 더 빨리 응답하기때문에 조금만 줌
   
   
   return;
}

void check_DHT11(void) {
	char RH_integral, RH_decimal, Tmpr_integral, Tmpr_decimal;
	
	int lux = ADC_converting_value(1);
	
	if(doorAutoFlag) {
		if(lux <= 200) printf("lux : off\n");
		else printf("lux : on\n");
	}
	
	
	DHT11_trigger(); //트리거
	data_input(); //입력 전환
	dumi_read(); // 데이터 전송 전에 시간 흘려보내기
	
	
	RH_integral = rx_byte(); //데이터 값 저장
	RH_decimal  = rx_byte();
	Tmpr_integral = rx_byte();
	Tmpr_decimal = rx_byte();
	
	rx_byte(); //check_sum
	
	data_output(); // data pin 출력 전환

	printf("env : humi %d.%d temper %d.%d lux %d\n", RH_integral, RH_decimal, Tmpr_integral, Tmpr_decimal, lux);
	
	
	
	return;
	
	
}
