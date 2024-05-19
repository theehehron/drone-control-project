#ifndef AGMPU6050_HPP
#define AGMPU6050_HPP

#include "ArduinoSTL.h"
#include "vector"
#include "cmath"
#include "Arduino.h"
#include "Wire.h"
#include "attitude_representation.hpp"



std::vector<int16_t> gyro_calibration(const int &MPU_ADDR);

std::vector<float> gyrorates_rad_per_sec(const int &MPU_ADDR, std::vector<int16_t> &gyrocals);

void serialprint_matrix(Matrix &Cbv);

void serialprint_eulers(std::vector<float> eulers);

void serialprint_eulers_deg(std::vector<float> eulers);



#endif
