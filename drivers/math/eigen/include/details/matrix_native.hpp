#pragma once

#include <Eigen/Dense>

namespace emblib::math::details {

template <typename scalar_type, size_t ROWS, size_t COLS = ROWS>
using matrix_native_t = Eigen::Matrix<scalar_type, ROWS, COLS>;

}