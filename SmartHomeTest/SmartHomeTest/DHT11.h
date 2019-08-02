/*
 * DHT11.h
 *
 * Created: 2019-04-23 오후 1:53:44
 *  Author: user
 */ 


#ifndef DHT11_H_
#define DHT11_H_


#define DHT11_DDR DDRB
#define DHT11_PORT PORTB
#define DHT11_PIN PINB
#define DHT11_PORT_num 4

char DHT11_main(void);
void DHT11_trigger(void);
void data_input(void);
void data_output(void);
char rx_byte(void);
void DHT11_init(void);
void dumi_read(void);
char getTemperature(void); //온도 정수값 리턴 //호출 후 1.5초 후에 재호출 가능
char getHumi(void); //습도 정수값 리턴		//호출 후 1.5초 후에 재호출 가능

#endif /* DHT11_H_ */