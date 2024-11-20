#include <PS4Controller.h>
#include <WiFiManager.h>
#include <WebServer.h>  
#include <ESP32Servo.h>
#include "DHT.h"
#include "RishabhBajaj.h"

#define LED_PIN 2                                                                                       // On board LED helpful for debugging webpage
#define MQ2_PIN 32                                                                                      // MQ2 Smoke Sensor
#define DHT11_PIN 33                                                                                    // DHT11 Temperature Humidity Sensor
#define SERVO_X_PIN 26                                                                                  // X-Axis Control Servo for ESP-Camera
#define SERVO_Y_PIN 25                                                                                  // Y-Axis Control Servo for ESP-Camera
#define DHT_TYPE DHT11                                                                                 

WiFiManager wm;                                                                                         // Object wm for WiFiManager
WebServer server(80);                                                                                   // Server on Port 80
Servo servoX;                                                                                           // Objects for Servos for X,Y Axis control of camera
Servo servoY;
DHT dht(DHT11_PIN, DHT_TYPE);                                                                           // Object for DHT11

char XML[512];                                                                                          // Webpage is updated using XML Strings
char buf[32];                                                                                           // Buffer for updating of webpages

/** 
 *  I am using the WiFiManager to dynamically allow the ESP32 to connect to any network it wants at runtime and not hardcode it
 *  However it would get assigned a different IP Address each time due to DHCP. So I add parameters in the WiFiManager interface to 
 *  allocate a user-given static IP Address so that I dont need the Serial Monitor to find the IP Address each time. 
 */

char ip_address[16] = "192.168.0.0";                                                                    // IP address of the ESP32 that you want. (This could be changed at runtime)
char default_gateway[16] = "192.168.0.0";                                                               // Default gateway of router (or any AP). (Also is changed at runtime)
char subnet_mask[16] = "255.255.255.0";                                                                 // Usually stays the same but can be changed


// You dont necessarily need to create variables for these and they could be directly sourced from PS4 library functions but this makes readability easier
int PS4_LStick_X_value;
int PS4_LStick_Y_value;
int PS4_RStick_X_value;
int PS4_RStick_Y_value;
uint8_t PS4_triangle_value;
uint8_t PS4_cicle_value;
uint8_t PS4_disconnected_bit;

// My required functions are encoded as digital values and sent to the STM to run the drive via UART communication
// Each bit specifies a certain function
uint8_t x_axis_speed_bit;                                                                               // Used for turning speed (1 = turn, 0 = no turn)
uint8_t x_axis_direction_bit;                                                                           // Used for turning direction (1 = right, 0 = left)
uint8_t y_axis_speed_bit;                                                                               // Used for front and back speed (1 = move, 0 = dont move)
uint8_t y_axis_direction_bit;                                                                           // Used for front and back direction (1 = front, 0 = back)
uint8_t uart_frame = 0;
int dead_zone_buffer = -20;                                                                             // Dead zone for analog joysticks on controller to eliminate drift

int temperature = 0;
int humidity = 0;
int smoke = 0;

int servo_angle_X = 40;                                                                                 // Angle for X-Axis control servo for camera (40 is centre aligned0
int servo_angle_Y = 70;                                                                                 // Angle for Y-Axis control servo for camera (70 is centre aligned)
unsigned long previous_servo_millis = 0;                                                                // Non-blocking delay is used to update camera servos else they move too fast
unsigned long current_servo_millis = 0;
unsigned long sensor_update = 0;                                                                        // Sensors are read periodically since DHT11 cannot process too fast
unsigned long current_server_millis = 0;                                                                // Webpage is updated periodically to prevent overloading
unsigned long previous_server_millis = 0;
int sensor_time_interval = 500;
int servo_time_interval = 12;
int server_update_time_interval = 500;
bool led_state;                                                                                         // Default on-board led state


void setup()
{
  //disableCore0WDT();                                                                                  // You may need to disable watchdog timers if webpage does not return control fast
  //disableCore1WDT();
  Serial.begin(115200);                                                                                 // Used for debugging. Not needed later on for actual operation
  Serial2.begin(115200, SERIAL_8N1, 16, 17);                                                            // UART2 for communication with STM32
  PS4.begin();                                                                                          
  dht.begin();
  initializePins();                                                                                     // Initializes GPIOS for sensors and servos
  setWiFiCredentials(ip_address, default_gateway, subnet_mask);                                         // Calls WiFiManager and sets up network
  setupServer();                                                                                        // Begins server and defines server operations
}

