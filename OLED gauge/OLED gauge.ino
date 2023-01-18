#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define ADC_POT A0
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define a struct
struct pinRead 
{
  int pin;  // analog channel indentifier
  int value; // sensor output value
};

QueueHandle_t structQueue;

void setup() {
Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  // Clear the buffer.
   display.clearDisplay();


// create Structure Queue
  structQueue = xQueueCreate(10, sizeof(struct pinRead));

  if (structQueue != NULL) {

  xTaskCreatePinnedToCore(
    displayTask,      // Function that should be called
    "displayTask",    // Name of the task (for debugging)
    2048,               // Stack size (bytes)
    NULL,               // Parameter to pass
    1,                  // Task priority
    NULL,               // Task handle
    1          // Core you want to run the task on (0 or 1)
);
  }
}

void loop() {
  uint adc = analogRead(ADC_POT);
 Serial.println (adc);
 delay(500);

 // push pot value to queue
 struct pinRead currentPinRead;  // define a structure of type pinRead 
    currentPinRead.pin = 0; // assign value '0' to pin element of struct
    currentPinRead.value = adc; // Read adc value 
    xQueueSend(structQueue, &currentPinRead, portMAX_DELAY); //write struct message to queue

}

void displayTask(void*parameter){
  for(;;){
    Serial.println ("Display");
    delay (1000);
    // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 28);
  display.println("Hello!!!!!!");
  display.display();

  }
  vTaskDelete(NULL);
}
