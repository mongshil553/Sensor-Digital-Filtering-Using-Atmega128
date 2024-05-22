#include "Sensors.h"
#include <avr/io.h>

//Read_{센서} 부분에서 필터링 실행

inline void Read_CDS(){
	cds_sensor_val = ADC;
	//필터링 해야됨
}

inline void Read_Fire(){
	fire_sensor_val = ADC;
	//필터링 해야됨
}
inline void Read_PSD(){
	psd_sensor_val = ADC;
	//필터링 해야됨
}

inline void Read_Pressure(){
	pressure_sensor_val = ADC;
	//필터링 해야됨
}

inline void Read_Thermister(){
	temp_sensor_val = ADC;
	//필터링 해야됨
}

inline void Read_Shock(){
	shk_sensor_val = ADC;
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