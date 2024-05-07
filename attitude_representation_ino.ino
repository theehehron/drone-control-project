#include "attitude_representation.hpp"
#include "Wire.h"
#include "AGmpu6050.hpp"

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
float p, q, r;
std::vector<int16_t> gyrocals(3);
float PI_ag = 3.141593;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(B00000000);
//  Wire.write(0x1B);
//  Wire.write(B00000000);
  Wire.endTransmission(true);

  gyrocals = gyro_calibration(MPU_ADDR);
  Serial.print(gyrocals[0]);
  Serial.print(" ");
  Serial.print(gyrocals[1]);
  Serial.print(" ");
  Serial.print(gyrocals[2]);
}



void loop() {
  std::vector<float> pqr = gyrorates_rad_per_sec(MPU_ADDR, gyrocals);

  Serial.print(pqr[0], 3);
  Serial.print(", ");
  Serial.print(pqr[1], 3);
  Serial.print(", ");
  Serial.print(pqr[2], 3);
  Serial.println();
  delay(3);
}
