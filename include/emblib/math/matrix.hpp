#pragma once

#include "matrix_views.hpp"
#include "ops.hpp"

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

    template <size_t SR, size_t SC>
    auto submatrix(size_t top, size_t left) const noexcept
    {
        return vmatrix<SR, SC, submatrix_view<V>>(submatrix_view<V>(m_view, top, left));
    }

    auto transpose() const noexcept
    {
        return vmatrix<C, R, transpose_view<V>>(transpose_view<V>(m_view));
    }

    template <size_t K, typename RV>
    auto matmul(const vmatrix<C, K, RV>& rhs) const noexcept
    {
        return vmatrix<R, K, matmul_view<V, RV, C>>(matmul_view<V, RV, C>(m_view, rhs.m_view));
    }

    auto operator()(size_t i, size_t j) const noexcept
    {
        return m_view(i, j);
    }

    template <typename RV>
    auto operator+(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_add<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    auto operator+(T scalar) const noexcept
    {
        using op = bin_op_add<view_data_type<V>, T>;
        using op_view = scalar_op_view<V, T, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, scalar));
    }

    template <typename RV>
    auto operator-(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_sub<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    auto operator-(T scalar) const noexcept
    {
        using op = bin_op_sub<view_data_type<V>, T>;
        using op_view = scalar_op_view<V, T, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, scalar));
    }

    template <typename RV>
    auto operator*(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_mul<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    auto operator*(T scalar) const noexcept
    {
        using op = bin_op_mul<view_data_type<V>, T>;
        using op_view = scalar_op_view<V, T, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, scalar));
    }

    template <typename RV>
    auto operator/(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_div<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    auto operator/(T scalar) const noexcept
    {
        using op = bin_op_div<view_data_type<V>, T>;
        using op_view = scalar_op_view<V, T, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, scalar));
    }

    template <typename RV>
    auto operator==(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_eq<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename RV>
    auto operator!=(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_neq<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename RV>
    auto operator<(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_lt<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename RV>
    auto operator>(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_gt<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename RV>
    auto operator<=(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_lte<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    template <typename RV>
    auto operator>=(const vmatrix<R, C, RV>& rhs) const noexcept
    {
        using op = bin_op_gte<view_data_type<V>, view_data_type<RV>>;
        using op_view = ewise_op_view<V, RV, op>;
        return vmatrix<R, C, op_view>(op_view(m_view, rhs.m_view));
    }

    bool all() const noexcept
    {
        for (size_t i = 0; i < R; i++)
            for (size_t j = 0; j < C; j++)
                if (!m_view(i, j)) return false;
        return true;
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
        this->operator=<basic_view<T, R, C>>(other);
    }

    matrix(matrix&& other) noexcept : base(basic_view<T, R, C>(m_data))
    {
        this->operator=<basic_view<T, R, C>>(other);
    }

    template <typename V>
    matrix& operator=(const vmatrix<R, C, V>& rhs) noexcept
    {
        static_assert(std::is_same_v<T, view_data_type<V>>);
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
