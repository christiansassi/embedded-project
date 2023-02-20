# bLong

<div align="center">
    <p>
        <img width="100%" src="assets/images/extras/logo.png"/>
    </p>
    <div>
        <img src="https://img.shields.io/badge/c-%2300599C.svg?style=flat&logo=c&logoColor=white" alt="C"/>
        <img src="https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white" alt="C++"/>
        <img src="https://img.shields.io/badge/html5-%23E34F26.svg?style=flat&logo=html5&logoColor=white" alt="HTML"/>
        <img src="https://img.shields.io/badge/javascript-%23323330.svg?style=flat&logo=javascript&logoColor=%23F7DF1E" alt="JavaScript"/>
        <img src="https://img.shields.io/badge/css3-%231572B6.svg?style=flat&logo=css3&logoColor=white" alt="CSS"/>
    </div>
</div>


## Table of contents

-   [Introduction](#introduction)
-   [Hardware and Software Requirements](#hardware-and-software-requirements)
    -   [Hardware Requirements](#hardware-requirements)
    -   [Software Requirements](#software-requirements)
-   [Installation and Configuration](#installation-and-configuration)
    -   [Code Composer Studio - MSP432](#code-composer-studio---msp432)
        -   [Setup IDE](#setup-ide-ccs)
        -   [Import the Project](#import-the-project-ccs)
        -   [Install the Libraries](#install-the-libraries-ccs)
        -   [Run the project](#run-the-project-ccs)
    -   [Arduino IDE 2 - ESP32](#arduino-ide-2---esp32)
        -   [Setup IDE](#setup-ide-arduino)
        -   [Import the Project](#import-the-project-arduino)
        -   [Install the Libraries](#install-the-libraries-arduino)
        -   [Run the project](#run-the-project-arduino)
-   [Project Layout](#project-layout)
-   [Code Overview](#code-overview)
    - [MSP432](#msp432)
      - [Project Layout](#project-layout-msp432)
      - [Menu](#menu)
      - [Meter](#meter)
      - [Bubble Level](#bubble-level)
      - [Lidar](#lidar)
    - [ESP32](#esp32)
      - [Project Layout](#project-layout-arduino)
      - [UART Communication](#uart-communication)
      - [I2C Communication](#i2c-communication)
      - [Web Application](#web-application)
-   [Getting Started](#getting-started)
-   [Video and Presentation](#video-and-presentation)
-   [Contributions](#contributions)
-   [Contacts](#contacts)


# Introduction

The goal of this project is to develop a multi-functional device by using an MSP432P401R, an ESP32 and other sensors. Thanks to them, the device can perform three main functions:

-   **Meter**: using an EC11 20-pulse rotary encoder, the device has the ability to measure distance similar to a conventional meter.
-   **Bubble Level**: with the integration of a 3-Axis Analog Accelerometer, the device can detect inclinations along x-axis with a range of 0 to 45 degrees.
-   **Lidar distance meter**: the device uses a VL53L0XX-V2 laser sensor to detect the distance from an object with a range of 20cm to 2m.

Moreover, the ESP32 will host a real-time web application that will enable you to view all measurements in real-time.

The project is named "*bLong*" because the letter "*b*" represents Bubble Level function and "*Long*" refers to the length/distance measured by the Meter and the Lidar sensor. The pronunciation of "*bLong*" is similar to "*belong*," emphasizing that this project is a part of us.

# Hardware and Software Requirements

## Hardware Requirements

The following components were used for this project:

-   [MSP432P401R](https://www.ti.com/lit/ds/slas826e/slas826e.pdf) + [BoosterPack](https://www.ti.com/lit/ug/slau599b/slau599b.pdf?ts=1675489797355)
-   [ESP32](https://cdn.shopify.com/s/files/1/1509/1638/files/ESP_-_32_NodeMCU_Developmentboard_Datenblatt_AZ-Delivery_Vertriebs_GmbH_10f68f6c-a9bb-49c6-a825-07979441739f.pdf?v=1598356497)
-   EC11 20-pulse rotary encoder
-   [VL53L0XX-V2 laser sensor](https://cdn.shopify.com/s/files/1/1509/1638/files/VL53L0X_Time_of_Flight_Sensor_Datenblatt_AZ-Delivery_Vertriebs_GmbH.pdf?v=1608104673)
-   Power bank (optional)

<div align="center">
    <p>
        <img width="100%" src="assets/images/connection diagrams/circuit.png"/>
    </p>
    <br>
    <table>
        <tr>
            <th>MSP432 pin</th>
            <th>ESP32 pin</th>
            <th>Encoder pin</th>
            <th>VL53LXX-V2 pin</th>
            <th>Wire colour</th>
        </tr>
        <tr>
        	<td>P4.0</td>
            <td>-</td>
        	<td>OUT A</td>
            <td>-</td>
        	<td>Pink</td>
        </tr>
        <tr>
        	<td>GND</td>
            <td>-</td>
        	<td>GND</td>
            <td>-</td>
        	<td>Black</td>
        </tr>
        <tr>
        	<td>P4.2</td>
            <td>-</td>
        	<td>OUT B</td>
            <td>-</td>
        	<td>Orange</td>
        </tr>
        <tr>
        	<td>5V</td>
        	<td>5V</td>
            <td>-</td>
        	<td>-</td>
        	<td>Cyan</td>
        </tr>
        <tr>
        	<td>GND</td>
        	<td>GND</td>
            <td>-</td>
        	<td>-</td>
        	<td>Black</td>
        </tr>
        <tr>
        	<td>P3.3</td>
        	<td>16</td>
            <td>-</td>
        	<td>-</td>
        	<td>Yellow</td>
        </tr>
        <tr>
        	<td>P3.2</td>
        	<td>17</td>
            <td>-</td>
        	<td>-</td>
        	<td>Magenta</td>
        </tr>
        <tr>
            <td>-</td>
        	<td>3.3V</td>
            <td>-</td>
        	<td>VIN</td>
        	<td>Red</td>
        </tr>
        <tr>
        	<td>-</td>
        	<td>GND</td>
            <td>-</td>
        	<td>GND</td>
        	<td>Black</td>
        </tr>
        <tr>
        	<td>-</td>
        	<td>22</td>
            <td>-</td>
        	<td>SCL</td>
        	<td>Green</td>
        </tr>
        <tr>
        	<td>-</td>
        	<td>21</td>
            <td>-</td>
        	<td>SDA</td>
        	<td>Blue</td>
        </tr>
    </table>
</div>


> **Note**: if you are using this connection diagram, be aware that by directly attaching the BoosterPack to the top of the MSP432, all its upper pins will be occupied because of the BoosterPack's size. As a result, it will be necessary to connect other devices and/or sensors below the MSP432 or above the BoosterPack.

A 3D printed structure was developed for this project. Although the connection diagram remains the same, the BoosterPack is not directly connected to the MSP432 due to the limited space. Instead, as showed in the photos, the BoosterPack was connected to the MSP432 using wires to separate the two boards and optimize the space within the structure.

<p float="left" align="center">
    <img width="49%" src="assets/images/photos/3d-model1.png"/>
    <img width="49%" src="assets/images/photos/3d-model2.png"/>
</p>

## Software Requirements

The following softwares were used for this project:

-   [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO)
-   [Arduino IDE 2](https://docs.arduino.cc/software/ide-v2)

> **Note**: you can also use [Arduino IDE 1](https://docs.arduino.cc/software/ide-v1) if you prefer, however, keep in mind that in that case some steps of the setup might change.

# Installation and Configuration

## Code Composer Studio - MSP432

<h3 id="setup-ide-ccs">Setup IDE</h3>

1. Download and install the latest version of [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO).
2. Follow the setup, particularly:
    - Select custom installation and tick **SimpleLink MSP432 low power + performance MCUs**.

<h3 id="import-the-project-ccs">Import the Project</h3>

1. Import the MSP432 project by selecting **File** > **Open Projects from File Systems…**.
2. In the **Import Projects from File System or Archive**, select the project.
3. Click Finish.

<h3 id="install-the-libraries-ccs">Install the Libraries</h3>

* Download the `SIMPLELINK-MSP432-SDK` library [here](https://www.ti.com/tool/download/SIMPLELINK-MSP432-SDK/3.40.01.02), then:

  1. In the **Project Explorer**, right click on the project folder, then go to **Properties**. Alternatively, you can press `Alt+Enter` on Windows or `Cmd+Enter` on Mac.
  2. Go to **Build** > **Arm Compiler** > **Include Options** and add the following path:
        - `simplelink_msp432p4_sdk_3_40_01_02\source` (or the path where you saved the lib).
  
  
    3. Go to **Build** > **Arm Linker** > **File Search Path** and add the following paths:
        - `simplelink_msp432p4_sdk_3_40_01_02\source\ti\grlib\lib\ccs\m4\grlib.a` (the path where you saved the lib).
        - `simplelink_msp432p4_sdk_3_40_01_02\source\ti\devices\msp432p4xx\driverlib\ccs\msp432p4xx_driverlib.lib` (the path where you saved the lib).
  
* Put the `LcdDriver` folder located at `MSP432P401R/libraries` inside the project folder.


<h3 id="run-the-project-ccs">Run the Project</h3>

Click on **Flash** to upload the script to the MSP432.

> **Note**: ensure that the option "*Build Project Before Load*" is checked. If not, you can check it to automatically build the project during the flashing phase, or you can do it manually before flashing the script.

## Arduino IDE 2 - ESP32

<h3 id="setup-ide-arduino">Setup IDE</h3>

1. Download and install the latest version of [Arduino IDE 2](https://docs.arduino.cc/software/ide-v2).
2. Execute the setup.
3. Start Arduino IDE 2.0.
4. Go to **File** > **Preferences** and enter the following into the **Additional Board Manager URLs**: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json.
5. Go to **Tools** > **Board** > **Boards Manager** and search for ESP32. Install the **ESP32 by Espressif Systems**.
6. Go to **Tools** > **Board** > **Board Manager** > **esp32** and select **ESP32 Dev Module**.

<h3 id="import-the-project-arduino">Import the Project</h3>

Go to **File** > **Open** and search for the `esp32.ino` file under `ESP32/src`.

<h3 id="import-the-libraries-arduino">Import the Libraries</h3>

1. Go to **Tools** > **Manage Libraries**. With the **Library Manager** menu on the left, search and install the following libraries:
    - [Adafruit_VL53L0X by Adafruit](https://github.com/adafruit/Adafruit_VL53L0X).
    - [AsyncTCP by dvarrel](https://github.com/dvarrel/AsyncTCP).
2. Install the **ESPAsyncWebServer** library from [here](https://github.com/me-no-dev/ESPAsyncWebServer). Once the download is completed, extract the `.zip` folder and rename it as **ESPAsyncWebServer**. Finally, move the ESPAsyncWebServer folder to your Arduino IDE installation libraries folder.
3. Finally, re-open your Arduino IDE.

<h3 id="run-the-project-arduino">Run the Project</h3>

Click on the **Upload** button.

> **Note**: if you encounter issues uploading the script to the ESP32, try holding down the **BOOT** button of the ESP32 while you see the message "*Connecting...*" displayed in the console. Release the button once the upload process begins.

# Project Layout

```
bLong
 ┣ assets 				# Project resources folder
 ┃ ┣ 3D case 				# 3D case folder
 ┃ ┗ images 				# Images folder
 ┣ ESP32 				# ESP32 folder
 ┃ ┣ libraries 				# ESP32 libraries
 ┃ ┗ src 				# ESP32 source code 
 ┣ MSP432P401R 				# MSP432 folder
 ┃ ┣ libraries 				# MSP432 libraries
 ┃ ┗ src 				# MSP432 source code
```

# Code Overview

## MSP432

<h3 id="project-layout-msp432">Project Layout</h3>

```
MSP432
 ┣ libraries						# Libraries to be included in the project (see Installation and Configuration)
 ┃ ┣ LcdDriver
 ┗ src
 ┃ ┣ LcdDriver						# Support for the Lcd screen of the BoosterPack board
 ┃ ┣ Gr_bLong.c						# Splash screen image
 ┃ ┣ Gr_bubbleLevel.c					# Bubble Level interface
 ┃ ┣ Gr_lidarInit.c					# Lidar interface
 ┃ ┣ Gr_menu.c						# Menu interface
 ┃ ┣ Gr_meterInit.c					# Initial Meter interface
 ┃ ┣ Gr_meterMeasure.c					# Running Meter interface
 ┃ ┣ main.c						# Main
 ┃ ┣ msp432_bubbleLevel.c				# Manages the Bubble Level tool 
 ┃ ┣ msp432_bubbleLevel.h				# //
 ┃ ┣ msp432_init.c					# Initializes hardware
 ┃ ┣ msp432_init.h					# //
 ┃ ┣ msp432_lidar.c					# Manages the Lidar distance tool
 ┃ ┣ msp432_lidar.h					# //
 ┃ ┣ msp432_menu.c					# Manages the Menu
 ┃ ┣ msp432_menu.h					# //
 ┃ ┣ msp432_meter.c					# Manages the Meter tool
 ┃ ┣ msp432_meter.h					# //
 ┃ ┣ msp432_uart.c					# Manages the UART communication
 ┃ ┣ msp432_uart.h					# //
 ┃ ┣ msp432_utils.c					# Set of utility functions (graphic, time etc...)
 ┃ ┣ msp432_utils.h					# //
```

All the `Gr_*.c` files contain images that have been converted into hexadecimal format for be embedded into the script.

### Menu

Navigate through the different modes using the joystick and select a mode by pressing the `A` button located on the top of the BoosterPack board. This is accomplished within the `msp432_menu.*` files. 

> **Note**: due to hardware problems/limitations, the buttons on the BoosterPack sometimes send multiple interrupts. To avoid this issue, a minimum time delay of `MIN_DELAY` seconds has been implemented in the `canClickA()` and `canClickB()` functions before another button press can occur.

> **Note**: due to hardware problems/limitations, the joystick on the BoosterPack sometimes generates false movement signals. To mitigate this issue, it has been set an higher sensitivity value by adjust the `SENSITIVITY` variable. However, it's important to note that setting the sensitivity value too high may negatively impact the use of the joystick, making it difficult to navigate between modes.

The navigation through the various modes is achieved by using the joystick. The MSP432 checks the delta between the previous and current position of the joystick every time it is moved. If the delta is greater than the sensitivity value minus 10, the menu interface will be updated and a new item will be selected. This is achieved through the use of the `joystickMenu(...)` function, which captures and evaluates the joystick movement, and the `updateMenu(...)` function, which updates the menu.

When a mode is selected, the MSP432 will send its corresponding index to the ESP32 to keep the two devices synchronized.

### Meter

The Meter function is made possible by the encoder. This is accomplished within the `msp432_meter.*` files. 

The `encoderInit(...)` function is first called to turn off the LED and display the initial interface. The tool can be controlled by clicking the `A` and `B` buttons. Pressing the A button starts and stops the measurement, while pressing the B button returns to the menu. It's important to note that the B button only has an effect when the measurement is not in progress, otherwise it will have no effect.

When the measurement is initiated, the system calls `encoderHandler(...)` function where it enters a loop where it constantly monitors the input from the encoder. If the sensor produces a valid input (i.e., the current clock value read from the encoder is different from the previous one), it is interpreted as an increment or decrement based on the direction of rotation. The direction can be determined by comparing the direction value with the current clock value. If they are equal, it is an increment, otherwise a decrement. The encoder used in this project is a 20-pulse encoder, so each step corresponds to *2πr / 20*, where *r* is the radius of the wheel and 20 is the number of pulses generated by the encoder in a complete rotation. The captured value is then transmitted to the ESP32.

### Bubble Level

The Bubble Level function is made possible by the 3-Axis Analog Accelerometer. However, this tool only displays the inclination angle along the x-axis. This is accomplished within the `msp432_bubbleLevel.*` files. 

The Bubble Level mode is initialized by calling the `bubbleInit(...)` function. Then, the interrupt generated by the accelerometer is captured and the value registered along the x-axis is used in the `drawRect(...)` function. This value is converted to an angle using a proportion and displayed every `N_CAPTURES` to prevent the display from flickering. To make the bubble movement more fluid, this setting only affects the text display and not the graphics. The captured value is then transmitted to the ESP32.

### Lidar

The Lidar distance function is made possible by the Lidar sensor. This is accomplished within the `msp432_lidar.*` files. 

For this project, the lidar sensor is connected to the ESP32 which acts as the master and the MSP432 as the slave. The communication between the two board is the same of the one used for the communication between the MSP432 (master) and ESP32 (slave) but switched. The MSP432 will send a message containing a 1 each time it desiders a new measure and a message containing 2 if the back button is pressed. 

## ESP32

<h3 id="project-layout-arduino">Project Layout</h3>

```tex
ESP32
 ┣ libraries
 ┃ ┣ Adafruit_VL53L0X				# Library used for the Lidar sensor
 ┃ ┣ AsyncTCP					# Library used for the Web Application
 ┃ ┗ ESPAsyncWebServer				# Library used for the Web Application
 ┗ src
 ┃ ┣ esp32.ino					# Arduino project
 ┃ ┣ esp32.svd						
 ┃ ┣ esp32_async_server.cpp			# Manages the Web Application
 ┃ ┣ esp32_async_server.h			# //
 ┃ ┣ esp32_lidar.cpp				# Manages the Lidar sensor
 ┃ ┣ esp32_lidar.h				# //
 ┃ ┣ esp32_uart.cpp				# Manages UART communication
 ┃ ┗ esp32_uart.h				# //
```

### UART Communication

In this project, UART 2 is used for the UART communication. The transmission of data from ESP32 to MSP432 is achieved through the connection between pin 17 of the ESP32 and P3.2, while the connection between pin 16 of the ESP32 and P3.3 is used for receiving data. This is accomplished within the `esp32_uart.*` files. In addition, the `UARTClass` provides the `UART` object, which facilitates the management of UART communication between the two devices.

The communication is initialized by creating an instance of the `HardwareSerial` class, `SerialPort`, where it is specified the UART version used. Then, the `SerialPort` object is further customized with other parameters.

- **UART_BAUDRATE** specifies the baud rate used by the ESP32 and it has to be the same for the MSP432.
- **UART_SERIAL** specifies the length of each message.
- **UART_RXD** specifies the pin used for reception.
- **UART_TXD** specifies the pin used for transmission.

```c++
this->SerialPort.begin(UART_BAUDRATE, UART_SERIAL, UART_RXD, UART_TXD);
```

> **Note**: in the `begin(...)` function of the `SerialPort` object, there are other adjustable parameters, including `invert`, `timeout_ms` and `rxfifo_full_threshold`, however, for this project, they are left unchanged at their default values.

Finally, the received message can be retrieved by calling the `getMessageUART()` function of the UART object. The ESP32 first checks the availability of the serial port using the `Serial.available()` function, and if it is available, it reads the port's buffer using `Serial.read()`.

Each message has a maximum length of 8 bits. To enable communication between the MSP432 and the ESP32, the MSP432 sends a start and end bit for each message and they can vary. The value to be sent is split into 8-bit chunks, so the number of chunks can vary based on the size of the value and at the end they are reassembled. To preserve decimal values and work with integer numbers only, the value is multiplied by 10 or 100, converted to an integer, and sent. In this way it's possible to preserve up to two decimal digits. Finally, the end bit is then transmitted. 

These are the start/stop bits used:

* **Meter**: the bits are equal to 0 since it was decided to transmit the increment detected by the encoder. If we transmitted the measured value, it could have been equal to 0, which would have caused the ESP32 to confuse it as a start/stop bit. By sending the increment, we ensure that it will never be equal to 0, as otherwise the meter function would not work. In addition, to minimize the number of packets sent, the MSP432 sends a sign bit after the start bit to indicate if the value is positive or negative. A value of 1 is used for negative numbers and 2 for positive ones, which allows us to use all 8 bits to represent a positive number. 

> **Note**: due to hardware problems/limitations, the encoder sends the same message twice. For this reason, `skip_message` variable in `esp32_uart.cpp` tells the ESP32 to ignore the duplicated message that comes after the first one.

* **Bubble Level**: the bits have a value of 50, but they can be set to any value above 45 as the Bubble Line transmits values within the range of 0 to 45.
* **Lidar distance meter**: the logic used for this mode is the same as the one used in the Meter mode. However, this time, the roles of the transmitter and receiver are switched. In this case, the ESP32 acts as the master and the MSP432 as the slave, as the Lidar sensor is connected to the ESP32. 

> **Note**: due to hardware problems/limitations, the MSP432 has some problems regarding the reception of the messages from the ESP32. For this reason, a delay was put in the `_sendMessage(...)` function.

> **Note**: using specific values as "system messages" reduces the available range of numbers for the MSP432 and ESP32, as these values may conflict with the more precise encoders. For example, using 2 as the sign bit for the meter mode may compromise a sensor with a sensitivity as low as 0.02 centimeters or less, as the system would not be able to distinguish between the sign bit and the actual detected value. While it's possible to create a more complex system, it's not necessary for this project due to hardware limitations set by the sensors used.

### I2C Communication

For the I2C communication between the ESP32 and the VL53L0XX-V2 Lidar sensor, the ESP32 uses the address `0x29` to communicate with the sensor. This is accomplished within the `esp32_lidar.*` files. In addition, the `LidarClass` provides the `Lidar` object, which facilitates the management of I2C communication between the ESP32 and the sensor.

This communication is initiated by configuring the sensor using the `lox.configSensor(...)` function, where `lox` is an instance of the `Adafruit_VL53L0X` class created specifically for interacting with this sensor. There are several configuration options available. In this project, it was chosen the configuration that allows for measurement of longer distances (up to 2 meters), despite the loss of precision. The sensor is then initialized using the `begin()` function of the lox object.

Finally, the captured data from the sensor can be obtained by using the `getMeasureLidar()` function. This function saves the data in a variable named `measure`. By examining the measure object, it's possible to determine if the detected distance is out of range, meaning the Lidar sensor did not receive a return signal. If everything is functioning properly and the `RangeStatus` variable returns a code other than 4, the distance value can be accessed by reading the `RangeMilliMeter` variable of the `measure` object. It's then possible to send the measure to the MSP432 by calling the `sendMessage(...)` function of the `UART` object.

### Web Application

The ESP32 hosts a Web Application, which generates a hotspot to provide access to a dedicated web page to view real-time measurements. This is accomplished within the `esp32_async_server.*` files. In addition, the `ServerClass` provides the `Server` object, which facilitates the management of the Web Application.

The Web Application is started by the `startServer()` function, which also initiates the ESP32 hotspot with the following characteristics:

- **SERVER_SSID** defines the SSID of the ESP32 hotspot.
- **SERVER_PASSWORD** sets the password for the hotspot.
- **SERVER_CHANNEL** sets the channel of the hotspot.
- **SERVER_SSID_HIDDEN** determines if the hotspot should be kept hidden.
- **SERVER_MAX_CONNECTION** specifies the maximum number of allowed connections to the hotspot.

```c++
WiFi.softAP(SERVER_SSID, SERVER_PASSWORD, SERVER_CHANNEL, SERVER_SSID_HIDDEN, SERVER_MAX_CONNECTION);
```

>**Note**: there are other hotspot features that can be modified. However, for this project, the main ones have been changed and the others have been left unchanged at their default values.

When the Web Application is started, a callback function is created for each HTTP endpoint using the following code:

```c++
this->server.on(URL, METHOD, [&](AsyncWebServerRequest *request){
	request->send_P(CODE, CONTENT_TYPE, CALLBACK);
});
```

In this code, the parameters are:

- **URL** specifies the URL of the HTTP endpoint associated with the callback function.
- **METHOD** specifies the HTTP method, such as HTTP_GET for GET requests.
- **CODE** specifies the HTTP response code, such as 200 for a successful response.
- **CONTENT_TYPE** specifies the type of content returned in the response, such as text/html.
- **CALLBACK** specifies the function that is triggered when the HTTP endpoint is accessed.

Finally, the Web Application retrieves information from the ESP32 server in real-time through the use of JavaScript and XMLHttpRequest.

> **Note**: it is possible to upload resources on the ESP32 using Arduino IDE 1. Unfortunately, at the time of creating this project, this function is not supported in version 2 of the IDE. As a result, the HTML file of the Web Application is embedded in the `esp32_async_server.h` file within the `index_html` variable.

# Getting Started

Once you have uploaded the scripts to both the MSP432 and ESP32 and done all the connections, you can start using the multi-functional device. Use the menu to select one of the three available functions: Meter, Bubble Level, or Lidar distance meter. You can view the real-time measurement on the MSP432's integrated display or by connecting to the ESP32's hotspot and navigating to [192.168.4.1](http://192.68.4.1), where you can see the current mode, the current measurement, and the last measurement.

> **Note**: the default **SSID** and **password** of the ESP32's hotspot are `ESP32` and `12345678`.

# Video and Presentation

Video

[Presentation](https://www.canva.com/design/DAFaXvxzNeQ/_uzlmAVW-CrFlIxJXbTDzw/edit?utm_content=DAFaXvxzNeQ&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)

# Contributions

This project was developed by three students from the University of Trento, each of whom focused on a specific aspect of the project:

<table>
    <tr>
        <td>Hardware</td>
        <td>Luca Pedercini, Pietro Bologna, Christian Sassi</td>
    </tr>
    <tr>
        <td>Menu and graphics</td>
        <td>Pietro Bologna, Christian Sassi</td>
    </tr>
    <tr>
        <td>Meter</td>
        <td>Pietro Bologna</td>
    </tr>
    <tr>
        <td>Bubble Level</td>
        <td>Luca Pedercini</td>
    </tr>
    <tr>
        <td>Lidar distance meter</td>
        <td>Luca Pedercini, Pietro Bologna, Christian Sassi</td>
    </tr>
    <tr>
        <td>ESP32</td>
        <td>Christian Sassi</td>
    </tr>
    <tr>
        <td>UART communication</td>
        <td>Pietro Bologna, Christian Sassi</td>
    </tr>
    <tr>
        <td>Code review and management</td>
        <td>Luca Pedercini, Pietro Bologna, Christian Sassi</td>
    </tr>
</table>

# Contacts

Luca Pedercini - [luca.pedercini@studenti.unitn.it](mailto:luca.pedercini@studenti.unitn.it)

Pietro Bologna - [pietro.bologna@studenti.unitn.it](mailto:pietro.bologna@studenti.unitn.it)

Christian Sassi - [christian.sassi@studenti.unitn.it](mailto:christian.sassi@studenti.unitn.it)

<a href="https://www.unitn.it/"><img src="assets/images/extras/unitn-logo.png" width="300px"></a>
