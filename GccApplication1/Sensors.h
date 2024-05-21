/*
 * Sensors.h
 *
 * Created: 2024-05-22 오전 1:06:25
 *  Author: kijun
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

//Pressure Sensor
volatile char pressure_sensor_val;
//외부 변수 써서 고주파를 차단해야되면 LPF, MAF이용
//예를 들어, 이전 값 저장하는 변수

//CdS Sensor
volatile char cds_sensor_val;

//Temperature sensor
volatile char temp_sensor_val;
volatile char temp_en;

//Shock Sensor
volatile char shk_sensor_val;
volatile char shk_detected;

//PSD Sensor
volatile char psd_sensor_val;

//Fire sensor
volatile char fire_sensor_val;

void Read_CDS();
void Read_Fire();
void Read_PSD();
void Read_Pressure();
void Read_Thermister();
void Read_Shock();



#endif /* SENSORS_H_ */