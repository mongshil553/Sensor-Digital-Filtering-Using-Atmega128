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
	volatile char color; //0x01:Red, 0x02:Green, 0x03:Blue, 0x04:None
	volatile short posX, posY;
};
volatile struct MarbleClass marble;

//Servo Motor
static volatile unsigned short Servo_Rotate_Angle, tmp;
void Calculate_Servo_Rotate_Angle();
void Servo_Home();
void Servo_Catch_Marble();
void Servo_Drop_Marble();
unsigned short Servo_pos, Servo_increment_threshold, Servo_step;
//Servo is using Timer2 for PWM

//Pressure Sensor
volatile char pressure_sensor_val;
//외부 변수 써서 고주파를 차단해야되면 LPF, MAF이용
//예를 들어, 이전 값 저장하는 변수

//CdS Sensor
volatile char cds_sensor_val;

//Temperature sensor
volatile char temp_sensor_val;
volatile char temp_en = 0x01;

//Shock Sensor
volatile char shk_sensor_val;
volatile char shk_detected;

//PSD Sensor
volatile char psd_sensor_val;

//LED PWM Value
volatile unsigned int led_pwm_value;
void LED_Set();
volatile char led_select = 0x00; //0x00: None, 0x01:Red, 0x02:Green, 0x03:Blue
//Use Timer2 for PWM when not using Servo

