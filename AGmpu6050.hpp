#ifndef AGMPU6050_HPP
#define AGMPU6050_HPP

#include "vector"
#include "cmath"
#include "Arduino.h"
#include "Wire.h"

std::vector<int16_t> gyro_calibration(const int &MPU_ADDR);

#endif