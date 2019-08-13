/*
 * ADC.h
 *
 * Created: 2019-04-24 오후 2:18:19
 *  Author: user
 */ 


#ifndef ADC_H_
#define ADC_H_


int ADC_converting_value(char channel);
void ADC_init(void);
int Volt_to_cm(int value);
void check_Sharp(void); // 적외선 센서 측정

#endif /* ADC_H_ */