//MAIN
volatile char state = 0x01;
volatile char PSD_Detected = 0x00;

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
	
	//hello
	
	marble.color = 0x00;
	marble.posX = -1;
	marble.posY = -1;
	
	LED_Set();
	
	Servo_Home(); //Move Servo to Home Position
	
	//Need to Check Connection with Server
	//If not connected, try for few seconds and if failed connection, do other actions
	//Thus, there are two cases. 1, Bluetooth is connected, 2, Bluetooth is not connected
	//This is for just in case bluetooth fails at presentation
	
	volatile short i;
	
    while (1) {
		switch(state){
			case 0b00000001:					//not started
				if(pressure_sensor_val >= 50)	//pressure threshold is 50(just guessing)
					state <<= 1;				//Change state
				
			case 0b00000010:	//wait for Marble data to arrive from the server
				if(marble.color != 0x00 && marble.posX >= 0 && marble.posY >= 0){
					//Marble Data has arrived from the server
					Calculate_Servo_Rotate_Angle();
					state <<= 1;
				}
				break;
				
			case 0b00000100:	//Catch & Drop Marble
				while(Servo_pos != Servo_Rotate_Angle){
					Servo_Catch_Marble(); //Rotate Servo to Marble to catch marble
					_delay_ms(1);
					while(PSD_Detected);
					//Because Servo_Pos does not change, Servo stops;
					//PWM Value has to remain unchanged for Servo to hold position.
				}
				
				//Servo Reached Destination				
				//Turn on ElectroMagnet
				//Alter Port for this action
				
				//Rotate Servo to Marble Box
				while(Servo_pos != Servo_Rotate_Angle){
					Servo_Drop_Marble(); //Rotate Servo to Marble Collecting Box
					_delay_ms(1);
					while(PSD_Detected);
				}

				//Servo Reached Destination
				//Turn of ElectroMagnet to drop marble
				//Alter Port for this action
				
				shk_detected = 0x00;
				//wait for some time for Shock Sensor to detect
				
				state <<= 1;
				
				//Set LED
				//Set OCR with Timer for PWM Control
				break;
				
			case 0b00001000: //Marble success or retry
				if(shk_detected == 0x01){ //Marble was succefully dropped
					shk_detected = 0x00;
					
					//Move servo to home
					while(Servo_pos != Servo_Rotate_Angle){
						Servo_Home(); //Rotate Servo to Home
						_delay_ms(1);
						while(PSD_Detected);
					}
					
					state <<= 1;
				}
				else{ //Marble failed
					//Move servo to home
					while(Servo_pos != Servo_Rotate_Angle){
						Servo_Home(); //Rotate Servo to Home
						_delay_ms(1);
						while(PSD_Detected);
					}
					
					//Turn on Electro Magnet
					
					temp_en = 0x00; //temperature sensor does not control servo speed
					Servo_increment_threshold = 100;
					
					//Move servo to Marble Collecting Box
					while(Servo_pos != Servo_Rotate_Angle){
						Servo_Drop_Marble();
						_delay_ms(1);
						while(PSD_Detected);
					}
					
					//Turn off Electro Magnet <- Drop Marble
					
					temp_en = 0x01; //temperature sensor controls servo speed
					
					//Move servo home
					while(Servo_pos != Servo_Rotate_Angle){
						Servo_Home();
						_delay_ms(1);
						while(PSD_Detected);
					}
					
					state <<= 1;
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
		
		default: //started
			//Get CdS Value
			//cds_sensor_val = 포트
			
			//Get Temperature Value
			//temp_sensor_val = 포트
			if(temp_en == 0x01){
				//Set Servo_increment_threshold with respect to temp_sensor_val
				//Servo_increment_threshold = ...
				//Servo_increment_threshold is used to check if cnt has reached threshold and increment servo pwm value to destination
			}
			
			//Get Shock Value
			//shk_sensor_val = 포트
			if(shk_sensor_val >= 50)
				shk_detected = 0x01;
			
			//Get PSD Value
			//Need to Control PSD
			//psd_sensor_val...
			if(psd_sensor_val >= 50){ //if human detected
				PSD_Detected = 0x01;
			}else{
				PSD_Detected = 0x00;
			}
		break;
	}
	
	//always get Fire sensor value
	//Need to Control Fire Sensor
}

//Bluetooth_ Income Interrupt
ISR(USART1_RX_vect){
	
	static char key = 0;
	static short tmp = 0;  
	
	rdata = UDR1;
	
	if(rdata == '*'){ //server answer (react to mcu inquiry)
		//do something
	}
	else{ //Incoming Marble data ex) 1,512,908.  <- Marble Color=1, posX=512, posY=908
		switch(key){
			case 0: //incoming Marble Color
				if(rdata == ','){
					marble.color = (char)tmp;
					tmp = 0;
					key++;
				}
				else
					tmp = tmp*10+rdata;
			break;
			
			case 1: //incoming Marble posX
				if(rdata == ','){
					marble.posX = tmp;
					tmp = 0;
					key++;
				}
				else
					tmp = tmp*10+rdata;
			break;
			
			case 2: //incoming Marble posY
				if(rdata == '.'){
					marble.posY = tmp;
					tmp = 0;
					key = 0;
				}
				else
					tmp = tmp*10+rdata;
			break;
		}
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

void Servo_Move(){
	static unsigned short tmp = 0;
	
	if(++tmp == Servo_increment_threshold) Servo_pos += Servo_step;
	
	//Set Servo PWM Value
	//OCR2 = ...
	
}
void Servo_Home(){
	Servo_Rotate_Angle = 0;
	
	if(Servo_pos >= Servo_Rotate_Angle) Servo_step = -1;
	else Servo_step = 1;
	
	Servo_Move();
}
void Servo_Catch_Marble(){
	if(Servo_pos >= Servo_Rotate_Angle) Servo_step = -1;
	else Servo_step = 1;
	
	Servo_Move();
}
void Servo_Drop_Marble(){
	Servo_Rotate_Angle = 30;
	
	if(Servo_pos >= Servo_Rotate_Angle) Servo_step = -1;
	else Servo_step = 1;
	
	Servo_Move();
}


void LED_Set(){
	
	//Need to find way to control 3 Led
	//Consider using demux
	//By using demux, we can select 1 of 3 LEDs with 1 output OC2 pin
	//need to wait for demux to set
	//output 4 pins, R,G,B,None
	
	switch(marble.color){
		case 0x00: //LED Off
			led_select = 0x04;
		break;
		
		case 0x01: //Red
			led_select = 0x01;
		break;
		
		case 0x02: //Green
			led_select = 0x02;
		break;
		
		case 0x03: //Blue
			led_select = 0x03;
		break;
		
		case 0x04: //None <- LED Off
			led_select = 0x04;
		break;
	}
	
	OCR2 = led_pwm_value; //Set PWM Value
}
