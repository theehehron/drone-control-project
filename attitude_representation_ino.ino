#include "attitude_representation.hpp"
#include "Wire.h"
#include "AGmpu6050.hpp"

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t gyro_x, gyro_y, gyro_z;
std::vector<int16_t> gyrocals(3);


void setup() {
  Serial.begin(115200);

  std::vector<float> eulers = {-3.0854, -0.9146, 1.7651};
  Matrix Cbv = euler_to_dcm(eulers);

  unsigned long a = millis();
  delay(5000);
  unsigned long b = millis();
  unsigned long delta_t = b - a;

  float y0 = 5.56;
  float y1 = 13.78;

  float out = trap_integration(y0, y1, delta_t);
//  Serial.println(out, 5);

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
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); // MPU-6050 Register Map and Descriptions
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 3*2, true);
  gyro_x = ((Wire.read()<<8 | Wire.read()) - gyrocals[0])/131;
  gyro_y = ((Wire.read()<<8 | Wire.read()) - gyrocals[1])/131;
  gyro_z = ((Wire.read()<<8 | Wire.read()) - gyrocals[2])/131;

  Serial.print(gyro_x);
  Serial.print(", ");
  Serial.print(gyro_y);
  Serial.print(", ");
  Serial.print(gyro_z);
  Serial.println();
  delay(3);
//  Serial.print(gyrocals[0]);
//  Serial.print(" ");
//  Serial.print(gyrocals[1]);
//  Serial.print(" ");
//  Serial.println(gyrocals[2]);
}
