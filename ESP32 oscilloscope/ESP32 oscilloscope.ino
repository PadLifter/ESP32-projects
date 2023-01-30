const int measurePin = 34; // GPIO 34 (Analog ADC1_CH6) 

int adcValue = 0; // storing adc value

void setup() {
  Serial.begin(115200);
  delay(1000);
  analogReadResolution(8);
  analogSetAttenuation(ADC_11db); 
}

void loop() {

  adcValue = analogRead(measurePin); // Reading ADC value  
  Serial.println(adcValue);
  delay(500);
}