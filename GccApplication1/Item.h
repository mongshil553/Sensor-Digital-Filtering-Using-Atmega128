/*
 * Item.h
 *
 * Created: 2024-05-27 오전 10:02:12
 *  Author: kijun
 */ 


#ifndef ITEM_H_
#define ITEM_H_

//Demux
#define ITEM_NONE		0x03	//0000 0111
#define ITEM_SERVO		0x02	//0000 0010
#define ITEM_SPEAKER	0x0E	//0000 1110
#define ITEM_LED_RED	0x05	//0000 1001
#define ITEM_LED_GREEN	0x0D	//0000 1101
#define ITEM_LED_BLUE	0x01	//0000 0001

void Select_Item(char item);
char cur_item, next_item;

//================ Servo ===================//
#define SERVO_MAX_POS 580
#define SERVO_MIN_POS 290
#define SERVO_HOME 620
#define SERVO_BOX 130
#define Servo_MAX_Speed 20.0
#define Servo_MIN_Speed 130.0

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
int Calculate_Servo_Rotate_Angle(short pos);

//================ LED ===================//
#define LED_MAX 3000.0
#define LED_MIN 200.0
#define CDS_MAX 2000.0
#define CDS_MIN 100.0
#define get_led_val (cds_sensor_val > CDS_MAX)?LED_MAX:(cds_sensor_val<CDS_MIN)?LED_MIN:(LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*cds_sensor_val

void RED_LED_On(unsigned int p);
void GREEN_LED_On(unsigned int p);
void BLUE_LED_On(unsigned int p);
void Set_LED(unsigned int p);

//================ Buzzer ====================//
#define Buzzer_MAX 200.0
#define Buzzer_MIN 50.0
void Buzzer_on(int hz);
void Buzzer_off();


//================ Thermister ================//
#define Thermister_MAX 400
#define Thermister_MIN 100

//================= Pressure ==================//


//================= Fire ======================//
#define Fire_MAX 600.0
#define Fire_MIN 50.0

//================= 7 Segment =================//
#define prox1 0x80 //10000000
#define prox2 0xC0 //11000000
#define prox3 0xE0 //11100000
#define prox4 0xF0 //11110000

void segment(char prox);

#endif /* ITEM_H_ */