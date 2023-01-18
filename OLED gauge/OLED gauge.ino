#include <Arduino_FreeRTOS.h>
#include <queue.h>

void setup() {
Serial.begin(112500)

  xTaskCreatePinnedToCore(
    displayTask,      // Function that should be called
    "displayTask",    // Name of the task (for debugging)
    2048,               // Stack size (bytes)
    NULL,               // Parameter to pass
    1,                  // Task priority
    NULL,               // Task handle
    1,          // Core you want to run the task on (0 or 1)
);

}

void loop() {
  

}
