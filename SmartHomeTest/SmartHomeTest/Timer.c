/*
 * Timer.c
 *
 * Created: 2019-04-10 오전 11:04:02
 *  Author: user
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h> // interrupt 사용하기 위함

#include "Timer.h"


//전역 변수는 초기화 안하면 0으로 초기화된다
volatile char check_DHT11_flag = 1, delay_flag = 1;	
volatile int check_DHT11_msec = 0, delay_msec = 0, music_i, long_key_msec = 0;				
volatile char i = 0, sec = 0, min = 0, hour = 0, cook_sec = 0, cook_min = 0, stop_sec = 0, stop_min = 0, alarm_min, alarm_hour;			
															//volatile : 워킹레지스터에 선언하지말고 RAM에다가 할당하라는 의미(휘발성)
															//전역변수 선언시에 많이 사용
volatile int beat, interval;																	

//isr은 가급적 빨리 실행해야 되기 때문에 딜레이 x, 함수호출x ...

ISR(TIMER0_COMPA_vect)						//interrupt 걸리면 실행하는 내용 //Interrupt Service Routine
{	
	check_DHT11_msec++;
//	if(delay_flag) delay_msec++;
	
	if(check_DHT11_msec >= 1500) {
		check_DHT11_msec = 0;
		check_DHT11_flag = 1;
	}

/*
	if(delay_msec >= 1000) {
		delay_msec = 0;
		delay_flag = 0;
	}	
*/

	/*
	
	//speaker
	if(speakout_flag) {
		if(!interval) {
			if(next_beat_flag) {
				next_beat_flag = 0;
				
				if(san_rabit_bell_tune[music_i]) OCR1A = F_CPU / 2 / 8 / san_rabit_bell_tune[music_i];
				else OCR1A = 0;
				beat = san_rabit_bell_beat[music_i];
				music_i++;
				if(san_rabit_bell_tune[music_i] == '/0')
				{
					speakout_flag = 0;
					OCR1A = 0;
				}
			}
		
			beat--;
			if(beat == 0){
				
				next_beat_flag = 1;
				interval = 10;		
			}
		}
		else {
			OCR1A = 0;
			interval--;			
		}
		
	}
	
	
	msec++;
	
	//CLOCK
	if(msec >= 1000)
	{
		msec = 0;
		sec++; //fnd 출력이 바뀌는 시점
		if(sec >= 60)
		{
			sec = 0;
			min++;
			if(min >= 60) {
				min = 0;
				hour++;
				if(hour >= 12) hour = 0;
			}
			
			
		}
		//if(w_flag)
		FND_clock(sec, min);
	}
	
*/
	
	/*//FND를 실제로 출력시키는 
	i++;
 	
 	if(i >=4) i = 0;
	

	//끄고 데이터 주고 끄고
	FND_COM_PORT &= 0b00001111; //상위 4비트 끄고
	FND_shift_out(FND[i]);
	FND_COM_PORT |= FND4digit_digit[i];//< FND_shift_out보다 먼저하면 이전 데이터가 출력돼서 정확한 출력이 나오지 않음
	*/
} 




void Timer0_init(void)
{
	//타이머 초기화
	TCCR0A |= 1 << WGM01;					//0b00000010;  //CTC Mode
	TCCR0B |= (1 << CS00 | 1 << CS01);		//0b00000011;  //64분주 사용(하위 3bit : 011)
	OCR0A = 249;							//비교할 값 => (16,000,000 / 64) / 1000 = 250 (0 ~ "249")			//1ms 주기
	TIMSK0 |= 1 << OCIE0A;                  //0b00000010; //어떤 interrupt 걸리게 할건지 정하는	// OC0A 비교 매치 인터럽트 활성화
	
	return;
}

void Timer0_HC_SR04_init(void)
{
	TCCR0A |= 1 << WGM01;					//CTC 모드
	TCCR0B |= (1 << CS02) | (1 << CS00);	//1024분주
	OCR0A = 255;							//
	
	return;
}

void Timer0_init_CTC_outA(void) //A 출력을 쓰기위한 초기화 함수
{
	DDRD |= 1 << PORTD6;
	TCCR0A |= 1<< WGM01 | 1 <<COM0A0;
	TCCR0B |= 1 <<  CS00 | 1 <<CS01;
	
	OCR0A = 30; //4000Hz
	//OCR0A = 63; // 2000Hz
	//OCR0A = 124;							//(250 / 2) - 1 //한 주기를 1msec
	//OCR0A = 249;							//한 주기 2msec //500Hz
	//250Hz를 만들고싶다면? => OCR0A를 늘리면 오버플로우(8bit) => 분주를 늘린다
	//CS0x (분주)랑 OCR값을 이용해서 주기, 주파수 조정
	TIMSK0 |= 1 << OCIE0A;                  //0b00000010; //어떤 interrupt 걸리게 할건지 정하는	// OC0A 비교 매치 인터럽트 활성화
	
	return;
}

void Timer1_init_CTC_outA(void) //A 출력을 쓰기위한 초기화 함수
{
	DDRB |= 1 << PORTB1;
	TCCR1A |= 1 << COM1A0; //TOGGLE
	TCCR1B |= 1 << WGM12 | 1 <<  CS11;  //CTC 모드//64분주
	OCR1A = 0; // 소리 끄기
	
	//OCR0A = 30; //4000Hz
	//OCR0A = 63; // 2000Hz
	//OCR0A = 124;							//(250 / 2) - 1 //한 주기를 1msec
	//OCR0A = 249;							//한 주기 2msec //500Hz
	//250Hz를 만들고싶다면? => OCR0A를 늘리면 오버플로우(8bit) => 분주를 늘린다
	//CS0x (분주)랑 OCR값을 이용해서 주기, 주파수 조정
	//TIMSK0 |= 1 << OCIE0A;                  //0b00000010; //어떤 interrupt 걸리게 할건지 정하는	// OC0A 비교 매치 인터럽트 활성화
	
	return;
}

void Timer2_init_fast_PWM_outA_Motor(void){
	//DDRD |= 1 << DDD3; //DDD <- 레지스터의 비트 이름 //PORTDx랑 define 값이 같음
	DDRB |= 1 << DDD3;
	DDRD |= 1 << DDD3;
	
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	//TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	//COM2B1 , COM2B0 : 11 이면 Set OC2B on Compare Match, clear OC2B at BOTTOM, (inverting mode).
	//->match가 되면 1로 되는것. ocr값을 작게 주면 high가 길다. 반대라서 헷갈리니까 10으로 준것
	TCCR2B |= (1 << CS20); //모터 구동 시에는 CS20
	
	OCR2A = 0; //BOTTOM
	OCR2B = 0;
	//따로 ICR 없음 //0~255까지가 100프로
}

