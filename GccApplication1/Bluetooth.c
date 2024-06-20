/*
 * Bluetooth.c
 *
 * Created: 2024-05-22 오전 1:11:43
 *  Author: kijun
 */ 

#include "Bluetooth.h"
#include "Marble.h"
#include <avr/io.h>

void init_BT(){
	UCSR1A = 0x00;
	UCSR1B = 0x18;
	UCSR1C = 0x06;
	
	UBRR1H = 0x00;
	UBRR1L = 103;
	
	#ifdef USE_BLUETOOTH_INTERRUPT
	UCSR1B |= 0x80; //Enable Interrupt
	#endif
}

char BT_Receive(){
	
	static char step = 0;
	static char tmp = 0;
	
	if(!(UCSR1A&(1<<RXC1))) return 0x00;
	
	rdata = UDR1;
	
	switch(step){
		case 1:
		if(rdata == '.') {
			step = 0;
			BT_send(1);
			//ElectroMagnet_On();
			return 0x00;
		}
		else marble.color = rdata - '0';
		break;
		
		case 2:
		if(rdata == '.') {
			step = 0;
			BT_send(1);
			//ElectroMagnet_On();
			return 0x00;
		}
		else marble.posX = marble.posX * 10 + rdata - '0';
		break;
		
		case 3:
		if(rdata == '.') {
			step = 0;
			BT_send(1);
			//ElectroMagnet_Off();
			return 0x01;
		}
		else marble.posY = marble.posY * 10 + rdata -'0';
		break;
	}
	
	if(rdata == '#') {
		marble.color = 0;
		step = 1;
	}
	else if(rdata == '!'){
		marble.posX = 0;
		step = 2;
	}
	else if(rdata == '*'){
		marble.posY = 0;
		step = 3;
	}
	
	BT_send(1);

	return 0x00;
}

void BT_send(char msg){
	//send msg
	while(!(UCSR1A & (1<<UDRE))); //Wait for buffer to clear
	UDR1 = msg; //send msg
}