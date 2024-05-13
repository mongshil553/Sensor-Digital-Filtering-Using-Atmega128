/*
 * Servo.c
 *
 * Created: 2024-05-13 오후 9:17:21
 *  Author: kijun
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "Overall.h"
#include "Servo.h"

void Calculate_Servo_Rotate_Angle(){
	//Marble_pos = ...
}

void Servo_Quick_Move(unsigned short val){
	Servo_pos = val;
	OCR1A = Servo_pos;
}

inline void Servo_Set_Target(unsigned short val){
	Servo_target = val;
	
	if(Servo_pos >= Servo_target) Servo_step = -1;
	else Servo_step = 1;
}

void Servo_Go_Home(){
	Servo_Set_Target(SERVO_HOME);
	Servo_Act();
}
void Servo_Go_Box(){
	Servo_Set_Target(SERVO_BOX);
	Servo_Act();
}
void Servo_Go_Marble(){
	Servo_Set_Target(Marble_pos);
	Servo_Act();
}

void Servo_Act(){
	//static unsigned short tmp = 0;
	
	unsigned short tmp = 0;
	
	while((Servo_pos != Servo_target) && Servo_Allowed){
		if(++tmp == Servo_increment_threshold){
			Servo_pos += Servo_step;
			OCR1A = Servo_pos;
			tmp = 0;
		}
		_delay_us(100);
	}
	
}