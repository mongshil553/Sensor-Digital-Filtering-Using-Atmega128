This is a Term Project for Robotics Lab3.

The main idea is that with using Atmega128 and Python Server, search and locate colored marble and rotate the servo motor, enable Electromagnet so as to catch the marble.

The Python server is used to locate the marble using Hough Transform and identify its color using K-Means Clustering Algorithm.

Atmega128 and Python Sever Communicates via Bluetooth(HC-04).

Used 6 sensors with filtering: Pressure Sensor, Thermister Sensor, Cds Sensor, PSD Sensor, Fire Detection Sensor, Vibration Detecting Sensor

i) Pressure Sensor
Used IIR Low Filter(Butterworth).

<img src = ![image](https://github.com/mongshil553/GccApplication1/assets/129606995/cfa31110-c6ef-409b-82f7-17083a4fae57) width="50%" height="50%">


