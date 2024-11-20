# Searcha and Survillance Bot 

This repository contains a set of projects and driver files for the STM32F401RE microcontroller, designed for interfacing with various sensors, displays, and peripherals. The repository includes STM32 driver code, Arduino-based sensor control, ESP32-based wireless communication, and a dynamic wireless network system, where data from various sensors can be displayed on a personal web page and processed through UART.

## Repository Structure

This repository is organized into several folders and files, each containing different components of the system. Below is a breakdown of each major section:

### 1. **inc Folder**  
The `inc` folder contains all the header files for the peripherals and drivers for the STM32F401RE microcontroller. Each peripheral driver is developed from scratch, ensuring full control and customization over the hardware.

- **STM32 Peripheral Headers:** These headers define the interfaces for the various peripherals such as UART, GPIO, I2C, etc., necessary for the system’s operations.

### 2. **src Folder**  
The `src` folder contains all the source files for the project. These files implement the logic defined in the headers within the `inc` folder.

- **Peripheral Drivers:** This includes the source code for each peripheral driver that interfaces directly with the STM32 hardware. 
- **Main Control Code:** The `main.c` file contains the central logic for the STM32F401RE, handling communication with various peripherals, sensors, and modules.

### 3. **main.c & survillance.c**  
The `main.c` file contains the main driver code for the STM32F401RE. It initializes all peripherals, manages communications, and processes incoming data from external modules.

- **survillance.c** contains additional code for surveillance functionality, which might include processing sensor data, controlling peripherals, and handling decision-making processes for the bot.

### 4. **Arduino Nano Sensor Control**  
There are two separate Arduino Nano programs:

- **Sensor and Display Control:** One program is used for controlling various sensors and managing the display output. It reads data from connected sensors (e.g., temperature, humidity) and displays it on an attached display (e.g., LCD, OLED).
  
- **Encoder Data Collection:** The second Arduino Nano code collects encoder data and sends it to the STM32F401RE for processing. This allows for precise measurement of rotation or position based on encoder readings.

### 5. **ESP32 Communication and Server Hosting**  
The ESP32 code is responsible for the wireless communication aspects of the system:

- **PS4 Controller Communication:** The ESP32 connects to a PS4 controller via Bluetooth and sends the controller’s input data to the STM32F401RE via UART for further processing. This allows for remote control of the system via the PS4 controller.

- **Sensor Data Display and Hosting:** The ESP32 also acts as a host for a personal web server that displays sensor data collected by various sensors. This server can be accessed by any device connected to the ESP32's WLAN network. The web server is dynamic and allows users to view real-time sensor data without the need for an internet connection.

### 6. **Camera Control and Web Display**  
This file controls the camera and streams the output to a personal web page. It uses the ESP32 to host the camera feed, which can be accessed by any device connected to the system’s WLAN.

- **Camera Hosting:** The camera output is displayed on a web page, which can be accessed through the ESP32's hosted WLAN. Users can see live images or video captured by the camera.

### 7. **Wireless Networking (WLAN)**  
The entire wireless network is hosted by the ESP32, which acts as an access point for the bot. Devices such as smartphones or laptops that support 2.4 GHz Wi-Fi can connect to the bot’s WLAN and access various services, such as the web server displaying sensor data or the camera feed.

- **No Internet Connection Required:** The bot’s wireless network does not rely on an internet connection. Instead, it creates a local network where any device with Wi-Fi and hotspot capabilities can act as a client or access point for the bot.

### Features Overview:
- **STM32F401RE Microcontroller:** Central processing unit for handling peripheral devices and sensor data.
- **Wireless Communication via ESP32:** Interfaces with PS4 controller, hosts web servers, and manages sensor data transmission.
- **Arduino Nano Integration:** Used for controlling sensors and collecting encoder data.
- **Camera Streaming:** Real-time camera feed displayed on a personal web page.
- **Dynamic WLAN Network:** Devices connect via the ESP32’s hotspot to interact with the system’s data and functionalities.

---

