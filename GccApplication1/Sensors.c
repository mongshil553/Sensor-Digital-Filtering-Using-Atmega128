#include "Sensors.h"
#include <avr/io.h>

//Read_{센서} 부분에서 필터링 실행

uint16_t lpf(uint16_t current_value, uint16_t new_value, float alpha) {
	return (uint16_t)(alpha * new_value + (1 - alpha) * current_value);
}

uint16_t hpf(uint16_t previous_value, uint16_t new_value, float alpha) {
	return (uint16_t)(alpha * (previous_value + new_value) - previous_value);
}

inline void Read_CDS(){
	static uint16_t previous_value = 0;
	cds_sensor_val = lpf(previous_value, ADC, 0.1);
	previous_value = cds_sensor_val;

	//필터링 해야됨
}

inline void Read_Fire(){
	static uint16_t previous_value = 0;
	fire_sensor_val = lpf(previous_value, ADC, 0.1);
	previous_value = fire_sensor_val;
	//필터링 해야됨
}
inline void Read_PSD(){
	static uint16_t previous_value = 0;
	psd_sensor_val = lpf(previous_value, ADC, 0.1);
	previous_value = psd_sensor_val;
	//필터링 해야됨
}

inline void Read_Pressure(){
	static uint16_t previous_value = 0;
	pressure_sensor_val = hpf(previous_value, ADC, 0.6);
	previous_value = pressure_sensor_val;
	//필터링 해야됨
}

inline void Read_Thermister(){
	static uint16_t previous_value = 0;
	temp_sensor_val = hpf(previous_value, ADC, 0.5);
	previous_value = temp_sensor_val;
	//필터링 해야됨
}

inline void Read_Shock(){
	static uint16_t previous_value = 0;
	shk_sensor_val = lpf(previous_value, ADC, 0.1);
	previous_value = shk_sensor_val;
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