void loop()
{
  if(PS4.isConnected())
  {
    PS4_disconnected_bit = 0;
    getPS4Data();
    encodeData();
    handleServos();
    if ((millis() - sensor_update) >= sensor_time_interval)
    {
      sensor_update = millis();
      readSensors();
    }
  }
  else
  {
    PS4_disconnected_bit = 1;
  }
  makeFrame(x_axis_speed_bit, x_axis_direction_bit, y_axis_speed_bit, y_axis_direction_bit, PS4_triangle_value, PS4_cicle_value, PS4_disconnected_bit);
  //Serial.println(uart_frame, BIN);                                                                    // Useful for debugging output on Seial Monitor
  Serial2.write(uart_frame);                                                                            // Sends frame to STM32
  uart_frame = 0;                                                                                       // Clears the frame after sending
  current_server_millis = millis();
  if(current_server_millis - previous_server_millis >= server_update_time_interval)
  {
    previous_server_millis = current_server_millis;
    server.handleClient();                                                                              // Updates to webpage after 500ms non-blocking delay
  }
}

void initializePins()
{
  pinMode(MQ2_PIN, INPUT);
  pinMode(DHT11_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  led_state = false;
  digitalWrite(LED_PIN, led_state);
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servoX.setPeriodHertz(50);
  servoY.setPeriodHertz(50);
  servoX.attach(SERVO_X_PIN, 500, 2400);
  servoY.attach(SERVO_Y_PIN, 500, 2400);
}

void setWiFiCredentials(char static_ip[], char static_gw[], char static_sn[])
{
  WiFi.mode(WIFI_STA );                                                                                  // Set ESP32 to end up in Station Mode after WiFiManager configuration
  
  WiFiManagerParameter custom_static_ip("static_ip", "Static IP", static_ip, 16);                        // Add custom parameters to allow user to set Static IP Address for ESP32
  WiFiManagerParameter custom_static_gw("static_gw", "Gateway", static_gw, 16);
  WiFiManagerParameter custom_static_sn("static_sn", "Subnet Mask", static_sn, 16);

  wm.addParameter(&custom_static_ip);                                                                    // Add parameters to WiFiManager Webpage
  wm.addParameter(&custom_static_gw);
  wm.addParameter(&custom_static_sn);

  wm.resetSettings();                                                                                    //Comment if you want ESP32 to remember old networks and connect automatically

  if (!wm.autoConnect("SS-90 ESP32")) {
    Serial.println("Failed to connect");
  }

  strcpy(static_ip, custom_static_ip.getValue());                                                        // Get the entered values from the WiFIManger interface
  strcpy(static_gw, custom_static_gw.getValue());
  strcpy(static_sn, custom_static_sn.getValue());

  if (strcmp(static_ip, "") != 0) {                                                                      // If a valid IP Address is entered
    IPAddress local_IP, gateway, subnet;                                                                 // Retrieve the entered credentials                                                                
    local_IP.fromString(static_ip);
    gateway.fromString(static_gw);
    subnet.fromString(static_sn);

    if (!WiFi.config(local_IP, gateway, subnet)) {                                                       // Setup the network using the entered credentials
      Serial.println("Failed to configure Static IP. Reverting to DHCP.");
    }
  }

  //Serial.println("Connected to Wi-Fi!");                                                               // Needed only for debugging
  //Serial.print("IP Address: ");
  //Serial.println(WiFi.localIP());
}

void setupServer()
{
  server.on("/", SendWebsite);                                                                           // Functions can be added if more functionalities are needede
  server.on("/xml", SendXML);
  server.on("/BUTTON_0", ProcessButton_0);
  server.begin();
}

int nonDeadZoneValue(int real_value)                                                                      // Gives non-dead zone value from analog sticks on PS4 controller
{
  if((real_value < dead_zone_buffer) || (real_value > abs(dead_zone_buffer)))
    return real_value;
  else
    return 0;
}

void getPS4Data()
{
  PS4_LStick_X_value = nonDeadZoneValue(PS4.LStickX());
  PS4_LStick_Y_value = nonDeadZoneValue(PS4.LStickY());
  PS4_RStick_X_value = nonDeadZoneValue(PS4.RStickX());
  PS4_RStick_Y_value = nonDeadZoneValue(PS4.RStickY());
  PS4_triangle_value = PS4.Triangle();
  PS4_cicle_value = PS4.Circle();
}

void encodeData()
{
  // Conditions to set b0,b1,b2,b3 speed bits on UART frame
  if ((PS4_LStick_X_value == 0) && (PS4_LStick_Y_value == 0))                   
  {
    x_axis_speed_bit = 0;
    y_axis_speed_bit = 0;
  }
  else if ((abs(PS4_LStick_X_value)) > (abs(PS4_LStick_Y_value)))               //To only choose the x or y direction. Priority is given. Only x speed or y speed is going to be transferred
  {
    x_axis_speed_bit = 1;
    y_axis_speed_bit = 0;
  }
  else
  {
    x_axis_speed_bit = 0;
    y_axis_speed_bit = 1;
  }
  if ((x_axis_speed_bit != 0) && (PS4_LStick_X_value > 0))                      //If joystick is towards right, x direction is 1, else 0
    x_axis_direction_bit = 1;
  else
    x_axis_direction_bit = 0;
    
  if ((y_axis_speed_bit != 0) && (PS4_LStick_Y_value > 0))                      //If joystick is upwards/forwards, y direction is 1 else 0
    y_axis_direction_bit = 1;
  else
    y_axis_direction_bit = 0;
}

// UART frame is created by shifting bits into respective positions
void makeFrame(uint8_t x_speed, uint8_t x_direction, uint8_t y_speed, uint8_t y_direction, uint8_t speed_mode, uint8_t headlight_toggle, uint8_t ps4_disconnect)
{
  uart_frame |= y_speed << 0;
  uart_frame |= y_direction << 1;
  uart_frame |= x_speed << 2;
  uart_frame |= x_direction << 3;
  uart_frame |= speed_mode << 4;
  uart_frame |= headlight_toggle << 5;
  uart_frame |= ps4_disconnect << 7;
}


void handleServos()
{
  current_servo_millis = millis();
  if((current_servo_millis - previous_servo_millis) >= servo_time_interval)
  {
    // If joystick is towards a direction, angle for that axis is changed slowly using a non-blocking delay
    if(PS4_RStick_X_value > 0)
    {
      servo_angle_X--;
      if(servo_angle_X < 10)                                                                              // Capped to prevent mechanical damage
        servo_angle_X = 10;
    }
    else if(PS4_RStick_X_value < 0)
    {
      servo_angle_X++;
      if(servo_angle_X > 70)                                                                              // Capped to prevent mechanical damage
        servo_angle_X = 70;
    }
      
    if(PS4_RStick_Y_value > 0)
    {
      servo_angle_Y--;
      if(servo_angle_Y < 30)                                                                              // Capped to prevent mechanical damage
        servo_angle_Y = 30;
    }
    else if(PS4_RStick_Y_value < 0)
    {
      servo_angle_Y++;
      if(servo_angle_Y > 110)                                                                             // Capped to prevent mechanical damage
        servo_angle_Y = 110;
    }
    previous_servo_millis = current_servo_millis;
  } 
  if(PS4.Cross())                                                                                         // X on PS4 Controller aligns servos back to central position
  {
    servo_angle_X = 40;
    servo_angle_Y = 70;
  }
  servoX.write(servo_angle_X);                                                                            // After angle calculation, it is actually written to servos
  servoY.write(servo_angle_Y);   
}

void readSensors()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(temperature) || isnan(humidity))
  {
    humidity = 0;
    temperature = 0;
  }
  smoke = map(analogRead(MQ2_PIN), 3000, 4095, 0, 1000);                                                  // Mapping to fit on webpage. My MQ2 was not calibrated properly
   
}

void SendWebsite() 
{
  server.send(200, "text/html", PAGE_MAIN);
}

void SendXML() 
{
  // Building the XML and Buffer strings to send real-time data to the webpage
  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");
  sprintf(buf, "<TEMP>%d</TEMP>\n", temperature);
  strcat(XML, buf);
  sprintf(buf, "<HUM>%d</HUM>\n", humidity);
  strcat(XML, buf);
  sprintf(buf, "<SMOKE>%d</SMOKE>\n", smoke);
  strcat(XML, buf);

  if (led_state) 
  {
    strcat(XML, "<LED>1</LED>\n");
  }
  else 
  {
    strcat(XML, "<LED>0</LED>\n");
  }
  strcat(XML, "</Data>\n");
  Serial.println(XML);
  server.send(200, "text/xml", XML);
}

void ProcessButton_0() 
{
  led_state = !led_state;
  digitalWrite(LED_PIN, led_state);  
  server.send(200, "text/plain", ""); 
}
