/*
 * GccApplication1.c
 *
 * Created: 2024-05-04 오전 12:07:19
 * Author : kijun
 */ 

#define DEBUG_ 0
#define F_CPU 16000000UL
//#define USE_BLUETOOTH_INTERRUPT

#define ElectroMagnet 7

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "Overall.h"
#include "Marble.h"
#include "Servo.h"
#include "Sensors.h"
#include "Bluetooth.h"

void sys_init();
void timer_setup();
void port_setup();

//Demux
#define ITEM_NONE 0x00;
#define ITEM_SERVO 0x04;
#define ITEM_SPEAKER 0x05;
#define ITEM_LED_RED 0x08;
#define ITEM_LED_GREEN 0x09;
#define ITEM_LED_BLUE 0x0A;
void Select_Item(char item);

//MAIN
volatile char state = 0x01;
volatile char PSD_Detected = 0x00;
volatile uint16_t adc_value =0; //ADC값 저장
volatile uint8_t current_channel=0;//현재 읽고있는 채널 값 저장


//LED PWM Value
volatile unsigned int led_pwm_value;
void LED_Set();
volatile char led_select = 0x00; //0x00: None, 0x01:Red, 0x02:Green, 0x03:Blue
//Use Timer2 for PWM when not using Servo

//Electromagnet
void ElectroMagnet_On();
void ElectroMagnet_Off();


void pin_init();
void port_setup();
void bt_init();

//**** Debug **************************************************************************************************************************************************//

#if DEBUG_ == 0
//현준이 일하는 곳 맨 위에 #define DEBUG_ 부분을 0으로 바꾸기
int main(void){
	
	adc_init(); // ADC 초기화
	timer0_init(); // 타이머0 초기화
	sei(); // 전역 인터럽트 허용

	while (1) {
		// ADC 채널 값을 읽고 필요한 변수에 저장
		switch (current_channel) {
			case 0x03:
			temp_sensor_val = adc_value;
			case 0x04:
			pressure_sensor_val = adc_value;
			break;
			case 0x05:
			shk_sensor_val = adc_value;
			break;
			case 0x06:
			fire_sensor_val = adc_value;
			break;
			case 0x07:
			psd_sensor_val = adc_value;
			break;
		}
		_delay_ms(10); 
	}

	return 0;
	
}



ISR(ADC_vect){//인터럽트 사용해서 ADC값 읽어옴
	adc_value=ADC; //ADC값 저장
}


ISR(TIMER0_OVF_vect){ //Use Timer0 for collecting sensor value and PWM
	
	static uint8_t channel_index = 0;
	uint8_t channels[] = {4, 5, 6, 7}; // ADC 채널 배열

	// 현재 채널 설정 및 변환 시작
	current_channel = channels[channel_index];
	ADMUX = (ADMUX & 0xF8) | (current_channel & 0x07); // ADC 채널 선택
	ADCSRA |= (1 << ADSC); // ADC 변환 시작

	// 다음 채널로 인덱스 이동
	channel_index++;
	if (channel_index >= sizeof(channels)) {
		channel_index = 0;
	}
}

//ADC 입력
//ADC 초기화
void adc_init(void){
	ADMUX=(1<<REFS0); //외부 레퍼런스 접압을 기준 전압으로 선택, 우측정렬, 초기 입력핀은 0번
	ADCSRA=(1<<ADEN)|(7<<ADPS0)|(1<<ADIE); //ADC enable, ADC interrupt enable 분주비 128
}

void start_adc_conversion(void){//인터럽트 사용할때 ADC 변화
	ADCSRA |=(1<<ADSC); //ADC 변화 시작
}

uint16_t read_adc_channel(uint8_t ch) {
	
	ADMUX = (ADMUX & 0xF8) | (ch & 0x07);// ADC 채널 선택
	start_adc_conversion();
	while (ADCSRA & (1 << ADSC)); // 변환 완료 대기
	_delay_ms(10); // 약간의 시간 대기
	return adc_value; // ADC 값 반환
}

