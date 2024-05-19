#ifndef ATTITUDE_REPRESENTATION_HPP
#define ATTITUDE_REPRESENTATION_HPP


#include "ArduinoSTL.h"
#include "vector"
#include "cmath"
#include "algorithm"


typedef std::vector<std::vector<float>> Matrix;

Matrix matrix_multiply_3_by_3(Matrix const &a, Matrix const &b);

Matrix matrix_scalar_multiply(float const &scalar, Matrix const &mat);

Matrix euler_to_dcm(std::vector<float> &euler_angles);

std::vector<float> dcm_to_euler(Matrix const &Cbv);

std::vector<float> dcm_to_quat(Matrix const &Cbv);

Matrix build_Omegab_bv(float p, float q, float r);

float trap_integration(float const &y0, float const &y1, unsigned long const &timestep);

void matrix_integral(Matrix &mat, Matrix const &mat_dot, Matrix const &mat_dot_prev, unsigned long const &timestep);


#endif
