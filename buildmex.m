clear; close all; clc;

mex matrix_multiply_3x3.cpp attitude_representation.cpp
mex euler_to_dcm.cpp attitude_representation.cpp
mex dcm_to_quat.cpp attitude_representation.cpp