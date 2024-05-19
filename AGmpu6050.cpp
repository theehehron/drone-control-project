#include "AGmpu6050.hpp"


std::vector<int16_t> gyro_calibration(const int &MPU_ADDR){
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    std::vector<int16_t> gyro_cals(3);
    std::vector<int16_t> sum(3);
    int count = 100;
    for(int i = 1; i < count; i++){
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x43); // MPU-6050 Register Map and Descriptions
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_ADDR, 3*2, true);
        gyro_x = Wire.read()<<8 | Wire.read();
        gyro_y = Wire.read()<<8 | Wire.read();
        gyro_z = Wire.read()<<8 | Wire.read();
        sum[0] = (sum[0] + gyro_x);
        sum[1] = (sum[1] + gyro_y);
        sum[2] = (sum[2] + gyro_z);
        delay(3);
    }

    gyro_cals = {sum[0]/count, sum[1]/count, sum[2]/count};
    Wire.endTransmission(true);
    return gyro_cals;
}



std::vector<float> gyrorates_rad_per_sec(const int &MPU_ADDR, std::vector<int16_t> &gyrocals){
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x43); // MPU-6050 Register Map and Descriptions
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 3*2, true);
    std::vector<float> gyro_rates = {static_cast<float>(((Wire.read()<<8 | Wire.read()) - gyrocals[0])/131)*3.1415927/180, \
                                     static_cast<float>(((Wire.read()<<8 | Wire.read()) - gyrocals[1])/131)*3.1415927/180, \
                                     static_cast<float>(((Wire.read()<<8 | Wire.read()) - gyrocals[2])/131)*3.1415927/180};
    return gyro_rates;
}

void serialprint_matrix(Matrix &Cbv){
  Serial.print(Cbv[0][0], 3);
  Serial.print(", ");
  Serial.print(Cbv[0][1], 3);
  Serial.print(", ");
  Serial.println(Cbv[0][2], 3);
  Serial.print(Cbv[1][0], 3);
  Serial.print(", ");
  Serial.print(Cbv[1][1], 3);
  Serial.print(", ");
  Serial.println(Cbv[1][2], 3);
  Serial.print(Cbv[2][0], 3);
  Serial.print(", ");
  Serial.print(Cbv[2][1], 3);
  Serial.print(", ");
  Serial.println(Cbv[2][2], 3);
  Serial.println("............");
}



void serialprint_eulers(std::vector<float> eulers){
  Serial.print(eulers[0], 3);
  Serial.print(", ");
  Serial.print(eulers[1], 3);
  Serial.print(", ");
  Serial.println(eulers[2], 3);
}



void serialprint_eulers_deg(std::vector<float> eulers){
  Serial.print(eulers[0]*180/PI, 3);
  Serial.print(", ");
  Serial.print(eulers[1]*180/PI, 3);
  Serial.print(", ");
  Serial.println(eulers[2]*180/PI, 3);
}
