#ifndef MEXUTILITIES_HPP
#define MEXUTILITIES_HPP

#include <vector>

typedef std::vector<std::vector<float>> Matrix;

template <class MATLABType>
void input2matrix(MATLABType& input, Matrix& mat);

template <class MATLABType>
void matrix2output(Matrix const& mat, MATLABType& output);

template <class MATLABType>
void input2vector(MATLABType& input, std::vector<float>& vec);

template <class MATLABType>
void vector2output(std::vector<float> const& vec, MATLABType& output);

#include "mexutilities.tpp"

#endif // MEXUTILITIES_HPP