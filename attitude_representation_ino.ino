#include "attitude_representation.hpp"
#include "Wire.h"

void setup() {
  Serial.begin(9600);

  std::vector<float> eulers = {-3.0854, -0.9146, 1.7651};
  Matrix Cbv = euler_to_dcm(eulers);

  Serial.print(Cbv[0][0], 4);
  Serial.print(" ");
  Serial.print(Cbv[0][1], 4);
  Serial.print(" ");
  Serial.println(Cbv[0][2], 4);  
  Serial.print(Cbv[1][0], 4);
  Serial.print(" ");
  Serial.print(Cbv[1][1], 4);
  Serial.print(" ");
  Serial.println(Cbv[1][2], 4); 
  Serial.print(Cbv[2][0], 4);
  Serial.print(" ");
  Serial.print(Cbv[2][1], 4);
  Serial.print(" ");
  Serial.println(Cbv[2][2], 4); 

  unsigned long a = millis();
  delay(5000);
  unsigned long b = millis();
  unsigned long delta_t = b - a;

  float y0 = 5.56;
  float y1 = 13.78;

  float out = trap_integration(y0, y1, delta_t);
  Serial.println(out, 5);
  
}



void loop() {

}
