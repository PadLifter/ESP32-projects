const int measurePin = 4; // GPIO 04 (Analog ADC2_CH0) 
const int potPin = 36;    // GPIO 36 (Analog ADC1_CH0)

int adcValue = 0;   // Storing adcValue
int potValue = 0;   // Storing potValue

void setup() {
  Serial.begin(115200);
  delay(1000);
  analogReadResolution(8);
  analogSetAttenuation(ADC_11db); 
}

void loop() {
  potValue = analogRead(potPin); // Reading ADC potValue
  adcValue = analogRead(measurePin); // Reading ADC voltage value
  Serial.println("Voltage: ");
  Serial.println(adcValue);
  Serial.print('\n');
  Serial.println("potValue: ");
  Serial.println(potValue);
  delay(1000);
}