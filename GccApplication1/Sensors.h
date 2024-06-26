#ifndef SENSORS_H_
#define SENSORS_H_

//Pressure Sensor
unsigned short pressure_sensor_val;

//CdS Sensor
short cds_sensor_val;

//Temperature sensor
short temp_sensor_val;
short temp_en;

//Shock Sensor
short shk_sensor_val;
char shk_detected;

//PSD Sensor
short psd_sensor_val;
char PSD_detected;
volatile short psd_dst;

//Fire sensor
short fire_sensor_val;
char Fire_Detected;

void Read_CDS();
void Read_Fire();
void Read_PSD();
void Read_Pressure();
void Read_Thermister();
void Read_Shock();

void Reset_sensor_val();

int calc_dist();
int calc_hz();
int calc_force();
char calc_speed();
int calc_led();

#endif /* SENSORS_H_ */