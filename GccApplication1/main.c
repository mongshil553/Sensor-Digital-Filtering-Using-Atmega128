/*
 * GccApplication1.c
 *
 * Created: 2024-05-04 오전 12:07:19
 * Author : kijun
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void sys_init();

//Marble
struct MarbleClass{
	volatile unsigned char color;
	volatile unsigned short posX, posY;
};
volatile struct MarbleClass marble;

//Servo Motor
static volatile unsigned char Motor_Rotate_Angle, tmp;
void Calculate_Servo_Rotate_Angle();
void Servo_Home();
void Servo_Move();
void Servo_Drop_Marble();
//Servo is using Timer2 for PWM

//Pressure Sensor
volatile char pressure_sensor_val;
//외부 변수 써서 고주파를 차단해야되면 LPF, MAF이용
//예를 들어, 이전 값 저장하는 변수

//CdS Sensor
volatile char cds_sensor_val;

//Temperature sensor
volatile char temp_sensor_val;

//Shock Sensor
volatile char shk_sensor_val;
volatile char shk_detected;

//PSD Sensor
volatile char psd_sensor_val;

//LED PWM Value
volatile unsigned int led_pwm_value;
void LED_Set();
//Use Timer2 for PWM when not using Servo

//MAIN
volatile char state = 0x01;
volatile char STOP = 0x00;

//블루투스 통신 RX PD3, TX PD2
void init_serial(void);
bool BT_send(char msg);
static volatile char rdata = 0; //read buffer from BT

void pin_init();
void init();

int main(void)
{
	pin_init(); //Pin Setup
	init();		//Interrupt, Timer, Register
	
	init_serial();
	
	marble.color = 0x00;
	marble.posX = -1;
	marble.posY = -1;
	
	Servo_Home(); //Move Servo to Home Position
	
	//Need to Check Connection with Server
	//If not connected, try for few seconds and if failed connection, do other actions
	//Thus, there are two cases. 1, Bluetooth is connected, 2, Bluetooth is not connected
	//This is for just in case at presentation, Bluetooth fails
	
	volatile short i;
	
    while (1) {
		switch(state){
			case 0b00000001:					//not started
				if(pressure_sensor_val >= 50)	//pressure threshold is 50(just guessing)
					state <<= 1;				//Change state
				
			case 0b00000010:	//Get Marble data from server
				if(marble.color != 0x00 && marble.posX >= 0 && marble.posY >= 0){
					//Marble Data has arrived from the server
					Calculate_Servo_Rotate_Angle();
					state <<= 1;
				}
				break;
				
			case 0b00000100:	
				//Rotate Servo as Servo_Rotate_Angle to catch marble
				Servo_Move();
				//wait untill Servo has reached destination
				for(i=0;i<1000;i++){
					_delay_ms(1);
					if(STOP){
						//STOP Servo Rotation
						//This can be done by disconnecting OC pin
						
						while(STOP);//if STOP has occured, wait until resolved
					}
				}
				
				
				//Turn on ElectroMagnet
				//Alter Port for this action
				
				//Rotate Servo to Marble Box
				Servo_Drop_Marble();
				//wait untill Servo has reached destination
				
				//Turn of ElectroMagnet to drop marble
				//Alter Port for this action
				
				shk_detected = 0x00;
				//wait for some time for Shock Sensor to detect
				
				state <<= 1;
				
				//Set LED
				//Set OCR with Timer for PWM Control
				break;
				
			case 0b00001000:
				if(shk_detected == 0x01){ //Marble was succefully dropped
					shk_detected = 0x00;
					
					//Move servo to home
					Servo_Home();
					//wait until servo has moved
					
					state <<= 1;
				}
				else{ //Marble failed
					//Do other actions
				}
				break;
				
			case 0b00010000:
				LED_Set();
				state = 0x01; //Done, wait for pressure sensor
				break;
		}
    }
}

ISR(TIMER0_OVF_vect){ //Use Timer0 for collecting sensor value and PWM
	
	//These Sensor Values are not filtered
	//Must use filtered value in future
	
	switch(state){
		case 0x01: //if not started
			//Get Pressure Value
			//pressure_sensor_val = 포트
		break;
		
		default:
			//Get CdS Value
			//cds_sensor_val = 포트
			
			//Get Temperature Value
			//temp_sensor_val = 포트
			
			//Get Shock Value
			//shk_sensor_val = 포트
			if(shk_sensor_val >= 50)
				shk_detected = 0x01;
			
			//Get PSD Value
			//Need to Control PSD
			//psd_sensor_val...
			if(psd_sensor_val >= 50){ //if human detected
				STOP = 0x01;
			}else{
				STOP = 0x00;
			}
		break;
	}
	
	//always get Fire sensor value
	//Need to Control Fire Sensor
}

//Bluetooth_ Income Interrupt
ISR(USART1_RX_vect){
	rdata = UDR1;
	
	switch(rdata & 0xe0){
		case 0b10000000: //Marble Color returned from server
			marble.color = rdata & 0x07;
			//Server ex) 0b10000001:red, 0b10000010:green, 0b10000100:blue;
		break;
		
		case 0b01000000: //Marble posX returned from server
			marble.posX = rdata & 0x01;
			marble.posX += ((rdata & 0x02)>>1)*10;
			marble.posX += ((rdata & 0x04)>>2)*100;
			//Server ex) 0b01000123: posX=123
		break;
		
		case 0b00100000: //Marble posY returned from server
			marble.posY = rdata & 0x01;
			marble.posY += ((rdata & 0x02)>>1)*10;
			marble.posY += ((rdata & 0x04)>>2)*100;
			//Server ex) 0b00100123: posY=123
		break;
	}
}

void pin_init(){
	//Pin Setup
}

void init(){
	//Init Timer
	
}

void init_serial(void){
	//TX PD2, RX PD3
	UCSR0A = 0x00; //Reset
	UCSR0B = 0x18; //Allow transfer, prohibit interrupt
	UCSR0C = 0x06; //Data 8 bit
	
	UBRR0H = 0x00;
	UBRR0L = 10; //Baud Rate 9600
	
	
}

bool BT_send(char msg){
	//send msg
	while(!(UCSR1A & (1<<UDRE))); //Wait for buffer to clear
	UDR1 = msg; //send msg
	
	return true;
}

void Calculate_Servo_Rotate_Angle(){
	//Motor_Rotate_Angle = ...;
}

void Servo_Home(){
	Motor_Rotate_Angle = 0;
	OCR2 = (135 * Motor_Rotate_Angle)/900 + 10;
}
void Servo_Move(){
	OCR2 = (135 * Motor_Rotate_Angle)/900 + 10;
}
void Servo_Drop_Marble(){
	Motor_Rotate_Angle = 30;
	OCR2 = (135 * Motor_Rotate_Angle)/900 + 10;
}

void LED_Set(){
	
	//Need to find way to control 3 Led
	//Consider using demux
	//By using demux, we can select 1 of 3 LEDs with 1 output OC2 pin
	//need to wait for demux to set
	
	OCR2 = led_pwm_value; //Set PWM Value
}