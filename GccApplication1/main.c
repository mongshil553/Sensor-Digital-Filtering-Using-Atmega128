/*
 * GccApplication1.c
 *
 * Created: 2024-05-04 오전 12:07:19
 * Author : kijun
 */ 

#define DEBUG_
#define F_CPU 16000000UL
//#define USE_BLUETOOTH_INTERRUPT

#define ElectroMagnet 7
#define Servo_MAX 520
#define Servo_MIN 230
#define SERVO_HOME 620
#define SERVO_BOX 130

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void sys_init();
void timer_setup();
void port_setup();

//Marble
struct MarbleClass{
	char color; //0x01:Red, 0x02:Green, 0x03:Blue, 0x04:None
	short posX, posY;
};
struct MarbleClass marble;
unsigned short Marble_pos;
void Calculate_Marble_pos();

//Demux
#define ITEM_NONE 0x00;
#define ITEM_SERVO 0x01;
#define ITEM_LED_RED 0x02;
#define ITEM_LED_GREEN 0x03;
#define ITEM_LED_BLUE 0x04;
#define ITEM_SPEARK 0x05;
void Select_Item(char item);

//Servo Motor
volatile unsigned short Servo_target, tmp;
//void Servo_Set_Target(unsigned short val);
void Servo_Quick_Move(unsigned short val);
void Servo_Go_Home();
void Servo_Go_Box();
void Servo_Go_Marble();
void Servo_Act();
void Servo_Set_Target(unsigned short val);
unsigned short Servo_pos, Servo_increment_threshold, Servo_step;
//Servo invrement_threshold: 0:Super Fast, 50:Very Slow
//Servo is using Timer1 for PWM

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

//Electromagnet
void ElectroMagnet_On();
void ElectroMagnet_Off();

//MAIN
volatile char state = 0x01;
volatile char PSD_Detected = 0x00;
volatile char Action_Allowed = 0x01;

//블루투스 통신
void init_serial(void);
void BT_send(char msg);
char BT_Receive(); //#x. : Color, !xxx. : posX, *xxx. : posY
volatile char rdata = 0; //read buffer from BT
volatile char rdatas[12];


void pin_init();
void port_setup();
void bt_init();

//**** Debug **************************************************************************************************************************************************//
#ifdef DEBUG_

int main(void){
	//debug
	
	cli();
	port_setup();
	timer_setup();
	bt_init();
	sei();
	
	ElectroMagnet_Off();
	
	Servo_increment_threshold = 20;
	Action_Allowed = 0x01;
	
	Servo_Quick_Move(375);
	
	while(1){
		_delay_ms(10);
		
#ifndef USE_BLUETOOTH_INTERRUPT
		if(BT_Receive()){
			if(marble.posX == 123) Servo_Quick_Move(200);
			else if(marble.posX == 321) Servo_Quick_Move(500);
			else Servo_Quick_Move(375);
		}
#endif
		
		switch(PIND & 0x03){
			case 0x01:
				ElectroMagnet_On();
				Servo_Go_Home();
			break;
			
			case 0x02:
				ElectroMagnet_Off();
				Servo_Go_Box();
			break;
			
			default:
			break;
		}
	
	}
	
}

ISR(TIMER0_OVF_vect){ //Use Timer0 for collecting sensor value and PWM
	
	volatile static char key = 0;
	
	if(key == 0) ElectroMagnet_On();
	else ElectroMagnet_Off();
	
	//These Sensor Values are not filtered
	//Must use filtered value in future
	/*
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
	*/
}

#ifdef USE_BLUETOOTH_INTERRUPT

ISR(USART1_RX_vect){
	
	rdata = UDR1;
	BT_send(rdata);
	
	static volatile char key = 0;
	
	ElectroMagnet_On();
	
	if(key == 0){
		if(rdata >= 0 && rdata <= 3){
			marble.color = rdata;
			key++;
		}
	}
	else if(key == 1){
		marble.posX = rdata*10;
		key++;
	}
	else if(key == 2){
		marble.posY = rdata*10;
		key++;
	}
	
	if(key == 3){
		ElectroMagnet_Off();
		key = 0;
		if(marble.color == 1) Servo_Go_Home();
		else if(marble.color == 2) Servo_Go_Box();
		else if(marble.color == 3){
			Servo_Set_Target((SERVO_BOX+SERVO_HOME)/2);
			Servo_Act();
		}
		else{
			Servo_Set_Target((SERVO_BOX+SERVO_HOME)/3);
			Servo_Act();
		}
	}
}
#endif // BLUETOOTH_INTERRUPT

#endif
//************************************************************************************************************************************************************//

//**** Not Debug *********************************************************************************************************************************************//
#ifndef DEBUG_

