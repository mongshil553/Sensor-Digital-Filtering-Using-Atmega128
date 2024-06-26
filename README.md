<h1>Term Project for Robotics Lab3.</h1>

The main idea is that with using Atmega128 and Python Server, search and locate colored marble and rotate the servo motor, enable Electromagnet so as to catch the marble.

The Python server is used to locate the marble using Hough Transform and identify its color using K-Means Clustering Algorithm.

Atmega128 and Python Sever Communicates via Bluetooth(HC-04).

<hr>
Used 6 sensors with filtering: Pressure Sensor, Thermister Sensor, Cds Sensor, PSD Sensor, Fire Detection Sensor, Vibration Detecting Sensor<br><br>

i) Pressure Sensor
Used IIR Low Filter(Butterworth).

<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/cfa31110-c6ef-409b-82f7-17083a4fae57" width="30%" height="30%" title="Matlab Result">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/33d1be96-94ed-426a-99b4-42c35bfa9606" width="30%" height="30%" title="Actual Data Before Filtering">

<br> &nbsp&nbsp&nbsp; Matlab Result

