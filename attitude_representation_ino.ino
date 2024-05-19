#include "attitude_representation.hpp"
#include "Wire.h"
#include "AGmpu6050.hpp"

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050

std::vector<int16_t> gyrocals(3);
std::vector<float> eulers(3);
float PI_ag = 3.141593;
int16_t counter = 0;

// Set Initial Conditions
Matrix Cbv = {{1, 0, 0}, \
              {0, 1, 0}, \
              {0, 0, 1}};

Matrix Cbv_dot_prev = {{0, 0, 0}, \
                       {0, 0, 0}, \
                       {0, 0, 0}};

unsigned long previous_time = 0;

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
  previous_time = micros();
}


 
void loop() {
  counter = counter + 1;
  
  std::vector<float> pqr = gyrorates_rad_per_sec(MPU_ADDR, gyrocals); 
  Matrix Omega_bv = build_Omegab_bv(pqr[0], pqr[1], pqr[2]);
  Matrix Cbv_dot = matrix_multiply_3_by_3(Omega_bv, matrix_scalar_multiply(-1, Cbv));  // Cbv_dot = -Omega_bv*Cbv

  unsigned long current_time = micros();
  unsigned long timestep = current_time-previous_time;
  
  // perform elementwise integration
  matrix_integral(Cbv, Cbv_dot, Cbv_dot_prev, timestep);

  previous_time = current_time;
  Cbv_dot_prev = Cbv_dot;
  

  if (counter % 20 == 0){
    eulers = dcm_to_euler(Cbv);
    
//    Serial.print(timestep); // used for debugging and for optimization measurements.
//    Serial.print(", ");
    serialprint_eulers(eulers);
  }
  if (counter == 100){
    counter = 0;
  }
}
