// THIS FILE IS CURRENTLY NOT USED. THE FUNCTIONS BELOW ARE NOW FUNCTION 
// TEMPLATES. THE IMPLEMENTATION OF THE FUNCTION TEMPLATE MUST BE INCLUDED
// IN THE HEADER (.hpp) FILE. THIS IS ALL A DUMB WORKAROUND TO GET MEX 
// WORKING.

// #include "mexutilities.hpp"

// void input2matrix(matlab::data::TypedArray<double>& input, Matrix& mat){
//     std::vector<size_t> input_dims = input.getDimensions();
//     for (int i = 0; i<input_dims[0]; i++){
//         for (int j = 0; j<input_dims[1]; j++){
//             mat[i][j] = static_cast<float>(input[i][j]);
//         }
//     }
// }
// void matrix2output(Matrix const& mat, matlab::data::TypedArray<float>& output){
//     std::vector<size_t> output_dims = output.getDimensions();
//     for (int i = 0; i<output_dims[0]; i++){
//         for (int j = 0; j<output_dims[1]; j++){
//             output[i][j] = mat[i][j];
//         }
//     }
// }
