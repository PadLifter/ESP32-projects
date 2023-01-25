#include "BluetoothSerial.h"

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;


const int fanPin = 13;  // GPIO 21, fan control
const int potPin = 26;  // A0, potentiometer
int potValue = 0;

// PWM properties
const int freq = 5000;
const int fanChannel = 0;
const int resolution = 8;

// Handle received and sent messages
String message = "";
char incomingChar;
int speed = 0;

// Timer: auxiliar variables
unsigned long previousMillis = 0;  // Stores last time pot value was published
const long interval = 10000;       // interval at which to publish pot value

void setup() {
  ledcAttachPin(fanPin, fanChannel);
  ledcSetup(fanChannel, freq, resolution);
  Serial.begin(115200);
  SerialBT.begin("sinihammas");  // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  unsigned long currentMillis = millis();

  // Send pot value once per 10 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    potValue = analogRead(potPin);
    //SerialBT.println(potValue);
  }

  //Read received BT messages
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      message += String(incomingChar);
    } else {
      message = "";
    }
    Serial.write(incomingChar);
  }

  // Check received message and control led and fan accordingly
  if (message == "led_on") {
    digitalWrite(ledPin, HIGH);
  } else if (message == "led_off") {
    digitalWrite(ledPin, LOW);
  } else if (isDigit(message[0])) {
    speed = message.toInt();
    if (speed >= 0 && speed <= 255) {
      ledcWrite(fanChannel, speed);
    }
  }

  /*
  } else if (message == "fan_on") {
    digitalWrite(fanPin, HIGH);
  } else if (message == "fan_off") {
    digitalWrite(fanPin, LOW);
  }
  */

  delay(20);
}