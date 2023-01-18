#define ADC_POT A0

void setup() {
Serial.begin(115200);

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

void loop() {
  uint adc = analogRead(ADC_POT);
 Serial.println (adc);
 delay(500);

}

void displayTask(void*parameter){
  for(;;){
    Serial.println ("Display");
    delay (1000);

  }
  vTaskDelete(NULL);
}