void adc_read(){
	
	adc_init();//ADC 초기화
	sei();
	
	uint16_t pressure_sensor_value = 0;
	uint16_t vibration_sensor_value = 0;
	uint16_t psd_sensor_value = 0;
	uint16_t fire_sensor_value = 0;
	
	while (1) {
		// ADC 채널 4 (압력 센서) 값 읽기
		pressure_sensor_value = read_adc_channel(4);
		
		// ADC 채널 5 (진동 감지 센서) 값 읽기
		vibration_sensor_value = read_adc_channel(5);
		
		// ADC 채널 6 (불꽃 감지 센서) 값 읽기
		fire_sensor_value = read_adc_channel(6);
		
		// ADC 채널 7 (PSD 센서) 값 읽기
		psd_sensor_value = read_adc_channel(7);
		
	}
}

void timer0_init(void) {
	TCCR0 = (1 << CS02) | (1 << CS00); // 분주비 1024
	TIMSK = (1 << TOIE0); // 타이머0 오버플로우 인터럽트 허용
	TCNT0 = 0; // 타이머 카운터 초기화
}


#elif DEBUG_ == 1
//기정이 일하는 곳
int main(void){
	//debug
	char iter = 0;
	
	cli();
	port_setup();
	timer_setup();
	bt_init();
	
	EIMSK = 0x03;
	EICRA = 0x0F;

	sei();
	
	ElectroMagnet_Off();
	
	Servo_increment_threshold = 20;
	Servo_Allowed = 0x01;
	
	Servo_Quick_Move(375);
	
	shk_detected = 0x00;
	
	while(1){
		_delay_ms(10);
		
#ifndef USE_BLUETOOTH_INTERRUPT
		if(BT_Receive()){
			if(marble.color == 1) Servo_Quick_Move(200);
			else if(marble.color == 2) Servo_Quick_Move(500);
			else Servo_Quick_Move(375);
			
			for(iter=0; iter<50; iter++){ //2 second for shock to be detected
				_delay_ms(100);
				if(shk_detected==0x01){
					Servo_Quick_Move(SERVO_BOX);
					shk_detected = 0x00;
				}
				
			}
			
			Servo_Quick_Move(SERVO_HOME);
		}
#endif
		switch(PIND & 0x03){
			case 0x01:
				ElectroMagnet_On();
				//Servo_Go_Home();
				//Servo_Quick_Move(SERVO_HOME);
			break;
			
			case 0x02:
				ElectroMagnet_Off();
				//Servo_Go_Box();
				//Servo_Quick_Move(SERVO_BOX);
			break;
			
			default:
			break;
		}
		
		
	
	}
	
}

ISR(INT1_vect)
{
	//BT_send('1');
	shk_detected = 0x01;
}

ISR(INT0_vect)
{
	BT_send('0');
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
#if DEBUG_ == 3

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
				
				_delay_ms(2000); //Wait 2 seconds for Marble to drop and Shock to be detected

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
					Servo_increment_threshold = 40; //Very slowly
					
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
	
	static char idx = 0x01;
	
	switch(idx){
	case 0x01:
		Read_CDS();
		idx <<=1;
		break;
		
	case 0x02:
		Read_Fire();
		//Is_Fire_Interrupt();
		idx <<=1;
		break;
		
	case 0x04:
		Read_Pressure();
		idx <<=1;
		break;
		
	case 0x08:
		Read_PSD();
		//Is_PSD_Interrupt();
		idx <<= 1;
		break;
		
	case 0x10:
		Read_Shock();
		idx <<=1;
		break;
		
	case 0x20:
		Read_Thermister();
		idx = 0x00;
		break;
	}
	
	//ADC Start
	
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

void pin_init(){
	//Pin Setup
}

void init(){
	//Init Timer
	
}

#endif
//************************************************************************************************************************************************************//

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
	PORTC = (PORTC & 0xF0) | item;
	_delay_us(100);
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
