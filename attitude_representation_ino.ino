#include "attitude_representation.hpp"

void setup() {
  Serial.begin(9600);

  std::vector<float> eulers = {-3.0854, -0.9146, 1.7651};
  Matrix Cbv = euler_to_dcm(eulers);

  Serial.print(Cbv[0][0]);
  Serial.print(" ");
  Serial.print(Cbv[0][1]);
  Serial.print(" ");
  Serial.println(Cbv[0][2]);  
  Serial.print(Cbv[1][0]);
  Serial.print(" ");
  Serial.print(Cbv[1][1]);
  Serial.print(" ");
  Serial.println(Cbv[1][2]); 
  Serial.print(Cbv[2][0]);
  Serial.print(" ");
  Serial.print(Cbv[2][1]);
  Serial.print(" ");
  Serial.println(Cbv[2][2]); 
}



void loop() {

}
