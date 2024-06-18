#include "Sensors.h"
#include <avr/io.h>

//Read_{센서} 부분에서 필터링 실행
double tmp1, tmp2;

unsigned short lpf(unsigned short current_value, unsigned short new_value, float alpha) {
	return (unsigned short)(alpha * new_value + (1 - alpha) * current_value);
}

unsigned short hpf(unsigned short previous_value, unsigned short new_value, float alpha) {
	return (unsigned short)(alpha * (previous_value + new_value) - previous_value);
}

inline void Read_CDS(){
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(!(ADCSRA & (1 << ADIF))); // ADC 변환 완료 플래그가 설정될 때까지 기다림
	ADCSRA |= (1<< ADIF); // ADC 변환 완료 플래그 클리어
	/*static unsigned short previous_value = 0;
	cds_sensor_val = lpf(previous_value, ADC, 0.1);
	previous_value = cds_sensor_val;*/
	cds_sensor_val=ADC;

	//필터링 해야됨
}

inline void Read_Fire(){
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(!(ADCSRA & (1 << ADIF))); // ADC 변환 완료 플래그가 설정될 때까지 기다림
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	//static unsigned short previous_value = 0;
	fire_sensor_val = ADC;//lpf(previous_value, ADC, 0.1);
	//previous_value = fire_sensor_val;
	//필터링 해야됨
}
inline void Read_PSD(){
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(!(ADCSRA & (1 << ADIF))); // ADC 변환 완료 플래그가 설정될 때까지 기다림
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	//static unsigned short previous_value = 0;
	psd_sensor_val = ADC;//lpf(previous_value, ADC, 0.1);
	//previous_value = psd_sensor_val;
	//필터링 해야됨
}

inline void Read_Pressure(){
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(!(ADCSRA & (1 << ADIF))); // ADC 변환 완료 플래그가 설정될 때까지 기다림
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	//static uint16_t previous_value = 0;
	pressure_sensor_val = ADC;//lpf(previous_value, ADC, 1);
	//previous_value = pressure_sensor_val;
	//필터링 해야됨
}

inline void Read_Thermister(){
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(!(ADCSRA & (1 << ADIF))); // ADC 변환 완료 플래그가 설정될 때까지 기다림
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	//static unsigned short previous_value = 0;
	temp_sensor_val = ADC;//lpf(previous_value, ADC, 1);
	//previous_value = temp_sensor_val;
	//필터링 해야됨
}

inline void Read_Shock(){
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(!(ADCSRA & (1 << ADIF))); // ADC 변환 완료 플래그가 설정될 때까지 기다림
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	//static unsigned short previous_value = 0;
	shk_sensor_val = ADC;//hpf(previous_value, ADC, 1);
	//previous_value = shk_sensor_val;
	//필터링 해야됨
}

inline void Reset_sensor_val(){
	pressure_sensor_val = 0;
	cds_sensor_val	= 0;
	temp_sensor_val = 0;	temp_en = 0;
	shk_sensor_val	= 0;	shk_detected = 0;
	psd_sensor_val	= 0;
	fire_sensor_val	= 0;
}