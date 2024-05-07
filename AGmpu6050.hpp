#ifndef AGMPU6050_HPP
#define AGMPU6050_HPP

#include "ArduinoSTL.h"
#include "vector"
#include "cmath"
#include "Arduino.h"
#include "Wire.h"
#include "attitude_representation.hpp"

std::vector<int16_t> gyro_calibration(const int &MPU_ADDR);

//Matrix Omegab_bv(int_16t &p, int_16t &q, int_16t &r);


std::vector<float> gyrorates_rad_per_sec(const int &MPU_ADDR, std::vector<int16_t> &gyrocals);

#endif
