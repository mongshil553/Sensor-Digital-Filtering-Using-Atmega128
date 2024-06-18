/*
 * GccApplication1.c
 *
 * Created: 2024-05-04 오전 12:07:19
 * Author : kijun
 */ 

#define DEBUG_ 1
#define F_CPU 16000000UL

#define ElectroMagnet 7

#define UBRR 103

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>//

#include "Marble.h"
#include "Sensors.h"
#include "Bluetooth.h"
#include "Item.h"

void pin_init();
void port_setup();
void timer0_init();
void timer1_init();
void adc_init(void);

//Electromagnet
void ElectroMagnet_On();
void ElectroMagnet_Off();

//Sensor Check
void If_PSD_Detected();
void If_Shock_Detected();
void If_Fire_Detected();


//**** Debug **************************************************************************************************************************************************//

#if DEBUG_ == 0
//현준이 일하는 곳 맨 위에 #define DEBUG_ 부분을 0으로 바꾸기
int main(void){
	
	volatile unsigned char LED=0xFF;
	
	DDRA=0xFF;
	DDRF=0x00;
	
	adc_init(); // ADC 초기화
	timer0_init(); // 타이머0 초기화
	timer1_init();
	
	Reset_sensor_val(); //센서 변수 초기화
	
	UBRR0L = (unsigned char)UBRR;
	UBRR0H = (unsigned char)(UBRR >> 8);

	
	UCSR0A = 0x00; //비동기 1배속
	UCSR0B = 0x18; //송수신 허가
	//UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0); //recv complete interrupt enable
	UCSR0C = 0x87; //비동기, non-parity mode, stop bit:1 bit, data: 8bit
	
	shk_detected = 0x00;
	
	
	
	sei(); // 전역 인터럽트 허용

	
	
	Select_Item(ITEM_NONE);
	port_setup();
	
	while (1) {
		// ADC 채널 값을 읽고 필요한 변수에 저장
		
		if (cds_sensor_val > 100) { //CDS
			LED &= 0xFE; //CDS에 해당하는 LED만 켜기 //1111 1110
		}
		else {
			LED |= ~0xFE; //CDS에 해단하는 LED만 끄기
		}
		
		
		
		
		if (temp_sensor_val > 200) {
			LED &= 0xFD; // 1111 1101
		}
		else {
			LED |= ~0xFD;
		}
		
		
		
		if (pressure_sensor_val > 900) {//보류 -
			LED &= 0xFB; //1111 1011
		}
		else {
			LED |= ~0xFB;
		}
		
		if ( shk_detected) {
			LED &= 0xF7;
			//shk_detected = 0x00;
		}
		else {
			LED |= ~0xF7;
		}
		
		if (fire_sensor_val > 100) {//-
			LED &= 0xEF;
		}
		else {
			LED |= ~0xEF;
		}
		
		if ( (psd_sensor_val> 520)) {
			LED &= 0xBF;
		}
		else {
			LED |= ~0xBF;
		}
		
		//if(shk_sensor_val <= 979){
			USART0_NUM(shk_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
		//}
		
		//---------------- 부저 ---------------//
		switch(PIND & 0x03){
			case 0x01:
			//ICR1 = 50;
			//OCR1A=ICR1/5;
			Buzzer_on(50);
			break;
			
			case 0x02:
			//ICR1 = 70;
			//OCR1A=ICR1/5;
			Buzzer_on(70);
			break;
			
			default:
			break;
		}
		//---------------- 부저 ---------------//
		
		PORTA=LED;
		_delay_us(1);

	}
	
	return 0;
	
}

void USART0_TX_vect(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART0_NUM(unsigned short num){

	USART0_TX_vect(num / 1000 + 48);
	USART0_TX_vect((num%1000) / 100 + 48);
	USART0_TX_vect((num%100)/10 + 48);
	USART0_TX_vect((num%10) + 48);
}


#elif DEBUG_ == 1
//기정이 일하는 곳
int main(void){
	//debug
	char iter = 0;
	
	cli();
	port_setup();
	timer1_init();
	//timer0_init();
	init_BT();
	
	Reset_sensor_val(); //센서 변수 초기화
	
	//EIMSK = 0x03; //External Interrupt Enable Mask
	EIMSK = 0x00;
	EICRA = 0x0F; //External Interrupt Control Register(Edge)

	sei();
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	shk_detected = 0x00;
	
	ElectroMagnet_Off();
	
	Servo_increment_threshold = 20;
	Servo_Allowed = 0x01;
	
	//Servo_Quick_Move(375);
	Servo_Goto(375);
	
	//PORTC &= 0xFB; //0x1111 1110
	//PORTC &= 0xF3; //0b1111 0010
	
	//PORTC = (PORTC & 0xF3) | (0x00 << 2);
	
	//0x1111 0011: mask, 
	
	while(1){
		_delay_ms(10);
		
		GREEN_LED_On(calc_led());
		
		switch(PIND & 0x03){
			case 0x02: //Select Red LED

				//Servo_Quick_Move(SERVO_HOME);
				//RED_LED_On((cds_sensor_val > CDS_MAX)?LED_MAX:(cds_sensor_val<CDS_MIN)?LED_MIN:(int)((LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*cds_sensor_val));
				RED_LED_On(800);
				//ElectroMagnet_On();
				//BT_send('1');
			break;
			
			case 0x01: //Select Green LED

				//Servo_Quick_Move(SERVO_BOX);
				//Select_Item(ITEM_LED_GREEN);
				
				GREEN_LED_On((cds_sensor_val > CDS_MAX)?LED_MAX:(cds_sensor_val<CDS_MIN)?LED_MIN:(int)((LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*cds_sensor_val));
				//GREEN_LED_On(800);
				//ElectroMagnet_Off();
			break;
		}
		
		
		/*
		if(BT_Receive()){
			//if(marble.color == 1) Servo_Quick_Move(200);
			//else if(marble.color == 2) Servo_Quick_Move(500);
			//else Servo_Quick_Move(375);
			
			if(marble.color == 0) RED_LED_On(500);
			else if(marble.color == 1) GREEN_LED_On(500);
			else BLUE_LED_On(500);
			
			for(iter=0; iter<50; iter++){ //2 second for shock to be detected
				_delay_ms(100);
				if(shk_detected==0x01){
					Servo_Quick_Move(SERVO_BOX);
					shk_detected = 0x00;
				}
				
			}
			
			Servo_Quick_Move(SERVO_HOME);
		}

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
		*/
		
	}
	
}

ISR(INT1_vect)
{
	//BT_send('1');

	//RED_LED_On(500);
}

ISR(INT0_vect)
{
	BT_send('0');
	//GREEN_LED_On(500);
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
	
	init_BT();	//Bluetooth Setup
	
	EIMSK = 0xC0; //1100 0000
	EICRB = 0xF0; //External Interrupt Control Register(Edge)
	
	Reset_sensor_val(); //센서 변수 초기화
	
	ElectroMagnet_Off();
	
	marble.color = 0x05;
	marble.posX = -1;
	marble.posY = -1;
	
	sei(); //Allow Interrupt
	
	Servo_Allowed = 0x01;
	
	Servo_Set_Speed(20);
	
	Servo_Goto(375);
	Servo_Go_Home();
	
	//Need to Check Connection with Server
	//If not connected, try for few seconds and if failed connection, do other actions
	//Thus, there are two cases. 1, Bluetooth is connected, 2, Bluetooth is not connected
	//This is for just in case bluetooth fails at presentation
	
	//short i;
	char state = 0x01;
	
    while (1) {
		switch(state){
			case 0b00000001:					//not started
				//if(pressure_sensor_val >= 50)	//pressure threshold is 50(just guessing)
					//state <<= 1;				//Change state
				
				if(pressure_sensor_val > 900){ //pressure 
					Select_Item(ITEM_NONE);
					ElectroMagnet_On();
					BT_send('0'); //start signal
					state <<= 1;
				}
				
				switch(PIND & 0x03){
					case 0x2:
					//ICR1 = 50;
					//OCR1A=ICR1/5;
					ElectroMagnet_On();
					break;
					
					case 0x01:
					//ICR1 = 70;
					//OCR1A=ICR1/5;
					ElectroMagnet_Off();
					break;
					
					default:
					break;
				}
				
				break;
				
			case 0b00000010:	//wait for Marble data to arrive from the server
				if(BT_Receive()){
					//Marble Data has arrived from the server
					//Calculate_Marble_pos();
					state <<= 1;
				}
				break;
				
			case 0b00000100:	//Catch & Drop Marble
				//Servo_Go_Marble(); //Rotate Servo to Marble to catch marble
				Servo_Goto(375);
				
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
					//Servo_Go_Home();
					//state <<= 1;
					//break;
					
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
					
					Servo_Set_Speed(20);
					Servo_Go_Home(); //Servo returns home
					
					state <<= 1;
				}
				break;
				
			case 0b00010000:
				
				
				if(marble.color == 0) RED_LED_On(get_led_val);
				else if(marble.color == 2) GREEN_LED_On(get_led_val);
				else if(marble.color == 1) BLUE_LED_On(get_led_val);
				else Select_Item(ITEM_NONE);
				
				//LED_Set(); //LED PWM of Marble Color
				state = 0x01; //Done, wait for pressure sensor
				break;
		}
    }
}

ISR(INT7_vect)
{
	//BT_send('0');
	//GREEN_LED_On(500);
	
	//Select_Item(ITEM_NONE);
	//Servo_Allowed = 0x00;
}

#endif
//************************************************************************************************************************************************************//

void timer0_init(void) {
	TCCR0 |= (1 << CS02) |(1<<CS01)| (1 << CS00); // 분주비 1024
	//TCCR0 |= (1<<CS01) | (1 << CS00);
	TIMSK |= (1 << TOIE0); // 타이머0 오버플로우 인터럽트 허용
	TCNT0 = 0; // 타이머 카운터 초기화
}

void timer1_init(){
	TCCR1A=0x82;
	TCCR1B=0x1b;

	ICR1=4999;     //TOP
}

void port_setup(){
	DDRA = 0xFF;
	DDRD = 0x00;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRF = 0x00;
	DDRE = 0x00;
	
	//DDRC = 0xFF;
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
			USART0_NUM(cds_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
			
			idx=0x02;
		break;
		
		case 0x02:
			Read_Thermister();
			//Servo_Set_Speed();
			USART0_NUM(temp_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
			idx=0x04;
		break;
		
		case 0x04:
			Read_Pressure();
			USART0_NUM(pressure_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
			idx=0x05;
		break;
		
		case 0x05:
		
			Read_Shock();
			If_Shock_Detected();
			USART0_NUM(shk_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
			idx=0x06;
		
		break;
		
		case 0x06:
		
			Read_Fire();
			//If_Fire_Detected();
			USART0_NUM(fire_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
			idx = 0x07;
		break;
		
		case 0x07:
		
			Read_PSD();
			If_PSD_Detected();
			USART0_NUM(psd_sensor_val);
			USART0_TX_vect('\n');
			USART0_TX_vect('\r');
			idx = 0x01;
		break;
	}
	
	//ADC Mux 선택, ADC 시작 시키고 ISR 종료
	ADMUX = (ADMUX & 0x40) | (idx & 0x0F); //다음 센서 선택
	//ADCSRA |= (1 << ADSC); // ADC 변환 시작
	
	Sensor_show(); //LED A
}

void ElectroMagnet_On(){
	//PORTC &= (0 << ElectroMagnet);
	PORTC = (PORTC & ~(1<<ElectroMagnet)) | (1<<ElectroMagnet);
	//PORTC = 0x7F;
}

void ElectroMagnet_Off(){
	//PORTC |=  (1 << ElectroMagnet);
	PORTC = (PORTC & ~(1<<ElectroMagnet)) | (0<<ElectroMagnet);
	//PORTC = 0xFF;
}

void If_Shock_Detected(){
	if(shk_sensor_val <= 900)
		shk_detected = 0x01;

}

void If_PSD_Detected(){
	if ( (psd_sensor_val> 520)) {
		//PORTC |= 0x20; //0010 0000
		//PORTA &= 0xBF;
		Servo_Allowed = 0x00;
	}
	else {
		//PORTC &= ~(0x20); //0100 0000
		//PORTA |= ~0xBF;
		Servo_Allowed = 0x01;
	}
	
}

void If_Fire_Detected(){
	static char i = 0; //increment
	static char s = 0x00; //state
	static char was = 0x00; //was detected
	
	if(fire_sensor_val >= 500){
	//if(1){
		
		/*if(!was){ //was not detected -> turn buzzer on
			was = 0x01;
			switch(s){
				case 0x00:
				Buzzer_on(50);
				break;
				case 0x01:
				Buzzer_on(70);
			}
		}
		else if(++i >= 20){
			if(s) s = 0x01;
			else s = 0x00;
			
			switch(s){
				case 0x00:
				Buzzer_on(50);
				break;
				case 0x01:
				Buzzer_on(70);
			}
		}*/
		
		if(++i >= 10){
			if(s) s = 0x00;
			else s = 0x01;
			i = 0;
			
			switch(s){
				case 0x00:
				Buzzer_on(50);
				break;
				case 0x01:
				Buzzer_on(70);
			}
		}
	}
	else{
		was = 0x00;
		Buzzer_off();
	}
}


void USART0_TX_vect(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART0_NUM(unsigned short num){

	USART0_TX_vect(num / 1000 + 48);
	USART0_TX_vect((num%1000) / 100 + 48);
	USART0_TX_vect((num%100)/10 + 48);
	USART0_TX_vect((num%10) + 48);
}