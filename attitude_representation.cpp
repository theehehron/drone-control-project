#include "attitude_representation.hpp"


Matrix matrix_multiply_3_by_3(Matrix const &a, Matrix const &b){
    // takes 3x3 a and 3x3 b and computes a*b
    Matrix result(3, std::vector<float>(3, 0));

    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            for (int k = 0; k<3; k++){
                result[i][j] += a[i][k] * b [k][j];
            }
        }
    }
    return result;
}



Matrix matrix_scalar_multiply(float const &scalar, Matrix const &mat){
    Matrix result(3, std::vector<float>(3, 0));
    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            result[i][j] = mat[i][j] * scalar;
        }
    }
    return result;
}



Matrix euler_to_dcm(std::vector<float> &euler_angles){
    // takes vector [phi, theta, psi] and computes DCM

    Matrix C1v = {{cos(euler_angles[2]),   sin(euler_angles[2]),   0}, \
                  {-sin(euler_angles[2]),  cos(euler_angles[2]),   0}, \
                  {                    0,                     0,   1}};
    
    Matrix C21 = {{cos(euler_angles[1]),  0,  -sin(euler_angles[1])}, \
                  {                   0,  1,                      0}, \
                  {sin(euler_angles[1]),  0,   cos(euler_angles[1])}};
    
    Matrix Cb2 = {{1,                     0,                    0}, \
                  {0,  cos(euler_angles[0]), sin(euler_angles[0])}, \
                  {0, -sin(euler_angles[0]), cos(euler_angles[0])}};
    
    Matrix C_x = matrix_multiply_3_by_3(C21, C1v);

    Matrix Cbv = matrix_multiply_3_by_3(Cb2, C_x);

    return Cbv;
}



std::vector<float> dcm_to_euler(Matrix const &Cbv){
    // Returns the euler angles associated with DCM Cbv
    std::vector<float> euler_angles(3, 0);
    euler_angles[0] = atan2(Cbv[1][2], Cbv[2][2]);
    euler_angles[1] = -asin(Cbv[0][2]);
    euler_angles[2] = atan2(Cbv[0][1], Cbv[0][0]);

    return euler_angles;
}



std::vector<float> dcm_to_quat(Matrix const &Cbv){
    // returns quaternion representation corresponding to the DCM Cbv
    float q_tilde1 = static_cast<float>(pow((.25*(1+Cbv[0][0]+Cbv[1][1]+Cbv[2][2])), .5));
    float q_tilde2 = static_cast<float>(pow((.25*(1+Cbv[0][0]-Cbv[1][1]-Cbv[2][2])), .5));
    float q_tilde3 = static_cast<float>(pow((.25*(1-Cbv[0][0]+Cbv[1][1]-Cbv[2][2])), .5));
    float q_tilde4 = static_cast<float>(pow((.25*(1-Cbv[0][0]-Cbv[1][1]+Cbv[2][2])), .5));

    float q_tilde[4] = {abs(q_tilde1), abs(q_tilde2), abs(q_tilde3), abs(q_tilde4)};

    int n = sizeof(q_tilde) / sizeof(q_tilde[0]);
    auto max_element = std::max_element(q_tilde, q_tilde + n);
    int max_index = std::distance(q_tilde, max_element);
    
    std::vector<float> q(4, 0);
    switch (max_index)
    {
    case 1:
        q[0] = (Cbv[1][2]-Cbv[2][1])/(4*q_tilde[1]);
        q[1] = q_tilde2;
        q[2] = (Cbv[0][1]+Cbv[1][0])/(4*q_tilde[1]);
        q[3] = (Cbv[2][0]+Cbv[0][2])/(4*q_tilde[1]);
        break;
    case 2:
        q[0] = (Cbv[2][0]-Cbv[0][2])/(4*q_tilde[2]);
        q[1] = (Cbv[0][1]+Cbv[1][0])/(4*q_tilde[2]);
        q[2] = q_tilde3;
        q[3] = (Cbv[1][2]+Cbv[2][1])/(4*q_tilde[2]);
        break;
    case 3:
        q[0] = (Cbv[1][0]-Cbv[0][1])/(4*q_tilde[3]);
        q[1] = (Cbv[0][2]+Cbv[2][0])/(4*q_tilde[3]);
        q[2] = (Cbv[1][2]+Cbv[2][1])/(4*q_tilde[3]);
        q[3] = q_tilde4;
        break;
    default:
        q[0] = q_tilde1;
        q[1] = (Cbv[1][2]-Cbv[2][1])/(4*q_tilde[0]);
        q[2] = (Cbv[2][0]-Cbv[0][2])/(4*q_tilde[0]);
        q[3] = (Cbv[0][1]-Cbv[1][0])/(4*q_tilde[0]);
        break;
    }
    return q;
}



Matrix build_Omegab_bv(float p, float q, float r){
    Matrix Omegab_bv = {{0, -r,  q}, \
                        {r,  0, -p}, \
                        {-q,  p,  0}};

    return Omegab_bv;
}



float trap_integration(float const &y0, float const &y1, unsigned long const &timestep){
    // takes y1, y2, and timestep in microseconds and integrates w.r.t. time
    float timestep_sec = (static_cast<float>(timestep))/1000/1000;
    float delta_area = (y0+y1)*timestep_sec/2;
    return delta_area;
}



void matrix_integral(Matrix &mat, Matrix const &mat_dot, Matrix const &mat_dot_prev, unsigned long const &timestep){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
      mat[i][j] = mat[i][j] + trap_integration(mat_dot_prev[i][j], mat_dot[i][j], timestep);
    }
  }
}



std::vector<float> accel_angle(const std::vector<float> &accels){
  // takes accelerometer reading in gs and outputs pitch angle and roll angle
  std::vector<float> angle(2);
  angle[0] = -atan2(accels[0], accels[2]);
  angle[1] = atan2(accels[1], accels[2]);
  return angle;
}



void dcm_integration(Matrix &Cbv, const std::vector<float> &pqr, Matrix &Cbv_dot, const unsigned long &timestep){
  Matrix Omega_bv = build_Omegab_bv(pqr[0], pqr[1], pqr[2]);
  Matrix Cbv_dot_temp = matrix_multiply_3_by_3(Omega_bv, matrix_scalar_multiply(-1, Cbv));
  matrix_integral(Cbv, Cbv_dot_temp, Cbv_dot, timestep);
  Cbv_dot = Cbv_dot_temp;
}


void complementary_filter(float &roll_angle, float &pitch_angle, const std::vector<float> &pqr, const std::vector<float> &accels, const unsigned long &timestep, const float gyro_ratio){
  float timestep_sec = (static_cast<float>(timestep))/1000/1000;
  float delta_roll = pqr[0]*timestep_sec;
  float delta_pitch = pqr[1]*timestep_sec;
  float gyro_roll_angle = roll_angle + delta_roll;
  float gyro_pitch_angle = pitch_angle + delta_pitch;
  
  std::vector<float> accel_angles = accel_angle(accels);
  
  roll_angle  = gyro_ratio*gyro_roll_angle  + (1-gyro_ratio)*accel_angles[1];
  pitch_angle = gyro_ratio*gyro_pitch_angle + (1-gyro_ratio)*accel_angles[0];
}
