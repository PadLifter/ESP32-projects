// Työ 1, tehtävä 3


#include "BluetoothSerial.h"

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// GPIO where LED is connected to
const int ledPin = 25;  //LED output
const int fanPin = 21;  //Fan control
const int potPin = 26;  //A0, pot
const int freq = 5000;
int potValue = 0;

// setting PWM properties
const int fanChannel = 0;
const int resolution = 8;

// Handle received and sent messages
String message = "";
char incomingChar;
int speed = 0;

// Timer: auxiliar variables
unsigned long previousMillis = 0;  // Stores last time temperature was published
const long interval = 3000;        // interval at which to publish sensor readings

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  Serial.begin(115200);

  // configure fan PWM functionalitites
  ledcSetup(fanChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(fanPin, fanChannel);

  // Bluetooth device name
  SerialBT.begin("sinihammas");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  unsigned long currentMillis = millis();

  // Send pot readings
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    SerialBT.println(potValue);
    potValue = analogRead(potPin);
  }

  // Read received messages (LED control command)
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      message += String(incomingChar);
    } else {
      message = "";
    }
    Serial.write(incomingChar);
  }
  // Check received message and control output accordingly
  if (message == "led_on") {
    digitalWrite(ledPin, HIGH);
  } else if (message == "led_off") {
    digitalWrite(ledPin, LOW);
  }
  /*
  } else if (message == "fan_on") {
    digitalWrite(fanPin, HIGH);
  } else if (message == "fan_off") {
    digitalWrite(fanPin, LOW);
  }
  */


//Setting fan speed between 0-255
  else if (isDigit(message[0])) {
    speed = message.toInt();
    if (speed >= 1 && speed <= 255)
      ;
    ledcWrite(fanChannel, speed);
  }


  delay(20);
}