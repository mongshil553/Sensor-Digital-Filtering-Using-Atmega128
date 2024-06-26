#define DEBUG_ 2
#define F_CPU 16000000UL

#define ElectroMagnet 7

//#define UBRR 103

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "Marble.h"
#include "Sensors.h"
#include "Bluetooth.h"
#include "Item.h"
#include "uart.h"

void pin_init();
void port_setup();
void timer0_init();
void timer1_init();
void timer2_init();
void adc_init(void);

//Electromagnet
void ElectroMagnet_On();
void ElectroMagnet_Off();

//Sensor Check
void If_PSD_Detected();
void If_Shock_Detected();
void If_Fire_Detected();

//System state
char state;


//**** Debug **************************************************************************************************************************************************//

#if DEBUG_ == 0
// Teammate 0 workspace
int main(void){
	// Deleted
}

#elif DEBUG_ == 1
// Teammate 1 workspace
int main(void){
	// Deleted
}

#endif
//************************************************************************************************************************************************************//

//**** Not Debug *********************************************************************************************************************************************//
#if DEBUG_ == 2

int main(void)
{	
	port_setup(); //setup port
	adc_init(); // ADC 초기화
	timer0_init(); // 타이머0 초기화
	timer1_init(); //타이머 초기화
	timer2_init();
	UART_init();
	
	init_BT();	//Bluetooth Setup
	
	EIMSK = 0xC3; //1100 0000
	EICRB = 0xF0; //External Interrupt Control Register(Edge)
	EICRA = 0x0F;
	Reset_sensor_val(); //센서 변수 초기화
	
	PORTA |= 0x80;
	ElectroMagnet_Off();
	
	marble.color = 0x05;
	marble.posX = -1;
	marble.posY = -1;
	
	sei(); //Allow Interrupt
	
	Servo_Allowed = 0x01;
	
	cur_item = ITEM_NONE; next_item = ITEM_NONE;
	
	Servo_Set_Speed(50);
	
	Servo_pos = SERVO_HOME;
	Servo_Goto(510);
	Servo_Go_Home();

	state = 0x01;
	int pressure_F = 0;
	
    while (1) {
		switch(state){
			case 0b00000001:					//not started
				PORTA |= 0x80;
				
				pressure_F = calc_force();
				if(pressure_F > 300){ //pressure 
					Select_Item(ITEM_NONE);
					BT_send('9'); //start signal
					PORTA &= 0x7F;
					
					state <<= 1;
				}
				break;
				
			case 0b00000010:	//wait for Marble data to arrive from the server
				if(BT_Receive()){
					state <<= 1;
				}
				break;
				
			case 0b00000100:	//Catch & Drop Marble
				Servo_Goto(Calculate_Servo_Rotate_Angle(marble.posY));
				
				//Servo Reached Destination				
				ElectroMagnet_On(); //Turn on ElectroMagnet
				_delay_ms(1000);
				
				//Rotate Servo to Marble Box
				Servo_Go_Box();

				//Servo Reached Destination
				shk_detected = 0x00; //Reset Shock Flag
				_delay_ms(300);
				ElectroMagnet_Off(); //Drop Marble
				
				_delay_ms(800); //Wait 1 seconds for Marble to drop and Shock to be detected

				state <<= 1;
				
				break;
				
			case 0b00001000: //Marble success or retry
				if(shk_detected == 0x01){ //Marble was succefully dropped
					shk_detected = 0x00;
					
					Servo_Go_Home();
					
					state <<= 1;
				}
				else{ //Marble failed
					//Move servo to home
					Servo_Set_Speed(20); //temporary
					Servo_Go_Home();
					
					//Turn on Electro Magnet
					ElectroMagnet_On();
					
					temp_en = 0x00; //temperature sensor does not control servo speed
					Servo_Set_Speed(80); //Very slowly
					
					//Move servo to Marble Collecting Box
					Servo_Go_Box();
					
					//Turn off Electro Magnet <- Drop Marble
					ElectroMagnet_Off();
					
					temp_en = 0x01; //temperature sensor controls servo speed
					
					//Servo_Set_Speed();
					Servo_Go_Home(); //Servo returns home
					
					state <<= 1;
				}
				
				break;
				
			case 0b00010000:
				
				if(marble.color == 0) RED_LED_On(calc_led());
				else if(marble.color == 1) GREEN_LED_On(calc_led());
				else if(marble.color == 2) BLUE_LED_On(calc_led());
				else Select_Item(ITEM_NONE);
				break;
				
		}
    }
}

#endif
//************************************************************************************************************************************************************//

void timer0_init(void) {
	//TCCR0 |= (1 << CS02) |(1<<CS01)| (1 << CS00); // 분주비 1024
	
	TCCR0 |= (1<<CS01) | (1 << CS00);
	
	TIMSK |= (1 << TOIE0); // 타이머0 오버플로우 인터럽트 허용
	TCNT0 = 0; // 타이머 카운터 초기화
}
void timer2_init(void) {
	//TCCR0 |= (1 << CS02) |(1<<CS01)| (1 << CS00); // 분주비 1024
	
	TCCR2 |= (1<<CS02) | (1 << CS00);
	
	TIMSK |= (1 << TOIE2); // 타이머0 오버플로우 인터럽트 허용
	TCNT2 = 0; // 타이머 카운터 초기화
}

