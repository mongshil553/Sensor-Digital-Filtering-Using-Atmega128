/*
 * Sensors.h
 *
 * Created: 2024-05-22 오전 1:06:25
 *  Author: kijun
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

//Pressure Sensor
unsigned short pressure_sensor_val;
//외부 변수 써서 고주파를 차단해야되면 LPF, MAF이용
//예를 들어, 이전 값 저장하는 변수

//CdS Sensor
unsigned short cds_sensor_val;

//Temperature sensor
unsigned short temp_sensor_val;
unsigned short temp_en;

//Shock Sensor
unsigned short shk_sensor_val;
char shk_detected;

//PSD Sensor
unsigned short psd_sensor_val;

//Fire sensor
unsigned short fire_sensor_val;

void Read_CDS();
void Read_Fire();
void Read_PSD();
void Read_Pressure();
void Read_Thermister();
void Read_Shock();

void Reset_sensor_val();

#endif /* SENSORS_H_ */