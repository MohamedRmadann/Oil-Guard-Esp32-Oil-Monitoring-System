<h1>Oil Guard: Esp32 Oil Monitoring System</h1>


<h2>Description</h2>
The project consists of an ESP32 microcontroller system integrated with sensors, including a DS18B20 temperature module and a Hall effect sensor, used to monitor oil parameters. A JavaScript script is used to automatically upload and store the data securely in Google Sheets as cloud storage. A mobile application, built using the MIT App Inventor platform, enables real-time monitoring of oil parameters and sends alerts when thresholds are exceeded, utilizing the built-in WiFi communication module of the ESP32 for communication, and Firebase for Real-Time storing. Additionally, a Python script leveraging Pandas and Matplotlib libraries is used to manipulate and visualize the collected data for deeper analysis.<br />
<!-- 
OilGuard is an IoT-based smart monitoring system designed to help fleet managers and vehicle owners track oil quality in real-time. Using an ESP32 device, the system collects data on oil condition and temperature, uploads it automatically to Google Sheets for secure cloud storage, and visualizes it through interactive graphs. A dedicated mobile app gives users instant access to live updates, maintenance alerts, and long-term trends, empowering smarter decisions and predictive maintenance to reduce costs and extend vehicle life.
-->

<h2>Utilities Used</h2>

- <b>Arduino IDE</b> 
- <b>FireBbse</b>
- <b>MIT App Inventor</b>
- <b>Google Sheet</b>
- <b>FireBbse</b>

<h2>Languages Used</h2>

- <b>Arduino Simplified C++
- Python
- Javascript 

<h2>Program walk-through:</h2>
<h3>Esp32 system code</h3>

<p align="center">
Defining all libraries: <br/>
<img src="https://i.imgur.com/59xKYBz.png" height="70%" width="80%" alt="Disk Sanitization Steps"/>
<br />
<br />
Define Varibales related to WiFi, Firebase and Google sheet:  <br/>
<img src="https://i.imgur.com/mkF62wm.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
<br />
  Define Pins connections, Constants, and Create Objects needed as Stepper motor:  <br/>
<img src="https://i.imgur.com/HbtFFCs.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
<br />
<br />
Create a function to setup the WiFi connection: <br/>
<img src="https://i.imgur.com/Pvg7o0r.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
<br />
<br />
Create a funcion to send data and restart the Google sheet:  <br/>
<img src="https://i.imgur.com/j29kmWx.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
<br />
<br />
Setup Function:  <br/>
<img src="https://i.imgur.com/ICqVOWi.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
</br>
<br />
</p>

<h3>Mobile app</h3>
<p align="center">
Mobile app Screen Example code blocks:  <br/>
<img src="https://i.imgur.com/grILBGo.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
<br />
</p>

<h3>Google sheet script</h3>
<p align="center">
<br/>
<img src="https://i.imgur.com/BMny813.png" height="80%" width="80%" alt="Disk Sanitization Steps"/>
<br />
</p>


<!--
 ```diff
- text in red
+ text in green
! text in orange
# text in gray
@@ text in purple (and bold)@@
```
--!>
