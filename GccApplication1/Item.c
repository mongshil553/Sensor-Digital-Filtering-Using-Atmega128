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

short table_y[12] = {14, 19, 28, 48, 58, 77, 81, 94, 100, 114, 122, 133};
short table_o[12] = {290, 310, 330, 350, 370, 390, 410, 430, 450, 470, 490, 510};

void Select_Item(char item){
	PORTC = (PORTC & 0xF0) | item;
	_delay_ms(100);
}

//================ Servo ===================//

int Calculate_Servo_Rotate_Angle(short pos){
	//Marble_pos = ...
	char target = table_o[0];
	for(int i = 1; i<12;i++){
		if(pos > table_y[i]) return table_o[i];
	}
	return table_o[11];
}
void Calculate_Marble_pos(){
	//.........
}


void Servo_Quick_Move(unsigned short val){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = Servo_pos;
	Servo_pos = val;
	OCR1A = Servo_pos;
	Select_Item(ITEM_SERVO);
}

inline void Servo_Set_Target(unsigned short val){
	Servo_target = val;
	
	if(Servo_pos >= Servo_target) Servo_step = -1;
	else Servo_step = 1;
}

void Servo_Go_Home(){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = Servo_pos;
	Servo_Set_Target(SERVO_HOME);
	Select_Item(ITEM_SERVO);
	Servo_Act();
}
void Servo_Go_Box(){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = Servo_pos;
	Servo_Set_Target(SERVO_BOX);
	Select_Item(ITEM_SERVO);
	Servo_Act();
}
void Servo_Go_Marble(){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = Servo_pos;
	Servo_Set_Target(Marble_pos);
	Select_Item(ITEM_SERVO);
	Servo_Act();
}
void Servo_Goto(unsigned short pos){
	Select_Item(ITEM_NONE);
	ICR1 = 4999;
	OCR1A = Servo_pos;
	Servo_Set_Target(pos);
	Select_Item(ITEM_SERVO);
	Servo_Act();
}

void Servo_Act(){
	//static unsigned short tmp = 0;
	
	unsigned short tmp = 0;
	
	while(Servo_pos != Servo_target){// && !Fire_Detected){
		if(Servo_Allowed){
			if(++tmp >= Servo_increment_threshold){
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

char calc_speed(){
	//if(temp_sensor_val < Thermister_MIN){
	//	return Servo_MIN_Speed;
	//}
	//else if(temp_sensor_val > Thermister_MAX){
	//	return Servo_MAX_Speed;
	//}
	//return (Servo_MAX_Speed-Servo_MIN_Speed)/(Thermister_MAX-Thermister_MIN)*(temp_sensor_val-Thermister_MAX)*1.0+Servo_MAX_Speed;
	return (temp_sensor_val > Thermister_MAX)?Servo_MAX_Speed:(temp_sensor_val<Thermister_MIN)?Servo_MIN_Speed:(Servo_MAX_Speed-Servo_MIN_Speed)/(Thermister_MAX-Thermister_MIN)*(temp_sensor_val-Thermister_MIN)+Servo_MIN_Speed;
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
	
	return (LUX > CDS_MAX)?LED_MAX:(LUX<CDS_MIN)?LED_MIN:(LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*(LUX-LED_MIN)+CDS_MIN;
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

int calc_hz(){
	return (fire_sensor_val > Fire_MAX)?Buzzer_MAX:(fire_sensor_val<Fire_MAX)?Buzzer_MIN:(Buzzer_MAX-Buzzer_MIN)/(Fire_MAX-Fire_MIN)*(fire_sensor_val-Fire_MIN)+Buzzer_MIN;
}

//=========== pressuer =======//
int calc_force(){
	if(pressure_sensor_val <= 10)
		return 0;
	else
		return pow(2.718, (pressure_sensor_val*5.0/1024.0-0.3456)/0.6935);
}