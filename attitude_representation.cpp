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



Matrix euler_to_dcm(std::vector<float> &euler_angles){
    // takes vector [phi, theta, psi] and computes DCM
    
    std::cout << euler_angles[0] << ", " << euler_angles[1] << ", " << euler_angles[2] << std::endl;
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



std::vector<float> dcm_to_quat(Matrix const &Cbv){
    // returns quaternion representation corresponding to the DCM Cbv
    float q_tilde1 = static_cast<float>(pow((.25*(1+Cbv[0][0]+Cbv[1][1]+Cbv[2][2])), .5));
    float q_tilde2 = static_cast<float>(pow((.25*(1+Cbv[0][0]-Cbv[1][1]-Cbv[2][2])), .5));
    float q_tilde3 = static_cast<float>(pow((.25*(1-Cbv[0][0]+Cbv[1][1]-Cbv[2][2])), .5));
    float q_tilde4 = static_cast<float>(pow((.25*(1-Cbv[0][0]-Cbv[1][1]+Cbv[2][2])), .5));

    float q_tilde[4] = {q_tilde1, q_tilde2, q_tilde3, q_tilde4};

    int n = sizeof(q_tilde) / sizeof(q_tilde[0]);
    auto max_element = std::max_element(q_tilde, q_tilde + n);
    int max_index = std::distance(q_tilde, max_element);
    
    std::vector<float> q(4, 0);
    switch (max_index)
    {
    case 1:
        // std::cout << "1";
        q[0] = (Cbv[1][2]-Cbv[2][1])/(4*q_tilde[1]);
        q[1] = q_tilde[1];
        q[2] = (Cbv[0][1]+Cbv[1][0])/(4*q_tilde[1]);
        q[3] = (Cbv[2][0]+Cbv[0][2])/(4*q_tilde[1]);
        break;
    case 2:
        // std::cout << "2";
        q[0] = (Cbv[2][0]-Cbv[0][2])/(4*q_tilde[2]);
        q[1] = (Cbv[0][1]+Cbv[1][0])/(4*q_tilde[2]);
        q[2] = q_tilde[2];
        q[3] = (Cbv[1][2]+Cbv[2][1])/(4*q_tilde[2]);
        break;
    case 3:
        // std::cout << "3";
        q[0] = (Cbv[1][0]-Cbv[0][1])/(4*q_tilde[3]);
        q[1] = (Cbv[0][2]+Cbv[2][0])/(4*q_tilde[3]);
        q[2] = (Cbv[1][2]+Cbv[2][1])/(4*q_tilde[3]);
        q[3] = q_tilde[3];
        break;
    default:
        // std::cout << "0";
        q[0] = q_tilde[0];
        q[1] = (Cbv[1][2]-Cbv[2][1])/(4*q_tilde[0]);
        q[2] = (Cbv[2][0]-Cbv[0][2])/(4*q_tilde[0]);
        q[3] = (Cbv[0][1]-Cbv[1][0])/(4*q_tilde[0]);
        break;
    }
    return q;
}