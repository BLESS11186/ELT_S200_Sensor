#include <ELT_S200.h>

S200 co2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  co2.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t c = co2.get_co2();
  Serial.println(c);
  delay(2000);
}
