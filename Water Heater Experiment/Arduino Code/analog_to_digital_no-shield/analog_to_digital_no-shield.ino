// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000


#define PIN_D0    2
#define PIN_D1    3
#define PIN_D2    4
#define PIN_D3    5
#define PIN_D4    6
#define PIN_D5    7
#define PIN_D6    8
#define PIN_D7    9

#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_set(value, bit) ((value) |= (1UL << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit_write(value, bit, bitvalue) (bitvalue ? bit_set(value, bit) : bit_clear(value, bit))

void setup() 
{  
  Serial.begin(115200);

  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  pinMode(PIN_D2, OUTPUT);
  pinMode(PIN_D3, OUTPUT);
  pinMode(PIN_D4, OUTPUT);
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D6, OUTPUT);
  pinMode(PIN_D7, OUTPUT);
}

unsigned char readTemperature()
{
  uint16_t measurement = analogRead(A0);
  
  float voltage = (float)measurement * 0.0048828125;
  //Serial.print("Voltage: ");
  //Serial.println(voltage);
  float resistance = voltage/5.0;
  resistance = 1.0 / resistance;
  resistance -= 1.0;
  resistance = resistance * 10000;
  //Serial.print("Resistance: ");
  //Serial.println(resistance);
  
  float steinhart;
  steinhart = resistance / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  //Serial.print("Converted temp: ");
  //Serial.println(steinhart);
  
  return ((unsigned char)steinhart);
}

void loop() 
{
  unsigned char temperature = readTemperature();
  
  digitalWrite(PIN_D0, bit_read(temperature, 0));
  digitalWrite(PIN_D1, bit_read(temperature, 1));
  digitalWrite(PIN_D2, bit_read(temperature, 2));
  digitalWrite(PIN_D3, bit_read(temperature, 3));
  digitalWrite(PIN_D4, bit_read(temperature, 4));
  digitalWrite(PIN_D5, bit_read(temperature, 5));
  digitalWrite(PIN_D6, bit_read(temperature, 6));
  digitalWrite(PIN_D7, bit_read(temperature, 7));

  //delay(1000);
}
