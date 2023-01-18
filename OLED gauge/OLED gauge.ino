#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define ADC_POT A0
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define a struct
struct pinRead {
  uint pin;    // analog channel indentifier
  uint value;  // sensor output value
};

QueueHandle_t structQueue;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  // Clear the buffer.
  display.clearDisplay();


  // create Structure Queue
  structQueue = xQueueCreate(10, sizeof(struct pinRead));

  if (structQueue != NULL) {

    xTaskCreatePinnedToCore(
      displayTask,    // Function that should be called
      "displayTask",  // Name of the task (for debugging)
      2048,           // Stack size (bytes)
      NULL,           // Parameter to pass
      1,              // Task priority
      NULL,           // Task handle
      1               // Core you want to run the task on (0 or 1)
    );
  }
}

void loop() {
  uint adc = analogRead(ADC_POT);
  Serial.println(adc);
  delay(500);

  // push pot value to queue
  struct pinRead currentPinRead;                                  // define a structure of type pinRead
  currentPinRead.pin = 0;                                         // assign value '0' to pin element of struct
  currentPinRead.value = adc;                                     // Read adc value
  xQueueSendToBack(structQueue, &currentPinRead, portMAX_DELAY);  //write struct message to queue
}

void displayTask(void* parameter) {
  for (;;) {

    uint POT = 0;                    // temporary variable to hold pot value
    uint TERM = 0;                   // temporary variable to hold terminal input value
    struct pinRead currentPinRead;  // structure to hold receiv data
    
    // Read structure elements from queue and check if data received successfully
    if (xQueueReceive(structQueue, &currentPinRead, portMAX_DELAY) == pdPASS) {
      // print received data elements on serial montor
      Serial.print("Pin: ");
      Serial.print(currentPinRead.pin);
      Serial.print(" Value: ");
      Serial.println(currentPinRead.value);

      // If condition checks,  if data receive from channel zero
      // If yes, store sensor value member of structure in temporary pot variable
      // Print pot value to oled
      if (currentPinRead.pin == 0) {
        POT = currentPinRead.value;

        // Clear row
        display.fillRect(0, 0, 128, 14, BLACK);

        // Print pot value
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println(POT);
        BarGraph(POT, currentPinRead.pin);
        display.display();
      }

      // If condition checks,  if data receive from channel one (A1)
      // If yes, store sensor value member of structure in temporary LDR variable
      if (currentPinRead.pin == 1) {

        TERM = currentPinRead.value; 
      }
     
    }
  }
   vTaskDelete(NULL);
}

void BarGraph (uint val, int pos){
  uint w = 127 * val/4095;
 if (pos == 0){
  display.fillRect(0, 17, 127, 14, BLACK);
  display.drawRect(0, 17, 127, 14, WHITE);
  display.fillRect(0, 17, w, 14, WHITE);
  display.display();
 }

Serial.println (w);

}
