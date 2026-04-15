#pragma once

#include "matrix_views.hpp"
#include <array>

namespace emblib::math {

/**
 * Definition of the matrix operations on a view of elements.
 */
template <size_t R, size_t C, typename V>
class vmatrix {
    template <size_t, size_t, typename>
    friend class vmatrix;

public:
    vmatrix(V view) : m_view(view) {}

    auto operator()(size_t i, size_t j) const noexcept
    {
        return m_view(i, j);
    }

    template <typename RV>
    auto operator+(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_plus<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename T>
    auto operator+(T scalar) const noexcept
    {
        using op = bin_op_plus<view_data_type<V>, T>;
        using op_view = scalar_op_view<V, T, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, scalar));
    }

private:
    V m_view;
};

/**
 * Matrix is a view which owns its data and allows modifying access.
 */
template <typename T, size_t R, size_t C>
class matrix : public vmatrix<R, C, basic_view<T, R, C>> {
    using base = vmatrix<R, C, basic_view<T, R, C>>;

public:
    explicit matrix() noexcept : base(basic_view<T, R, C>(m_data)) {}

    explicit matrix(T value) noexcept : base(basic_view<T, R, C>(m_data))
    {
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                m_data[i][j] = value;
            }
        }
    }

    matrix(const T (&data)[R][C]) noexcept : base(basic_view<T, R, C>(m_data))
    {
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                m_data[i][j] = data[i][j];
            }
        }
    }

    matrix(const matrix& other) noexcept : base(basic_view<T, R, C>(m_data))
    {
        *this = other;
    }

    matrix(matrix&& other) noexcept : base(basic_view<T, R, C>(m_data))
    {
        *this = other;
    }

    template <typename V>
    matrix& operator=(const vmatrix<R, C, V>& rhs) noexcept
    {
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                m_data[i][j] = rhs(i, j);
            }
        }
        return *this;
    }

    T& operator()(size_t i, size_t j) noexcept
    {
        return m_data[i][j];
    }

    T operator()(size_t i, size_t j) const noexcept
    {
        return m_data[i][j];
    }

private:
    T m_data[R][C];
};

}
