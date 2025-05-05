#pragma once

#include "emblib/emblib.hpp"
#include "matrix.hpp"
#include <cmath>

namespace emblib::math {

/**
 * Vector is a 1-column matrix
 */
template <
    typename scalar_type,
    size_t DIM,
    typename base_type = details::matrix_native_t<scalar_type, DIM, 1>
>
class vector : public matrix<scalar_type, DIM, 1, base_type> {

public:
    template <typename other_base>
    using vector_same_t = vector<scalar_type, DIM, other_base>;

    vector(scalar_type scalar = 0) :
        matrix<scalar_type, DIM, 1, base_type>(scalar) {}

    vector(const std::initializer_list<scalar_type>& elements) :
        matrix<scalar_type, DIM, 1, base_type>({elements}) {}

    template <typename other_base>
    vector(const matrix<scalar_type, DIM, 1, other_base>& matrix) :
        matrix<scalar_type, DIM, 1, base_type>(matrix) {}

    template <typename other_base, typename = std::enable_if<!std::is_same_v<scalar_type, bool>>>
    vector(const matrix<bool, DIM, 1, other_base>& matrix) :
        matrix<scalar_type, DIM, 1, base_type>(matrix.template cast<scalar_type>()) {}

    /**
     * Get element as a copy
     */
    scalar_type operator()(size_t idx) const
    {
        return matrix<scalar_type, DIM, 1, base_type>::operator()(idx, 0);
    }

    /**
     * Get element by reference
     */
    scalar_type& operator()(size_t idx)
    {
        return matrix<scalar_type, DIM, 1, base_type>::operator()(idx, 0);
    }

    /**
     * Dot product
     */
    template <typename rhs_base>
    scalar_type dot(const vector_same_t<rhs_base>& rhs) const noexcept
    {
        const auto res = this->transpose().matmul(rhs);
        return res(0, 0);
    }

    /**
     * Cross product
     */
    template <typename rhs_base>
    vector cross(const vector_same_t<rhs_base>& rhs) const noexcept
    {
        static_assert(DIM == 3);
        auto lhs = *this;
        return vector {
            lhs(1) * rhs(2) - lhs(2) * rhs(1),
            lhs(2) * rhs(0) - lhs(0) * rhs(2),
            lhs(0) * rhs(1) - lhs(1) * rhs(0)
        };
    }

    /**
     * Compute the square of the norm
     */
    scalar_type norm_sq() const noexcept
    {
        scalar_type res {0};
        for (size_t i = 0; i < DIM; i++) {
            auto elem = (*this)(i);
            res += elem * elem;
        }
        return res;
    }
    
    /**
     * Compute the norm
     */
    scalar_type norm() const noexcept
    {
        return std::sqrt(norm_sq());
    }

    /**
     * Returns a new vector with the same direction
     * and unit magnitude
     */
    vector normalized() const noexcept
    {
        return *this / norm();
    }

    /**
     * Create a square matrix with elements of this vector
     * as diagonal elements
     */
    matrix<scalar_type, DIM> as_diagonal() const noexcept
    {
        matrix<scalar_type, DIM> result {0};
        for (size_t i = 0; i < DIM; i++) {
            result(i, i) = (*this)(i);
        }
        return result;
    }
};

/**
 * Multiplication operator overloading to prevent treating
 * a vector as a scalar when multiplying a matrix
 */
template <typename lhs_scalar, typename rhs_scalar, size_t DIM, typename lhs_base, typename rhs_base>
auto operator*(const matrix<lhs_scalar, DIM, 1, lhs_base>& lhs, const vector<rhs_scalar, DIM, rhs_base>& rhs) noexcept
{
    return lhs * static_cast<const matrix<rhs_scalar, DIM, 1, rhs_base>&>(rhs);
}

/**
 * Division operator overloading to prevent treating
 * a vector as a scalar when dividing a matrix
 */
template <typename lhs_scalar, typename rhs_scalar, size_t DIM, typename lhs_base, typename rhs_base>
auto operator/(const matrix<lhs_scalar, DIM, 1, lhs_base>& lhs, const vector<rhs_scalar, DIM, rhs_base>& rhs) noexcept
{
    return lhs / static_cast<const matrix<rhs_scalar, DIM, 1, rhs_base>&>(rhs);
}

template <size_t DIM>
using vectorf = vector<float, DIM>;

using vector3f = vectorf<3>;

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace math;
}
#endif