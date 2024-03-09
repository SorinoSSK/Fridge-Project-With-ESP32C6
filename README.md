# Fridge-Project-With-ESP32C6
## Introduction
This is a project to simulate an IoT fridge and to learn ESP-IDF. The project uses Espressif's rainmaker app to track and control the IoT fridge's functionality. This IoT fridge project consist of 2 main functionality, track if the door of the fridge is closed or opened, and locking of the fridge. 

## Installation
### Install Espressif
**Step 1:**\
Download espressif from the following link:
https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32s3/get-started/index.html \
Uncheck "***Run ESP-IDF Command Prompt Environment***"", during installation.

**Step 2:**\
In Visual Studio Code, open ***Extensions*** and search for ***Espressif IDF***.

**Step 3:**\
Install ***Espressif IDF***

**Step 4:**\
Press ***Ctrl + Shift + P*** or ***F1*** and search for ```Configure ESP-IDF extension```.

**Step 5:**
1) Select "***USE EXISTING SETUP***".
2) Select "***Find ESP-IDF in your system***" under *Select ESP-IDF version*.
3) Select your espressif idf folder under *Enter ESP-IDF directory (IDF_PATH)*.\
If you did not change the installation directory in **Step 1**, the folder is usually in "***C:/Espressif/frameworks/(the idf version you have downloaded)***".
4) Select your espressif tool folder under *Enter ESP-IDF Tools directory (IDF_TOOLS_PATH)*\
If you did not change the installation directory in **Step 1**, the folder is usually in "***C:/Espressif/tools***".

### Install rainmaker
**Step 1:**\
Go to your espressif's framework folder. If you did not change the installation directory in **Step 1**, the folder is usually in "*C:/Espressif/framework*".

**Step 2:**\
git clone the following repository into the framework folder:
https://github.com/espressif/esp-rainmaker

### Download Espressif IoT libraries
git clone from the following repository:
https://github.com/espressif/esp-iot-solution/tree/272ec0e4122d2f74f66a2d93467acc9f9f4b811b 

## Getting started with this project.
**Step 1:**\
Copy ***SinKiat_Fridge*** and paste it into "***C:/Espressif/framework/esp-rainmaker/examples/***".

**Step 2:**\
Copy the ***servo*** library from *Espressif IoT Library*, "***(directory to Espressif IoT Folder)/esp-iot-solution-master/components/motor/servo***" into the components folder of your *rainmaker directory*, "***C:/Espressif/frameworks/esp-rainmaker/components/motor/***"

**Step 3:**\
Open ***SinKiat_Fridge*** with visual studio. Initialise your COM port and device in your visual studio. Build and flash the program into your device.

**Step 4:**\
Download the ***ESP RainMaker*** app from app store or google play store.

**Step 5:**\
Enjoy. :)