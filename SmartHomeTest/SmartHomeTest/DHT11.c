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


//char DHT11_data[4] = {0, };  //check_sum은 따로 저장X
 
char DHT11_main(void)
{
   char RH_integral, RH_decimal, Tmpr_integral, Tmpr_decimal;
   
   DHT11_init();
   
   UART0_init(9600);
   
   while(1)
   {
      
      DHT11_trigger(); //트리거
      
      data_input(); //입력전환
      dumi_read(); // 데이터 전송전에 시간 흘려보내기
      
	  
      RH_integral = rx_byte(); //데이터 값 저장
      RH_decimal  = rx_byte(); 
      Tmpr_integral = rx_byte();
      Tmpr_decimal = rx_byte();
	  
      rx_byte(); //check_sum
      
      //DHT11_init();
      data_output(); // data pin 출력 전환
      _delay_ms(1500); //적당한 시간을 주지않으면 초기화를 못해서 한번만 출력하고 안된당
	  
      printf("RH : %d.%d, Temperature : %d.%d \n", RH_integral, RH_decimal, Tmpr_integral, Tmpr_decimal); 
     // printf("RH : %d.%d, Temperature : %d.%d \n", DHT11_data[0], DHT11_data[1], DHT11_data[2], DHT11_data[3]);
      
   }
   
   return 0;
}

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
   
   /*
   for(int i = 0; i <4; i++)
   {
      for(int j = 0; j < 8; j++)
      {
         while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // low인동안 기다리기 // 50us
         _delay_us(30); // high 26-28 us : data "0"  // 데이터 값이 1이면 30us 지나도 여전히 high 이기때문에
         DHT11_data[i] <<= 1;
         if(DHT11_PIN & (1 << DHT11_PORT_num))//들어온게 1일때만 끝자리에 넣어준다
         {
            DHT11_data[i] |= 0b00000001; // RH_integral |= 0b00000001;
         }
         while(DHT11_PIN & (1 << DHT11_PORT_num)); //data 값이 1인 경우(high) 시간 보내주기
      }
      
   }
   */
   /*
   for(int i = 0; i < 8; i++)
   {
      while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // low인동안 기다리기 // 50us
      _delay_us(30); // high 26-28 us : data "0"  // 데이터 값이 1이면 30us 지나도 여전히 high 이기때문에
      RH_integral = RH_integral << 1;      // RH_integral <<= 1;
      if(DHT11_PIN & (1 << DHT11_PORT_num))//들어온게 1일때만 끝자리에 넣어준다
      {
         RH_integral++; // RH_integral |= 0b00000001;
      }
      while(DHT11_PIN & (1 << DHT11_PORT_num)); //data 값이 1인 경우(high) 시간 보내주기
   }
   for(int i = 0; i < 8; i++)
   {
      while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // low인동안 기다리기 // 50us
      _delay_us(30); // high 26-28 us : data "0"  // 데이터 값이 1이면 30us 지나도 여전히 high 이기때문에
      RH_decimal = RH_decimal << 1;
      if(DHT11_PIN & (1 << DHT11_PORT_num))//들어온게 1일때만 끝자리에 넣어준다
      {
         RH_decimal |= 0b00000001;
      }
      while(DHT11_PIN & (1 << DHT11_PORT_num)); //data 값이 1인 경우(high) 시간 보내주기
   }
   
   for(int i = 0; i < 8; i++)
   {
      while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // low인동안 기다리기 // 50us
      _delay_us(30); // high 26-28 us : data "0"  // 데이터 값이 1이면 30us 지나도 여전히 high 이기때문에
      Tmpr_integral = Tmpr_integral << 1;
      if(DHT11_PIN & (1 << DHT11_PORT_num))//들어온게 1일때만 끝자리에 넣어준다
      {
         Tmpr_integral |= 0b00000001;
      }
      while(DHT11_PIN & (1 << DHT11_PORT_num)); //data 값이 1인 경우(high) 시간 보내주기
   }
   
   for(int i = 0; i < 8; i++)
   {
      while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // low인동안 기다리기 // 50us
      _delay_us(30); // high 26-28 us : data "0"  // 데이터 값이 1이면 30us 지나도 여전히 high 이기때문에
      Tmpr_decimal = Tmpr_decimal << 1;
      if(DHT11_PIN & (1 << DHT11_PORT_num))//들어온게 1일때만 끝자리에 넣어준다
      {
         Tmpr_decimal |= 0b00000001;
      }
      while(DHT11_PIN & (1 << DHT11_PORT_num)); //data 값이 1인 경우(high) 시간 보내주기
   }
   */
   //check_num 8bit 버리기
   /*
   for(int i = 0; i < 8; i++)
   {
      while(!(DHT11_PIN & (1 << DHT11_PORT_num))); // low인동안 기다리기 // 50us
      while(DHT11_PIN & (1 << DHT11_PORT_num)); //data 값이 1인 경우(high) 시간 보내주기
   }
   */
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

char getTemperature(void){
	char RH_integral = 0, RH_decimal = 0, Tmpr_integral = 0, Tmpr_decimal = 0;
	DHT11_trigger(); //트리거
	
	data_input(); //입력전환
	dumi_read(); // 데이터 전송전에 시간 흘려보내기
	
	
	RH_integral = rx_byte(); //데이터 값 저장
	RH_decimal = rx_byte();
	Tmpr_integral = rx_byte();
	Tmpr_decimal = rx_byte();
	
	rx_byte(); //check_sum
	
	//DHT11_init();
	data_output(); // data pin 출력 전환
	//_delay_ms(1500);
	
	return Tmpr_integral;
}

char getHumi(void){ //습도
	char RH_integral = 0, RH_decimal = 0, Tmpr_integral = 0, Tmpr_decimal = 0;
	DHT11_trigger(); //트리거
	
	data_input(); //입력전환
	dumi_read(); // 데이터 전송전에 시간 흘려보내기
	
	
	RH_integral = rx_byte(); //데이터 값 저장
	RH_decimal = rx_byte();
	Tmpr_integral = rx_byte();
	Tmpr_decimal = rx_byte();
	
	rx_byte(); //check_sum
	
	//DHT11_init();
	data_output(); // data pin 출력 전환
	//_delay_ms(1500);
	
	return RH_integral;
}