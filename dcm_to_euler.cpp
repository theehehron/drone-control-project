#include <vector>
#include "mex.hpp"
#include "mexAdapter.hpp"
#include "mexutilities.hpp"
#include "attitude_representation.hpp"

class MexFunction : public matlab::mex::Function {
public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
        // Initialize Array Factory for creating matlab::data type arrays
        matlab::data::ArrayFactory factory;
        checkArguments(outputs, inputs);
        
        // Unpack the inputs
        matlab::data::TypedArray<double> input1 = std::move(inputs[0]);
        
        // create empty 3x3 "Matrix" aka std::vector<std::vector<float>>
        size_t rows = 3;
        size_t cols = 3;
        Matrix Cbv(rows, std::vector<float>(cols, 0));
        
        // Fill the matrix elements with the input elements
        input2matrix<matlab::data::TypedArray<double>>(input1, Cbv);
        
        // Implement function to be tested.
        std::vector<float> result = dcm_to_euler(Cbv);

        // Create an empty matlab::data::TypedArray with the same dimensions as the output.
        // and pack the resulting "Matrix" into the matlab::data::TypedArray
        matlab::data::TypedArray<float> out = factory.createArray<float>({3,1}, {0, 0, 0, 0});
        vector2output<matlab::data::TypedArray<float>>(result, out);
        
        // Send the output outta here
        outputs[0] = std::move(out);
    }
    void checkArguments(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs){
      std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
      matlab::data::ArrayFactory factory;

      if (inputs.size() != 1) {
            matlabPtr->feval(u"error", 
                0, std::vector<matlab::data::Array>({ factory.createScalar("Enter DCM as 3x3 Matrix aka 'std::vector<std::vector<float>>'.") }));
      }
      if (inputs[0].getNumberOfElements() != 9) {
            matlabPtr->feval(u"error", 
                0, std::vector<matlab::data::Array>({ factory.createScalar("Enter DCM as 3x3 Matrix aka 'std::vector<std::vector<float>>'.") }));
      }
    }
};