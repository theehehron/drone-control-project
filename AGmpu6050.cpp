#include "AGmpu6050.hpp"

std::vector<int16_t> gyro_calibration(const int &MPU_ADDR){
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    std::vector<int16_t> gyro_cals(3);
    for(int i = 1; i < 3000; i++){
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x43); // MPU-6050 Register Map and Descriptions
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_ADDR, 3*2, true);
        gyro_x = Wire.read()<<8 | Wire.read();
        gyro_y = Wire.read()<<8 | Wire.read();
        gyro_z = Wire.read()<<8 | Wire.read();
        gyro_cals[0] = (gyro_cals[0] + gyro_x) / i;
        gyro_cals[1] = (gyro_cals[1] + gyro_y) / i;
        gyro_cals[2] = (gyro_cals[2] + gyro_z) / i;
        delay(3);
    }
    Wire.endTransmission(true);
    return gyro_cals;
}