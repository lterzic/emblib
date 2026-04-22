#pragma once

#include "matrix_views.hpp"
#include "ops.hpp"

#include <cmath>
#include <limits>

namespace emblib::math {

template <typename T, size_t R, size_t C>
class matrix;

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

    // Solves A·X = B where *this is A (square N×N) and B is N×K.
    // Uses LU decomposition with partial pivoting on a stack-allocated augmented matrix.
    // Returns NaN-filled matrix if A is singular.
    template <size_t K, typename BV>
    auto matdivl(const vmatrix<R, K, BV>& B) const noexcept
        -> matrix<view_data_type<V>, R, K>
    {
        static_assert(R == C, "matdivl requires a square matrix");
        using T = view_data_type<V>;
        static_assert(std::is_floating_point_v<T>, "matdivl requires floating-point element type");

        constexpr size_t N = R;

        T aug[N][N + K];
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++)
                aug[i][j] = (*this)(i, j);
            for (size_t k = 0; k < K; k++)
                aug[i][N + k] = B(i, k);
        }

        for (size_t col = 0; col < N; col++) {
            size_t pivot_row = col;
            T max_val = std::abs(aug[col][col]);
            for (size_t row = col + 1; row < N; row++) {
                T val = std::abs(aug[row][col]);
                if (val > max_val) { max_val = val; pivot_row = row; }
            }

            if (max_val == T(0)) {
                matrix<T, N, K> nan_result;
                for (size_t i = 0; i < N; i++)
                    for (size_t k = 0; k < K; k++)
                        nan_result(i, k) = std::numeric_limits<T>::quiet_NaN();
                return nan_result;
            }

            if (pivot_row != col)
                for (size_t j = 0; j < N + K; j++)
                    std::swap(aug[col][j], aug[pivot_row][j]);

            for (size_t row = col + 1; row < N; row++) {
                T factor = aug[row][col] / aug[col][col];
                for (size_t j = col; j < N + K; j++)
                    aug[row][j] -= factor * aug[col][j];
            }
        }

        matrix<T, N, K> result;
        for (size_t k = 0; k < K; k++) {
            for (size_t i = N; i-- > 0; ) {
                T sum = aug[i][N + k];
                for (size_t j = i + 1; j < N; j++)
                    sum -= aug[i][j] * result(j, k);
                result(i, k) = sum / aug[i][i];
            }
        }
        return result;
    }

    // Solves X·A = B where *this is B (R×N) and A is square (N×N).
    // Implemented as X = (Aᵀ \ Bᵀ)ᵀ using matdivl.
    template <typename AV>
    auto matdivr(const vmatrix<C, C, AV>& A) const noexcept
        -> matrix<view_data_type<V>, R, C>
    {
        using T = view_data_type<V>;
        static_assert(std::is_floating_point_v<T>, "matdivr requires floating-point element type");
        matrix<T, R, C> result = A.transpose().matdivl(this->transpose()).transpose();
        return result;
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
    matrix(const vmatrix<R, C, V>& other) noexcept : base(basic_view<T, R, C>(m_data))
    {
        this->operator=<V>(other);
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