int main(void)
{
	pin_init(); //Pin Setup
	init();		//Interrupt, Timer, Register
	bt_init();	//Bluetooth Setup
	
	Action_Allowed = 0x01;
	
	init_serial();
	
	marble.color = 0x00;
	marble.posX = -1;
	marble.posY = -1;
	
	LED_Set();
	
	Select_Item(ITEM_SERVO);
	Servo_Go_Home(); //Move Servo to Home Position
	
	//Need to Check Connection with Server
	//If not connected, try for few seconds and if failed connection, do other actions
	//Thus, there are two cases. 1, Bluetooth is connected, 2, Bluetooth is not connected
	//This is for just in case bluetooth fails at presentation
	
	short i;
	
    while (1) {
		switch(state){
			case 0b00000001:					//not started
				if(pressure_sensor_val >= 50)	//pressure threshold is 50(just guessing)
					state <<= 1;				//Change state
				
			case 0b00000010:	//wait for Marble data to arrive from the server
				if(BT_Receive()){
					//Marble Data has arrived from the server
					Calculate_Marble_pos();
					state <<= 1;
				}
				break;
				
			case 0b00000100:	//Catch & Drop Marble
				Servo_Go_Marble(); //Rotate Servo to Marble to catch marble
				
				//Servo Reached Destination				
				ElectroMagnet_On(); //Turn on ElectroMagnet
				
				//Rotate Servo to Marble Box
				Servo_Go_Box();

				//Servo Reached Destination
				shk_detected = 0x00; //Reset Shock Flag
				ElectroMagnet_Off(); //Drop Marble

				state <<= 1;
				
				//Set LED
				//Set OCR with Timer for PWM Control
				break;
				
			case 0b00001000: //Marble success or retry
				if(shk_detected == 0x01){ //Marble was succefully dropped
					shk_detected = 0x00;
					
					Servo_Go_Home();
					
					state <<= 1;
				}
				else{ //Marble failed
					//Move servo to home
					Servo_Go_Home();
					
					//Turn on Electro Magnet
					ElectroMagnet_On();
					
					temp_en = 0x00; //temperature sensor does not control servo speed
					Servo_increment_threshold = 40;
					
					//Move servo to Marble Collecting Box
					Servo_Go_Box();
					
					//Turn off Electro Magnet <- Drop Marble
					ElectroMagnet_Off();
					
					temp_en = 0x01; //temperature sensor controls servo speed
					
					Servo_Go_Home(); //Servo returns home
					
					state <<= 1;
				}
				break;
				
			case 0b00010000:
				switch(marble.color){
					case 0x00:
						Select_Item(ITEM_NONE);
						break;
					case 0x01:
						Select_Item(ITEM_LED_RED);
						break;
					case 0x02:
						Select_Item(ITEM_LED_GREEN);
						break;
					case 0x03:
						Select_Item(ITEM_LED_BLUE);
						break;
					default:
						Select_Item(ITEM_NONE);
						break;
				}
				LED_Set(); //LED PWM of Marble Color
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

#endif
//************************************************************************************************************************************************************//

void bt_init(){
	UCSR1A = 0x00;
	UCSR1B = 0x18;
	UCSR1C = 0x06;
	
	UBRR1H = 0x00;
	UBRR1L = 103;
	
#ifdef USE_BLUETOOTH_INTERRUPT
	UCSR1B |= 0x80; //Enable Interrupt
#endif
}

void timer_setup(){
	TCCR1A=0x82;
	TCCR1B=0x1b;

	ICR1=4999;     //TOP
}

void port_setup(){
	DDRA = 0xFF;
	DDRD = 0x00;
	DDRB = 0xFF;
	DDRC = (1<<ElectroMagnet);
	//DDRC = 0xFF;
}

inline void ElectroMagnet_On(){
	PORTC &= (0 << ElectroMagnet);
	//PORTC = 0x7F;
}

inline void ElectroMagnet_Off(){
	PORTC |=  (1 << ElectroMagnet);
	//PORTC = 0xFF;
}

void Select_Item(char item){
	PORTC = (PORTC & 0xF8) | item;
	_delay_us(100);
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
			BT_send(rdata);
			//ElectroMagnet_On();
			return 0x00;
		}
		else marble.color = rdata;
		break;
		
		case 2:
		if(rdata == '.') {
			step = 0;
			BT_send(rdata);
			//ElectroMagnet_On();
			return 0x00;
		}
		else marble.posX = marble.posX * 10 + rdata - '0';
		break;
		
		case 3:
		if(rdata == '.') {
			step = 0;
			BT_send(rdata);
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
	
	BT_send(rdata);

	return 0x00;
}

void BT_send(char msg){
	//send msg
	while(!(UCSR1A & (1<<UDRE))); //Wait for buffer to clear
	UDR1 = msg; //send msg
}

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
	
	while((Servo_pos != Servo_target) && Action_Allowed){
		if(++tmp == Servo_increment_threshold){
			Servo_pos += Servo_step;
			OCR1A = Servo_pos;
			tmp = 0;
		}
		_delay_us(100);
	}
	
}

void LED_Set(){
	
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
