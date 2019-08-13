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

void DHT11_init(void);
void check_DHT11(void); // 온,습도 측정
void data_input(void);
void data_output(void);
char rx_byte(void);
void DHT11_trigger(void);
void dumi_read(void);


#endif /* DHT11_H_ */