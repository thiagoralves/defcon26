#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_MCP23017.h>
         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000


#define PIN_D0    9
#define PIN_D1    10
#define PIN_D2    0
#define PIN_D3    1
#define PIN_D4    2
#define PIN_D5    3
#define PIN_D6    4
#define PIN_D7    5

#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_set(value, bit) ((value) |= (1UL << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit_write(value, bit, bitvalue) (bitvalue ? bit_set(value, bit) : bit_clear(value, bit))

Adafruit_MCP23017 mcp1;
Adafruit_ADS1015 ads1(72);     /* Use this for the 12-bit version, 73 if the jumper is shorted) */

void setup() 
{  
  mcp1.begin(1);      // use default address 0
  ads1.setGain(GAIN_ONE);
  ads1.begin();
  Serial.begin(115200);

  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  mcp1.pinMode(0, OUTPUT);
  mcp1.pinMode(1, OUTPUT);
  mcp1.pinMode(2, OUTPUT);
  mcp1.pinMode(3, OUTPUT);
  mcp1.pinMode(4, OUTPUT);
  mcp1.pinMode(5, OUTPUT);

}

unsigned char readTemperature()
{
  uint16_t measurement = ads1.readADC_SingleEnded(0);
  
  float voltage = (float)measurement * 0.001953125;
  float resistance = voltage/5.0;
  resistance = 1.0 / resistance;
  resistance -= 1.0;
  resistance = resistance * 10000;
  
  float steinhart;
  steinhart = resistance / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  
  return ((unsigned char)steinhart);
}

void loop() 
{
  unsigned char temperature = readTemperature();
  
  uint16_t mcp_regs = 0;
  mcp_regs = mcp1.readGPIOAB();
  digitalWrite(PIN_D0, bit_read(temperature, 0));
  digitalWrite(PIN_D1, bit_read(temperature, 1));
  bit_write(mcp_regs, 0, bit_read(temperature, 2));
  bit_write(mcp_regs, 1, bit_read(temperature, 3));
  bit_write(mcp_regs, 2, bit_read(temperature, 4));
  bit_write(mcp_regs, 3, bit_read(temperature, 5));
  bit_write(mcp_regs, 4, bit_read(temperature, 6));
  bit_write(mcp_regs, 5, bit_read(temperature, 7));
  mcp1.writeGPIOAB(mcp_regs);
}
