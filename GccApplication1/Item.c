#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "Item.h"
#include "Sensors.h"

short table_y[12] = {14, 19, 28, 48, 58, 77, 81, 94, 100, 114, 122, 133}; // marble y position
short table_o[12] = {290, 310, 330, 350, 370, 390, 410, 430, 450, 470, 490, 510}; //servo rotate OCR values accordingly

void Select_Item(char item){
	//next_item = item;
	//while(cur_item != next_item);
	PORTC = (PORTC & 0xF0) | item;
	_delay_ms(100);
}

//================ Servo ===================//

int Calculate_Servo_Rotate_Angle(short pos){
	//Marble_pos = ...
	//char target = table_o[0];
	for(int i = 11; i>=0;i--){
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
	
	while(Servo_pos != Servo_target && !Fire_Detected){
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
void Set_LED(unsigned int p){
	OCR1A = p;
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



//=========== 7 Segment =======//
void segment(char prox){
	PORTC = (PORTC & 0x0F) | prox;
}