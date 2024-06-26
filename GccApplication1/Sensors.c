#include "Sensors.h"
#include "Item.h"
#include "uart.h"
#include <avr/io.h>

//Read_{센서} 부분에서 필터링 실행
double tmp1, tmp2;

long Fir_low_b[11] = {143, 303, 723, 1245, 1670, 1833, 1670, 1245, 723, 303, 143};
long IIR_low_b[3] = {82, 164, 82};
long IIR_low_a[2] = {-17284, 7611};
long IIR_high_b[3] = {1059, -2117, 1059};
long IIR_high_a[2] = {8927, 3162};


short lpf(unsigned short current_value, unsigned short new_value, float alpha) {
	return (short)(alpha * new_value + (1 - alpha) * current_value);
}

inline void Read_CDS(){ //IIR Low
	ADCSRA |= (1<< ADIF); // ADC 변환 완료 플래그 클리어
	
	static short iter0 = 0, iter1 = 0;
	static long sum = 0;
	static short dataX[3] = {0,0,0};
	static short dataY[2] = {0,0};
	static short iX = 0, jX = 2, iY=0, jY=1;
	
	iX = (iX+1) % 3;
	jX = (jX+1) % 3;
	dataX[jX] = ADC;
	
	sum=0;
	for(iter0 = 2, iter1 = iX; iter0 >= 0; iter0--){
		sum += IIR_low_b[iter0] * dataX[iter1];
		iter1=(iter1+1)%3;
	}
	for(iter0 = 1, iter1 = iY; iter0>=0; iter0--){
		sum -= IIR_low_a[iter0] * dataY[iter1];
		iter1=(iter1+1)%2;
	}
	
	iY = (iY+1)%2;
	jY = (jY+1)%2;
	dataY[jY] = sum/10000;
	cds_sensor_val = dataY[jY];
}

inline void Read_Fire(){ //FIR Low
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	
	static short iter0 = 0, iter1 = 0;
	static long sum = 0;
	static short data[11] = {0,0,0,0,0,0,0,0,0,0,0};
	static short i = 0, j = 10;
	
	i = (i+1) % 11;
	j = (j+1) % 11;
	data[j] = ADC;

	sum=0;
	for(iter0 = 10, iter1 = i; iter0 >= 0; iter0--){
		sum += Fir_low_b[iter0] * data[iter1];
		iter1=(iter1+1)%11;
	}

	fire_sensor_val = sum/10000;
}
inline void Read_PSD(){ //IIR Low
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	
	static short iter0 = 0, iter1 = 0;
	static long sum = 0;
	static short dataX[3] = {0,0,0};
	static short dataY[2] = {0,0};
	static short iX = 0, jX = 2, iY=0, jY=1;
	
	iX = (iX+1) % 3;
	jX = (jX+1) % 3;
	dataX[jX] = ADC;
	
	sum=0;
	for(iter0 = 2, iter1 = iX; iter0 >= 0; iter0--){
		sum += IIR_low_b[iter0] * dataX[iter1];
		iter1=(iter1+1)%3;
	}
	for(iter0 = 1, iter1 = iY; iter0>=0; iter0--){
		sum -= IIR_low_a[iter0] * dataY[iter1];
		iter1=(iter1+1)%2;
	}
	
	iY = (iY+1)%2;
	jY = (jY+1)%2;
	dataY[jY] = sum/10000;
	psd_sensor_val = dataY[jY];
}

inline void Read_Pressure(){ //FIR Low
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	
	static short iter0 = 0, iter1 = 0;
	static long sum = 0;
	static short data[11] = {0,0,0,0,0,0,0,0,0,0,0};
	static short i = 0, j = 10;
	
	i = (i+1) % 11;
	j = (j+1) % 11;
	data[j] = ADC;

	sum=0;
	for(iter0 = 10, iter1 = i; iter0 >= 0; iter0--){
		sum += Fir_low_b[iter0] * data[iter1];
		iter1=(iter1+1)%11;
	}

	pressure_sensor_val = sum/10000;
}

inline void Read_Thermister(){ //LPF
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	
	temp_sensor_val = (1-0.188)*temp_sensor_val + 0.188*ADC;
}

inline void Read_Shock(){ //IIR High
	ADCSRA |= (1 << ADIF); // ADC 변환 완료 플래그 클리어
	
	static short iter0 = 0, iter1 = 0;
	static long sum = 0;
	static short dataX[3] = {0,0,0};
	static short dataY[2] = {0,0};
	static short iX = 0, jX = 2, iY=0, jY=1;
	
	iX = (iX+1) % 3;
	jX = (jX+1) % 3;
	dataX[jX] = ADC;
	
	sum=0;
	for(iter0 = 2, iter1 = iX; iter0 >= 0; iter0--){
		sum += IIR_high_b[iter0] * dataX[iter1];
		iter1=(iter1+1)%3;
	}
	for(iter0 = 1, iter1 = iY; iter0>=0; iter0--){
		sum -= IIR_high_a[iter0] * dataY[iter1];
		iter1=(iter1+1)%2;
	}
	
	iY = (iY+1)%2;
	jY = (jY+1)%2;
	dataY[jY] = sum/10000;
	shk_sensor_val = dataY[jY];
}

inline void Reset_sensor_val(){
	pressure_sensor_val = 0;
	cds_sensor_val	= 0;
	temp_sensor_val = 0;	temp_en = 0x01;
	shk_sensor_val	= 0;	shk_detected = 0;
	psd_sensor_val	= 0;
	fire_sensor_val	= 0;	Fire_Detected = 0x00;
}

int calc_dist(){
	return (27.61/(psd_sensor_val*1.0-0.1696))*1000;
}
int calc_hz(){
	if(fire_sensor_val>=500) return 200;
	if(fire_sensor_val >= 400) return 190;
	else if(fire_sensor_val >= 350) return 180;
	else if(fire_sensor_val >= 230) return 170;
	else if(fire_sensor_val >= 180) return 150;
	else return 100;
}
int calc_force(){
	if(pressure_sensor_val <= 10)
	return 0;
	else
	return pow(2.718, (pressure_sensor_val*5.0/1024.0-0.3456)/0.6935);
}
char calc_speed(){
	return (temp_sensor_val > Thermister_MAX)?Servo_MAX_Speed:(temp_sensor_val<Thermister_MIN)?Servo_MIN_Speed:
	(Servo_MAX_Speed-Servo_MIN_Speed)/(Thermister_MAX-Thermister_MIN)*(temp_sensor_val-Thermister_MIN)+Servo_MIN_Speed;
}
int calc_led(){
	double cds=(4700.0 * 1023)/cds_sensor_val-4700.0;
	double LUX = pow(10, 1-(log(cds)-log(40000))/0.8);
	
	return (LUX > CDS_MAX)?LED_MAX:(LUX<CDS_MIN)?LED_MIN:(LED_MAX-LED_MIN)/(CDS_MAX-CDS_MIN)*(LUX-LED_MIN)+CDS_MIN;
}