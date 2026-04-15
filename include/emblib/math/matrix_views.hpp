#pragma once

#include <cstddef>
#include <type_traits>

namespace emblib::math {

/**
 * A view represents a view over the elements of a matrix.
 * Views do not own any data, they just provide an access point to each
 * element. This allows no-copy transformations on the matrices using
 * lazy evaluation of each element at access time.
 */

/**
 * Get the type of a view element.
 */
template <typename V>
using view_data_type = std::invoke_result_t<const V, size_t, size_t>;

/**
 * Basic view is a view over a 2D array of elements with a trivial access pattern.
 */
template <typename T, size_t R, size_t C>
class basic_view {
    using storage_type = T[R][C];

public:
    basic_view(storage_type& storage) : m_storage(storage) {}

    T operator()(size_t i, size_t j) const noexcept
    {
        return m_storage[i][j];
    }

private:
    storage_type& m_storage;
};

/**
 * Scalar op view defines an operation between a matrix and a scalar.
 */
template <typename V, typename T, typename Op>
class scalar_op_view {
public:
    scalar_op_view(V view, T scalar) : m_view(view), m_scalar(scalar) {}

    auto operator()(size_t i, size_t j) const noexcept
    {
        return Op()(m_view(i, j), m_scalar);
    }

private:
    V m_view;
    T m_scalar;
};

/**
 * Element wise binary operation between two views of matrices.
 */
template <typename V1, typename V2, typename Op>
class ewise_op_view {
public:
    ewise_op_view(V1 lhs, V2 rhs) : m_lhs(lhs), m_rhs(rhs) {}

    auto operator()(size_t i, size_t j) const noexcept
    {
        return Op()(m_lhs(i, j), m_rhs(i, j));
    }

private:
    V1 m_lhs;
    V2 m_rhs;
};

/**
 * Transposed view of a matrix.
 */
template <typename V>
class transpose_view {
public:
    transpose_view(V view) : m_view(view) {}

    auto operator()(size_t i, size_t j) const noexcept
    {
        return m_view(j, i);
    }

private:
    V m_view;
};

template <typename L, typename R = L>
struct bin_op_plus {
    auto operator()(const L& lhs, const R& rhs) const noexcept { return lhs + rhs; }
};

}
