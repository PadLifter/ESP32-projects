const int measurePin = 4; // GPIO 04 (Analog ADC2_CH0) 
const int potPin = 36;    // GPIO 36 (Analog ADC1_CH0)

uint8_t adcValue = 0;       // Storing adcValue
uint8_t potValueTrig = 0;       // Storing potValue
int sampleCount = 0;        // Storing samples

uint8_t sine[100];

void setup() {
  Serial.begin(115200);
  delay(1000);
  analogReadResolution(8);        // Sets the sample bits and read resolution
  analogSetAttenuation(ADC_11db); // Sets the input attenuation
}

void tableWrite(uint8_t adcValue, uint8_t potValueTrig){
  sine[sampleCount] = adcValue;
  if(sine[0] - potValueTrig > -4 && sine[0] - potValueTrig < 4){
    if(sampleCount == 1 && sine[1] <= sine[0]){
    sampleCount = 0;
    Serial.print("Voltage decreasing");
    }
    else{
      if(sampleCount == 99){
      sampleCount = 0;
      Serial.print("Array completed");
      }
      else{sampleCount++;}
      Serial.print("Samplecount: ");
      Serial.println(sampleCount);
    }
  }
}

void loop() {

  potValueTrig = analogRead(potPin); // Reading ADC potValue
  adcValue = analogRead(measurePin); // Reading ADC voltage value
  tableWrite(adcValue, potValueTrig);
  Serial.println("Voltage: ");
  Serial.println(adcValue);
  Serial.println("potValue: ");
  Serial.println(potValueTrig);
  
}

/*

  Serial.println("Voltage: ");
  Serial.println(adcValue);
  Serial.print('\n');
  Serial.println("potValue: ");
  Serial.println(potValue);
  delay(1000);
  */
