#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_EIGEN
    #include <Eigen/Dense>
#elif EMBLIB_MATH_USE_XTENSOR
    #define XTENSOR_ENABLE_ASSERT
    #define XTENSOR_CHECK_DIMENSIONS
    #include <xtensor/xfixed.hpp>
    #include <xtensor-blas/xlinalg.hpp>
#else
    #include <array>
#endif

namespace emblib::math::details {

#if EMBLIB_MATH_USE_EIGEN
    template <typename scalar_type, size_t ROWS, size_t COLS = ROWS>
    using matrix_native_t = Eigen::Matrix<scalar_type, ROWS, COLS>;
#else
    #error "Matrix implementation not defined"
#endif

}