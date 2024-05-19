#include "attitude_representation.hpp"
#include "Wire.h"
#include "AGmpu6050.hpp"

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
std::vector<float> pqr(3);
std::vector<int16_t> gyrocals(3);
float PI_ag = 3.141593;
int16_t counter = 0;

// Set Initial Conditions
Matrix Cbv = {{1, 0, 0}, \
              {0, 1, 0}, \
              {0, 0, 1}};

Matrix Cbv_neg = matrix_scalar_multiply(-1, Cbv);

Matrix Cbv_dot_prev = {{0, 0, 0}, \
                       {0, 0, 0}, \
                       {0, 0, 0}};

Matrix Omega_bv = {{0, 0, 0}, \
                   {0, 0, 0}, \
                   {0, 0, 0}};;
Matrix Cbv_dot = {{0, 0, 0}, \
                  {0, 0, 0}, \
                  {0, 0, 0}};;;
unsigned long initial_time = 0;
unsigned long previous_time = initial_time;

// Variables that get used later:
float Cbv_1_1 = 1;
float Cbv_1_2 = 0;
float Cbv_1_3 = 0;
float Cbv_2_1 = 0;
float Cbv_2_2 = 1;
float Cbv_2_3 = 0;
float Cbv_3_1 = 0;
float Cbv_3_2 = 0;
float Cbv_3_3 = 1;
unsigned long timestep = 0;
std::vector<float> eulers(3);

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
//  Serial.print(gyrocals[0]);
//  Serial.print(" ");
//  Serial.print(gyrocals[1]);
//  Serial.print(" ");
//  Serial.print(gyrocals[2]);
//  Serial.println();
//  Serial.print("gyro cals complete");
//  Serial.println();
  previous_time =  micros();
}



void loop() {
  counter = counter + 1;
  
  pqr = gyrorates_rad_per_sec(MPU_ADDR, gyrocals);
  Omega_bv = build_Omegab_bv(pqr[0], pqr[1], pqr[2]);
  Cbv_dot = matrix_multiply_3_by_3(Omega_bv, matrix_scalar_multiply(-1, Cbv));  // Cbv_dot = -Omega_bv*Cbv

  unsigned long current_time =  micros();
  timestep = current_time-previous_time;

  Cbv_1_1 = Cbv_1_1 + trap_integration(Cbv_dot_prev[0][0], Cbv_dot[0][0], timestep);
  Cbv_1_2 = Cbv_1_2 + trap_integration(Cbv_dot_prev[0][1], Cbv_dot[0][1], timestep);
  Cbv_1_3 = Cbv_1_3 + trap_integration(Cbv_dot_prev[0][2], Cbv_dot[0][2], timestep);
  Cbv_2_1 = Cbv_2_1 + trap_integration(Cbv_dot_prev[1][0], Cbv_dot[1][0], timestep);
  Cbv_2_2 = Cbv_2_2 + trap_integration(Cbv_dot_prev[1][1], Cbv_dot[1][1], timestep);
  Cbv_2_3 = Cbv_2_3 + trap_integration(Cbv_dot_prev[1][2], Cbv_dot[1][2], timestep);
  Cbv_3_1 = Cbv_3_1 + trap_integration(Cbv_dot_prev[2][0], Cbv_dot[2][0], timestep);
  Cbv_3_2 = Cbv_3_2 + trap_integration(Cbv_dot_prev[2][1], Cbv_dot[2][1], timestep);
  Cbv_3_3 = Cbv_3_3 + trap_integration(Cbv_dot_prev[2][2], Cbv_dot[2][2], timestep);
  Cbv = {{Cbv_1_1, Cbv_1_2, Cbv_1_3}, \
         {Cbv_2_1, Cbv_2_2, Cbv_2_3}, \
         {Cbv_3_1, Cbv_3_2, Cbv_3_3}};
  
  previous_time = current_time;
  Cbv_dot_prev = Cbv_dot;
  
  

  if (counter % 10 == 0){
    eulers = dcm_to_euler(Cbv);

    Serial.print(timestep);
    Serial.print(", ");
    serialprint_eulers(eulers);
  }
  if (counter == 100){
    counter = 0;
  }
}
