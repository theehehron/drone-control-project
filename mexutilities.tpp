template <class MATLABType>
void input2matrix(MATLABType& input, Matrix& mat){
    std::vector<size_t> input_dims = input.getDimensions();
    for (int i = 0; i<input_dims[0]; i++){
        for (int j = 0; j<input_dims[1]; j++){
            mat[i][j] = static_cast<float>(input[i][j]);
        }
    }
}
template <class MATLABType>
void matrix2output(Matrix const& mat, MATLABType& output){
    std::vector<size_t> output_dims = output.getDimensions();
    for (int i = 0; i<output_dims[0]; i++){
        for (int j = 0; j<output_dims[1]; j++){
            output[i][j] = mat[i][j];
        }
    }
}
template <class MATLABType>
void input2vector(MATLABType& input, std::vector<float>& vec){
    std::vector<size_t> input_dims = input.getDimensions();
    for (int i = 0; i<input_dims[0]; i++){
        vec[i] = static_cast<float>(input[i]);
    }
}
template <class MATLABType>
void vector2output(std::vector<float> const& vec, MATLABType& output){
    int it = 0;
    for (auto elem : vec){
        output[it] = elem;
        it++;
    }
}