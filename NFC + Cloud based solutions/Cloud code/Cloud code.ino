// TAMK SULAB1 exercise template for sending telemetry data into ThingsBoard
// cloud using MQTT using Arduino IDE and  ESP32/ESP8266 HW.
// Template is built on example sketch "0003-esp8266_send_data.ino" (TS 25.9.21)
// *****************************************************************************

// Installation Guide:
// 0. Save this sketch with a new name (eg. including your ESP board name)
// 1. Install ESP32 or ESP8266 according to your device in hand. 
//    - First install packages (File->Preferences), either 
//      https://dl.espressif.com/dl/package_esp32_index.json for ESP32 boards, or
//      http://arduino.esp8266.com/stable/package_esp8266com_index.json for ESP8266 boards
//    - Then, add your board (Tools -> Boards Manager) to sketch
// 2. Set compilation flag below accordingly
#define ESP_USED        32 // or: 8266 (not tested, good luck!)

// 3a. If using ESP32 dev board without integrated OLED display: goto 4.
// 3b. If using I2C-based OLED-display like Wemos ESP32 OLED:
//      - install libaries Adafruit GFX and Adafruit SSD1306 via Library Manager
//      - Set compilation flag below to 1:
#define I2C_OLED_USED   0  // Set to 1 if Wemos ESP32 OLED or similar device is used

// 4. Install libraries "ThingsBoard" and "ArduinoJson" via Library Manager
// 5. Modify required parts of code below to match with your networking setup (WLAN settings, ThingsBoard device token, etc.)
// 6. Test that your sketch compiles and programs to your ESP as expected - fix when necessary
// 7. Modify your own sketch as wanted, debug, program, test it and enjoy the results :]

#if (ESP_USED != 32) && (I2C_OLED_USED != 0)
#error "Sounds like an invalid setup, please check!"
#endif

#include "ThingsBoard.h"

#if ESP_USED == 8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#if I2C_OLED_USED == 1
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#endif

#define WIFI_ACCESS_POINT   "Samin phone"
#define WIFI_PASSWORD       "S4mpp4123"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "sJvg6BeRPgtjDi5SIPHi"
#define THINGSBOARD_SERVER  "tb.tamk.cloud"

#define MEAS_CH1_NAME       "temperature"
#define MEAS_CH2_NAME       "weight"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Define analog input pins
// #define AN_IN_PIN1          26
// #define AN_IN_PIN2          25 // tst dbg        

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance

ThingsBoard tb(espClient);

// the Wifi radio's status
int status = WL_IDLE_STATUS;

// Raw ADC reading result
int sensorValue = 0;

// Measurement values calculated from ADC readings
int iMeasVal1 = 0;
float fMeasVal2 = 0.0;

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);

#if I2C_OLED_USED == 1
  // Init I2C communication, e.g. SDA = 5 and SCL = 4
  Wire.begin(5, 4);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
#endif // I2C_OLED_USED == 1

  WiFi.begin(WIFI_ACCESS_POINT, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending data...");

#if I2C_OLED_USED == 1
  displayValues();
#endif

  /* tst dbg: commented out
  sensorValue = analogRead(AN_IN_PIN1);
  Serial.print("Value read from analog input 1: ");
  Serial.println(sensorValue);
  iMeasVal1 = sensorValue;
*/
  iMeasVal1 = 20 + random(10); // tst dbg

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  tb.sendTelemetryInt(MEAS_CH1_NAME, iMeasVal1);

  /* tst dbg: commented out
  sensorValue = analogRead(26);
  Serial.print("Value read from analog input 2: ");
  Serial.println(sensorValue);
  fMeasVal2 = sensorValue;
  */
  
  fMeasVal2 = 49.9 + random(20); // tst dbg

  tb.sendTelemetryFloat(MEAS_CH2_NAME, fMeasVal2);

  tb.loop();

  delay(2000);
}

void InitWiFi()
{
  Serial.print("Connecting to AP: ");  Serial.println(WIFI_ACCESS_POINT);
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_ACCESS_POINT, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_ACCESS_POINT, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}

#if I2C_OLED_USED == 1
void displayValues(void) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);// Draw white text
  display.setTextSize(2);             // pixel scale
  display.setCursor(0, 0);            // Start at top-left corner
  display.print(AN_IN_PIN1); display.print(": "); display.println(iMeasVal1); 
  display.print(AN_IN_PIN2); display.print(": "); display.println(fMeasVal2); 
  display.setTextSize(1);             // pixel scale
  display.print("IP: "); display.println(WiFi.localIP());
  display.print("RSSI (dBm): "); display.print(WiFi.RSSI());
  display.display();
}
#endif // I2C_OLED_USED == 1
