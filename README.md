<h1>Term Project for Robotics Lab3.</h1>

The main idea is that with using Atmega128 and Python Server, search and locate colored marble and rotate the servo motor, enable Electromagnet so as to catch the marble.

The Python server is used to locate the marble using Hough Transform and identify its color using K-Means Clustering Algorithm.

Atmega128 and Python Sever Communicates via Bluetooth(HC-04).

<hr>
Used 6 sensors with filtering: Pressure Sensor, Thermister Sensor, CdS Sensor, PSD Sensor, Fire Detection Sensor, Vibration Detecting Sensor<br><br>

i) Pressure Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/cfa31110-c6ef-409b-82f7-17083a4fae57" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/33d1be96-94ed-426a-99b4-42c35bfa9606" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/90d291e6-067b-40af-b5df-d8e2e7d47071" width="32%" height="32%" title="Actual Data After Filtering"> 

<br>ii) Thermister Sensor: Used Low Pass Filter. <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/204190dd-7046-48dc-a7b8-b071cf5f70c1" width="24%" height="24%" title="Matlab Result">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/6f36ff2a-3df1-445e-b15f-0a8246113020" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/b5cb4414-c2e5-451b-8bd5-8f09221c31d2" width="30%" height="30%" title="Actual Data After Filtering"> 

<br>iii) CdS Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/5ed2a1cc-3c51-489c-b580-80cf91ca57e9" width="24%" height="24%" title="Matlab Result">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/440e30b0-6e98-4c18-8d2f-d422d210b199" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/91696f82-a984-4875-9268-270c27b3fc12" width="40%" height="40%" title="Actual Data After Filtering"> 

<br> iv) PSD Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/a43835b6-928c-400a-a7ae-0591a3ab7312" width="24%" height="24%" title="Matlab Result">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/349da58d-2a61-4d50-9561-f647ecffbfa7" width="37%" height="37%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/cec50a78-4d11-42de-9d00-a42cf59d0a49" width="37%" height="37%" title="Actual Data After Filtering"> 

<img src = "" width="24%" height="24%" title="Matlab Result">
<img src = "" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "" width="32%" height="32%" title="Actual Data After Filtering"> 
