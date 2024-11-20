#include "esp_camera.h"
#include <WiFiManager.h>


/** 
 *  This code was sourced directly from the Camera WebServer Built-in Example. I have only added the WiFiManager aspect to allow dynamic networking
 *  instead of hardcoding the WiFi credentials, similar to the ESP32. I have also changed the Webpage code to make it simpler and the way I wanted it
 *  - index_html_gz.h
 *  No other changes have been made to the Camera WebServer Example. Feel free to optimize this code as your requirement.
 */
WiFiManager wm;

// Details for netowrking that will be changed at runtime
char ip_address[16] = "192.168.0.0";
char default_gateway[16] = "192.168.0.0";
char subnet_mask[16] = "255.255.255.0";

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

void startCameraServer();
void setupLedFlash(int pin);

void setup() {
  pinMode(4, OUTPUT);
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_SVGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  SetWiFiCredentials(ip_address, default_gateway, subnet_mask);
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print(WiFi.localIP());
}

void loop() {
  // Do nothing. Everything is done in another task by the web server`
  delay(10000);
}

// Code added to handle dynamic networking
void SetWiFiCredentials(char static_ip[], char static_gw[], char static_sn[])
{
  WiFi.mode(WIFI_STA );

  // Create custom parameter fields for IP, Gateway, and Subnet
  WiFiManagerParameter custom_static_ip("static_ip", "Static IP", static_ip, 16);
  WiFiManagerParameter custom_static_gw("static_gw", "Gateway", static_gw, 16);
  WiFiManagerParameter custom_static_sn("static_sn", "Subnet Mask", static_sn, 16);

  // Add these parameters to the WiFiManager
  wm.addParameter(&custom_static_ip);
  wm.addParameter(&custom_static_gw);
  wm.addParameter(&custom_static_sn);

  // Uncomment the next line to reset settings for testing
  //wm.resetSettings();

  // Automatically connect using saved credentials, or start the portal
  if (!wm.autoConnect("SS-90 CAM")) {
    Serial.println("Failed to connect");
    //ESP.restart();
  }

  // Retrieve the entered parameters from the portal
  strcpy(static_ip, custom_static_ip.getValue());
  strcpy(static_gw, custom_static_gw.getValue());
  strcpy(static_sn, custom_static_sn.getValue());

  // Check if a static IP was entered
  if (strcmp(static_ip, "") != 0) {
    // Convert strings to IPAddress objects for use in WiFi.config()
    IPAddress local_IP, gateway, subnet;
    local_IP.fromString(static_ip);
    gateway.fromString(static_gw);
    subnet.fromString(static_sn);

    // Configure the ESP32 with the provided static IP details
    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("Failed to configure Static IP. Reverting to DHCP.");
    }
  }

  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
