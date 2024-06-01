/*
 * Servo.h
 *
 * Created: 2024-05-13 오후 9:15:54
 *  Author: kijun
 */ 

#ifndef SERVO_H_
#define SERVO_H_

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

#endif /* SERVO_H_ */