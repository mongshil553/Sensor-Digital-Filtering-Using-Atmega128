<h1>Term Project for Robotics Lab3.</h1>

The main idea is that with using Atmega128 and Python Server, search and locate colored marble and rotate the servo motor, enable Electromagnet so as to catch the marble.

The Python server is used to locate the marble using Hough Transform and identify its color using K-Means Clustering Algorithm.

Atmega128 and Python Sever Communicates via Bluetooth(HC-04).

<hr>
Used 6 sensors with filtering: Pressure Sensor, Thermister Sensor, CdS Sensor, PSD Sensor, Fire Detection Sensor, Vibration Detecting Sensor<br><br>

i) Pressure Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/cfa31110-c6ef-409b-82f7-17083a4fae57" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/84ea5d32-4c68-44a2-8699-76dd11d574e2" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/d2fb1386-20a0-49f2-ab2d-717f0cd18718" width="32%" height="32%" title="Actual Data After Filtering"> 

<br>ii) Thermister Sensor: Used Low Pass Filter. <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/204190dd-7046-48dc-a7b8-b071cf5f70c1" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/ffdeb258-23ef-4651-a94a-b9a53383beab" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/fb49e9db-51b6-4a96-b2f2-d84904ffa2ed" width="30%" height="30%" title="Actual Data After Filtering"> 

<br>iii) CdS Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/5ed2a1cc-3c51-489c-b580-80cf91ca57e9" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/2ce08be5-68f8-4762-8f79-0a43fbf290cf" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/4bf537e7-c7f1-4b04-b40d-e1b8503a0490" width="40%" height="40%" title="Actual Data After Filtering"> 

<br> iv) PSD Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/a43835b6-928c-400a-a7ae-0591a3ab7312" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/349da58d-2a61-4d50-9561-f647ecffbfa7" width="37%" height="37%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/f9157af7-1fcc-4a6f-a1da-ced9d48b516d" width="37%" height="37%" title="Actual Data After Filtering"> 

<br> v) Fire Detection Sensor: Used FIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/69e55dbf-2435-425e-88c8-7e24cfe24268" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/f94d337a-0b34-436d-8123-c862eb7706ae" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/52dc35e0-55b7-4769-b534-ef2d1139b653" width="32%" height="32%" title="Actual Data After Filtering"> 


<img src = "" width="24%" height="24%" title="Matlab Result">
<img src = "" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "" width="32%" height="32%" title="Actual Data After Filtering"> 
