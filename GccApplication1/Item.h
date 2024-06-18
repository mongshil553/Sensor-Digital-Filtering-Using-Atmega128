/*
 * Item.h
 *
 * Created: 2024-05-27 오전 10:02:12
 *  Author: kijun
 */ 


#ifndef ITEM_H_
#define ITEM_H_

//Demux
#define ITEM_NONE		0x03	//0000 0011
#define ITEM_SERVO		0x02	//0000 0010
#define ITEM_SPEAKER	0x0E	//0000 1110
#define ITEM_LED_RED	0x09	//0000 1001
#define ITEM_LED_GREEN	0x0D	//0000 1101
#define ITEM_LED_BLUE	0x01	//0000 0001

void Select_Item(char item);

//================ Servo ===================//
#define Servo_MAX 520
#define Servo_MIN 230
#define SERVO_HOME 130
#define SERVO_BOX 620

//Servo Motor
volatile unsigned short Servo_target, tmp;
unsigned short Servo_pos, Servo_increment_threshold, Servo_step;
volatile char Servo_Allowed;

//void Servo_Set_Target(unsigned short val);
void Servo_Quick_Move(unsigned short val);
void Servo_Go_Home();
void Servo_Go_Box();
void Servo_Go_Marble();
void Servo_Goto(unsigned short pos);
void Servo_Act();
void Servo_Set_Target(unsigned short val);
void Servo_Set_Speed(char sp);
//Servo invrement_threshold: 0:Super Fast, 50:Very Slow
//Servo is using Timer1 for PWM

unsigned short Marble_pos;
void Calculate_Marble_pos();

//================ LED ===================//
#define LED_MAX 800.0
#define LED_MIN 200.0
#define CDS_MAX 400.0
#define CDS_MIN 100.0
#define get_led_val (cds_sensor_val > CDS_MAX)?LED_MAX:(cds_sensor_val<CDS_MIN)?LED_MIN:(LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*cds_sensor_val
int calc_led();
void RED_LED_On(unsigned int p);
void GREEN_LED_On(unsigned int p);
void BLUE_LED_On(unsigned int p);

//================ Buzzer ====================//
void Buzzer_on(int hz);
void Buzzer_off();

#endif /* ITEM_H_ */