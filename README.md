<h1>Term Project for Robotics Lab3.</h1>

The main idea is that with using Atmega128 and Python Server, search and locate colored marble and rotate the servo motor, enable Electromagnet so as to catch the marble.

The Python server is used to locate the marble using Hough Transform and identify its color using K-Means Clustering Algorithm.

Atmega128 and Python Sever Communicates via Bluetooth(HC-04).

<hr>
<h3>Following images are Hardware Images</h3> <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/a30a8e43-35bf-4413-9689-e635dedbe66d" width="60%" height="60%">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/f0fe351e-fe2f-460a-aeb0-420fb8e6e8ba" width="37%" height="37%"> <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/da8a83f0-487c-4523-9f41-98ddb99bf2c7" width="24%" height="24%">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/5153a1b4-eed2-446b-acd7-7093159d24e9" width="24%" height="24%">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/c22e4d8c-d1f7-498a-9418-22671be9f99d" width="24%" height="24%"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/44a12962-f3d4-430a-bc28-6fc79d082e1c" width="24%" height="24%"> <br>

<hr>
<h3>Following images are circuit images</h3>
Main board with Atmega128 is prepared by the lab so images are not uploaded to this git. <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/2fdb8b02-3d12-43a9-930c-4d58ccc44dfe" width="30%" height="30%">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/ebf3f4ed-36fc-4613-8492-7856194834e0" width="30%" height="30%"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/6eb4d559-89a6-49b6-938b-fffb06fa233a" width="30%" height="30%"><br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/0b9f1389-e79b-4f50-87aa-c9efeeb0c1ec" width="40%" height="40%">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/f6a02f39-be46-49dc-9908-de5f209e5433" width="50%" height="50%">

<hr>
<h3>Following are informations about Sensors</h3>
Used 6 sensors with filtering: Pressure Sensor, Thermister Sensor, CdS Sensor, PSD Sensor, Fire Detection Sensor, Vibration Detecting Sensor<br><br>

i) Pressure Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/cfa31110-c6ef-409b-82f7-17083a4fae57" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/84ea5d32-4c68-44a2-8699-76dd11d574e2" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/d2fb1386-20a0-49f2-ab2d-717f0cd18718" width="32%" height="32%" title="Actual Data After Filtering"> 

<br>ii) Thermister Sensor: Used Low Pass Filter. <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/204190dd-7046-48dc-a7b8-b071cf5f70c1" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/1e78bd2f-a2ef-4209-a3c7-278172e30d39" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/877ffd15-7668-4369-b2ca-e3e24255470e" width="32%" height="32%" title="Actual Data After Filtering"> 

<br>iii) CdS Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/5ed2a1cc-3c51-489c-b580-80cf91ca57e9" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/4bb0b58e-be20-47a8-a55f-50badef11591" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/23713133-dcf5-4094-9489-396600bbe0dc" width="32%" height="32%" title="Actual Data After Filtering"> 

<br> iv) PSD Sensor: Used IIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/a43835b6-928c-400a-a7ae-0591a3ab7312" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/6c55aa76-30b4-4b2a-b33e-fd8e9de5b9b0" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/3214ad98-c872-41d1-87d0-b5f1f7f3ae42" width="32%" height="32%" title="Actual Data After Filtering"> 

<br> v) Fire Detection Sensor: Used FIR Low Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/69e55dbf-2435-425e-88c8-7e24cfe24268" width="24%" height="24%" title="Matlab Result"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/a31737ff-20c8-4dcc-aae4-7938781a4660" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/0b4688d0-ad69-43f2-8f4a-d11ac3359417" width="32%" height="32%" title="Actual Data After Filtering"> 

<br> vi) Vibration Detecting Sensor: Used IIR High Filter(Butterworth). <br>
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/ac4e1cb8-78ce-4cbc-9667-da3549b94cd2" width="24%" height="24%" title="Matlab Result">
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/e33ddb3e-c5f3-48e2-891e-232034ed7cef" width="32%" height="32%" title="Actual Data Before Filtering"> 
<img src = "https://github.com/mongshil553/GccApplication1/assets/129606995/056dea6c-5846-40fd-bb9b-f975c278f117" width="32%" height="32%" title="Actual Data After Filtering"> 
