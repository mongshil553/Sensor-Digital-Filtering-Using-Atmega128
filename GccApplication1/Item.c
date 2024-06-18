/*
 * Item.c
 *
 * Created: 2024-05-27 오전 10:21:15
 *  Author: kijun
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "Item.h"
#include "Sensors.h"

void Select_Item(char item){
	PORTC = (PORTC & 0xF0) | item;
	_delay_ms(100);
}

//================ Servo ===================//

void Calculate_Servo_Rotate_Angle(){
	//Marble_pos = ...
}
void Calculate_Marble_pos(){
	//.........
}


void Servo_Quick_Move(unsigned short val){
	Select_Item(ITEM_SERVO);
	Servo_pos = val;
	OCR1A = Servo_pos;
}

inline void Servo_Set_Target(unsigned short val){
	Select_Item(ITEM_SERVO);
	
	Servo_target = val;
	
	if(Servo_pos >= Servo_target) Servo_step = -1;
	else Servo_step = 1;
}

void Servo_Go_Home(){
	OCR1A = Servo_pos;
	Select_Item(ITEM_SERVO);
	Servo_Set_Target(SERVO_HOME);
	Servo_Act();
}
void Servo_Go_Box(){
	OCR1A = Servo_pos;
	Select_Item(ITEM_SERVO);
	Servo_Set_Target(SERVO_BOX);
	Servo_Act();
}
void Servo_Go_Marble(){
	OCR1A = Servo_pos;
	Select_Item(ITEM_SERVO);
	Servo_Set_Target(Marble_pos);
	Servo_Act();
}
void Servo_Goto(unsigned short pos){
	OCR1A = Servo_pos;
	Select_Item(ITEM_SERVO);
	Servo_Set_Target(pos);
	Servo_Act();
}

void Servo_Act(){
	//static unsigned short tmp = 0;
	
	unsigned short tmp = 0;
	
	while(Servo_pos != Servo_target){
		if(Servo_Allowed){
			if(++tmp == Servo_increment_threshold){
				Servo_pos += Servo_step;
				OCR1A = Servo_pos;
				tmp = 0;
			}
		}
		_delay_us(100);
	}
	
}

void Servo_Set_Speed(char sp){
	Servo_increment_threshold = sp;
}

//================ LED ====================//
void RED_LED_On(unsigned int p){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = p;
	Select_Item(ITEM_LED_RED);
}
void GREEN_LED_On(unsigned int p){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = p;
	Select_Item(ITEM_LED_GREEN);
}
void BLUE_LED_On(unsigned int p){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = p;
	Select_Item(ITEM_LED_BLUE);
}

int calc_led(){
	double cds=(4700.0 * 1023)/cds_sensor_val-4700.0;
	double LUX = pow(10, 1-(log(cds)-log(40000))/0.8);
	
	return (LUX > CDS_MAX)?LED_MAX:(LUX<CDS_MIN)?LED_MIN:(LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*LUX;
}
//=============== Buzzer =================//
void Buzzer_on(int key){
	Select_Item(ITEM_NONE);
	ICR1 = key;
	OCR1A = ICR1 / 2;
	Select_Item(ITEM_SPEAKER);
}
void Buzzer_off(){
	Select_Item(ITEM_NONE);
}