void timer1_init(){
	TCCR1A=0x82;
	TCCR1B=0x1b;
	//TIMSK |= (1 << TOIE1); //타이머1 오버플로우 인터럽트 허용

	ICR1=4999;     //TOP
}

void port_setup(){
	DDRA = 0xFF;
	DDRD = 0x00;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRF = 0x00;
	DDRE = 0xFF;
}

//ADC 입력
//ADC 초기화
void adc_init(void){
	ADMUX=(1<<REFS0); //외부 레퍼런스 접압을 기준 전압으로 선택, 우측정렬, 초기 입력핀은 0번
	ADCSRA=(1<<ADEN)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //ADC enable, ADC interrupt enable 분주비 128
}

inline void Sensor_show(){
	if (cds_sensor_val > 100) { //CDS
		PORTA &= 0xFE; //CDS에 해당하는 LED만 켜기 //1111 1110
	}
	else {
		PORTA |= ~0xFE; //CDS에 해단하는 LED만 끄기
	}
	
	if (temp_sensor_val > 200) {
		PORTA &= 0xFD; // 1111 1101
	}
	else {
		PORTA |= ~0xFD;
	}
	
	
	
	if (pressure_sensor_val > 900) {//보류 -
		PORTA &= 0xFB; //1111 1011
	}
	else {
		PORTA |= ~0xFB;
	}
	
	if ( shk_detected) {
		PORTA &= 0xF7;
		//shk_detected = 0x00;
	}
	else {
		PORTA |= ~0xF7;
	}
	
	if (fire_sensor_val > 100) {//-
		PORTA &= 0xEF;
	}
	else {
		PORTA |= ~0xEF;
	}
	
	
	if ( (psd_sensor_val> 520)) {
		
		PORTA &= 0xBF;
	}
	else {
		PORTA |= ~0xBF;
	}
}

ISR(TIMER0_OVF_vect){ //Use Timer0 for collecting sensor value
	static char idx = 0x01;
	
	switch(idx){
		case 0x01:
			Read_CDS();
			//USART0_NUM(cds_sensor_val);
			idx=0x02;
		break;
		
		case 0x02:
			Read_Thermister();
			//USART0_NUM(temp_sensor_val);
			idx=0x04;
		break;
		
		case 0x04:
			Read_Pressure();
			//USART0_NUM(pressure_sensor_val);
			idx=0x05;
		break;
		
		case 0x05:
			Read_Shock();
			//USART0_NUM(shk_sensor_val);
			idx=0x06;
		break;
		
		case 0x06:
			Read_Fire();
			//USART0_NUM(fire_sensor_val);
			idx = 0x07;
		break;
		
		case 0x07:
			Read_PSD(); //Filtering Done
			//USART0_NUM(psd_sensor_val);
			idx = 0x01;
		break;
		
	}
	
	//ADC Mux 선택, ADC 시작 시키고 ISR 종료
	ADMUX = (ADMUX & 0x40) | (idx & 0x0F); //다음 센서 선택
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	_delay_us(100);
	
	//Sensor_show(); //LED A
}

ISR(TIMER2_OVF_vect){
	If_Shock_Detected();
	If_Fire_Detected();
	If_PSD_Detected();
	
	if(temp_en)
		Servo_Set_Speed(calc_speed());
}
ISR(TIMER1_OVF_vect){
	//PORTC = (PORTC & 0xF0) | next_item;
	cur_item = next_item;
	//_delay_ms(1);
}

void ElectroMagnet_On(){
	PORTB = (PORTB & ~(1<<0)) | (1<<0);
}

void ElectroMagnet_Off(){
	PORTB = (PORTB & ~(1<<0)) | (0<<0);
}

void If_Shock_Detected(){
	if(shk_sensor_val <= -50)
		shk_detected = 0x01;

}

ISR(INT1_vect) //For debugging
{
	GREEN_LED_On(calc_led());
}

ISR(INT0_vect) //For debugging
{
	RED_LED_On(calc_led());
	//BT_send('0');
	//GREEN_LED_On(500);
}


void If_PSD_Detected(){
	psd_dst = calc_dist();

	if ( psd_dst <= 45) {
		Servo_Allowed = 0x00;
		segment(prox4);
	}
	else if(psd_dst <= 55){
		Servo_Allowed = 0x01;
		segment(prox3);
	}
	else if(psd_dst <= 70){
		Servo_Allowed = 0x01;
		segment(prox2);
	}
	else if(psd_dst <= 80){
		Servo_Allowed = 0x01;
		segment(prox1);
	}
	else if(psd_dst <= 120){
		Servo_Allowed = 0x01;
		segment(0x00);
	}
}

void If_Fire_Detected(){
	static volatile short i = 0; //increment
	
	Fire_Detected = 0x00;
	if(fire_sensor_val <= 800){
		Fire_Detected = 0x01;
		state = 0x01;
		if(++i >= 15){
			i = 0;

			Buzzer_on(calc_hz());
		}
	}
}


