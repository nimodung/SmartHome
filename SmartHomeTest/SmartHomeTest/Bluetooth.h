/*
 * Bluetooth.h
 *
 * Created: 2019-05-14 오후 4:17:03
 *  Author: SmartHome by HJE
 */ 


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_


int Bluetooth_main(void);
void Bluetooth_init(void);
void setName(char *name);
void setPIN(char *pin);
void connectTest(void);


#endif /* BLUETOOTH_H_ */
