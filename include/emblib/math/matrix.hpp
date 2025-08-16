#pragma once

#include "details/matrix_native.hpp"

namespace emblib::math {

/**
 * Matrix
 */
template <
    typename scalar_type,
    size_t ROWS,
    size_t COLS = ROWS,
    typename base_type = details::matrix_native_t<scalar_type, ROWS, COLS>
>
class matrix {

public:
    /**
     * Type definition for a matrix of same shape but different scalar
     */
    template <typename other_scalar_type, typename other_base_type>
    using matrix_similar_t = matrix<other_scalar_type, ROWS, COLS, other_base_type>;

    /**
     * Type definition for a matrix same as this one except for the
     * underlying implementation
     */
    template <typename other_base_type>
    using matrix_same_t = matrix_similar_t<scalar_type, other_base_type>;

    /**
     * Used when static casting to this type
     * @note See operator typecast method
     */
    using scalar_type_trait = scalar_type;

public:
    /**
     * Base constructor for creating this wrapper from implementation types
     */
    explicit matrix(base_type base) noexcept : m_base(base) {}

    /**
     * Base constructor for creating this out of another base type
     */
    template <typename other_base>
    matrix(const matrix_same_t<other_base>& other) noexcept : m_base(other.get_base()) {}

    /**
     * Base constructor for implicitly casting a bool matrix to this scalar type
     */
    template <typename other_base, typename = std::enable_if<!std::is_same_v<scalar_type, bool>>>
    matrix(const matrix_similar_t<bool, other_base>& other) noexcept : m_base(other.template cast<scalar_type>().get_base()) {}

    /**
     * Initialize all the elements of the matrix with the same value
     */
    matrix(scalar_type scalar) noexcept;

    /**
     * Initialize the matrix with the elements
     * @todo Change to fixed size arrays
     */
    matrix(std::initializer_list<std::initializer_list<scalar_type>> elements) noexcept;

    /**
     * Get reference to the underlying matrix expression
     */
    const base_type& get_base() const noexcept
    {
        return m_base;
    }

    /**
     * Cast to a different scalar type
     */
    template <typename cast_type>
    auto cast() const noexcept
    {
        auto casted_base = cast_base<cast_type>();
        return matrix_similar_t<cast_type, decltype(casted_base)>(casted_base);
    }

    /**
     * Casting operator
     */
    template <typename cast_type>
    explicit operator cast_type() const noexcept
    {
        return cast<typename cast_type::scalar_type_trait>();
    }

    /**
     * Get element
     */
    scalar_type operator()(size_t row, size_t col) const
    {
        return m_base(row, col);
    }

    /**
     * Get element
     */
    scalar_type& operator()(size_t row, size_t col)
    {
        return m_base(row, col);
    }

    /**
     * Get a reference to a submatrix
     * @todo Create a const version
     */
    template <size_t TOP, size_t LEFT, size_t ROW_COUNT, size_t COL_COUNT>
    auto get_submatrix() noexcept;

    /**
     * Transpose
     */
    auto transpose() const noexcept;

    /**
     * Element-wise addition
     */
    template <typename rhs_base>
    auto operator+(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise addition with a scalar
     */
    auto operator+(const scalar_type& rhs) const noexcept;

    /**
     * Element-wise addition in-place
     */
    template <typename rhs_base>
    void operator+=(const matrix_same_t<rhs_base>& rhs) noexcept;

    /**
     * Element-wise addition in-place with a scalar
     */
    void operator+=(const scalar_type& rhs) noexcept;

    /**
     * Element-wise subtraction
     */
    template <typename rhs_base>
    auto operator-(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise subtraction with a scalar
     */
    auto operator-(const scalar_type& rhs) const noexcept;

    /**
     * Element-wise subtraction in-place
     */
    template <typename rhs_base>
    void operator-=(const matrix_same_t<rhs_base>& rhs) noexcept;

    /**
     * Element-wise subtraction in-place with a scalar
     */
    void operator-=(const scalar_type& rhs) noexcept;

    /**
     * Element-wise multiplication
     */
    template <typename rhs_scalar, typename rhs_base>
    auto operator*(const matrix_similar_t<rhs_scalar, rhs_base>& rhs) const noexcept;

    /**
     * Element-wise multiplication with a scalar
     */
    template <typename rhs_scalar>
    auto operator*(const rhs_scalar& rhs) const noexcept;

    /**
     * Element-wise multiplication in-place
     */
    template <typename rhs_base>
    void operator*=(const matrix_same_t<rhs_base>& rhs) noexcept;

    /**
     * Element-wise in-place multiplication with a scalar
     */
    void operator*=(const scalar_type& rhs) noexcept;

    /**
     * Element-wise division
     */
    template <typename rhs_scalar, typename rhs_base>
    auto operator/(const matrix_similar_t<rhs_scalar, rhs_base>& rhs) const noexcept;

    /**
     * Element-wise division with a scalar
     */
    template <typename rhs_scalar>
    auto operator/(const rhs_scalar& rhs) const noexcept;

    /**
     * Element-wise division in-place
     */
    template <typename rhs_base>
    void operator/=(const matrix_same_t<rhs_base>& rhs) noexcept;

    /**
     * Element-wise in-place division with a scalar
     */
    void operator/=(const scalar_type& rhs) noexcept;

    /**
     * Element-wise negative
     */
    auto operator-() const noexcept;

    /**
     * Element-wise less than
     */
    template <typename rhs_base>
    auto operator<(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise less than or equal to
     */
    template <typename rhs_base>
    auto operator<=(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise less than
     */
    template <typename rhs_base>
    auto operator>(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise less than or equal to
     */
    template <typename rhs_base>
    auto operator>=(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise equal
     */
    template <typename rhs_base>
    auto operator==(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise logical and
     */
    template <typename rhs_base>
    auto operator&&(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise logical or
     */
    template <typename rhs_base>
    auto operator||(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise logical not
     */
    auto operator!() const noexcept;

    /**
     * Are all elements non-null
     */
    bool all() const noexcept;

    /**
     * Are any elements non-null
     */
    bool any() const noexcept;

    /**
     * Fill all elements with the same value
     */
    void fill(scalar_type scalar) noexcept;

    /**
     * Matrix multiplication
     */
    template <size_t RHS_COLS, typename rhs_scalar, typename rhs_base>
    auto matmul(const matrix<rhs_scalar, COLS, RHS_COLS, rhs_base>& rhs) const noexcept;

    /**
     * Equivalent to multiplying this matrix from the left by the inverse of the divisor
     */
    template <typename divisor_base>
    auto matdivl(const matrix<scalar_type, ROWS, ROWS, divisor_base>& divisor) const noexcept;

    /**
     * Equivalent to multiplying this matrix from the right by the inverse of the divisor
     */
    template <typename divisor_base>
    auto matdivr(const matrix<scalar_type, COLS, COLS, divisor_base>& divisor) const noexcept
    {
        return matrix<scalar_type, ROWS, COLS>(transpose().matdivl(divisor.transpose()).transpose());
    }

    /**
     * Assign a submatrix
     */
    template <size_t OTHER_ROWS, size_t OTHER_COLS, typename other_base>
    void set_submatrix(
        size_t row_begin,
        size_t col_begin,
        const matrix<scalar_type, OTHER_ROWS, OTHER_COLS, other_base>& other
    ) noexcept
    {
        if ((row_begin + OTHER_ROWS > ROWS) || (col_begin + OTHER_COLS > COLS))
            return;

        for (size_t r = 0; r < OTHER_ROWS; r++) {
            for (size_t c = 0; c < OTHER_COLS; c++)
                (*this)(row_begin + r, col_begin + c) = other(r, c);
        }
    }

    /**
     * Diagonal matrix
     */
    static matrix diagonal(scalar_type diag_elem = 1) noexcept
    {
        static_assert(ROWS == COLS);
        matrix result(0);
        for (size_t i = 0; i < ROWS; i++)
            result(i, i) = diag_elem;
        return result;
    }

private:
    template <typename cast_type>
    auto cast_base() const noexcept;

private:
    base_type m_base;
};

/** @todo Add external operators for scalar + matrix, ... */

/**
 * External operator for multiplying a scalar by a matrix
 */
template <typename scalar_type, size_t ROWS, size_t COLS, typename matrix_base>
auto operator*(const scalar_type& lhs, const matrix<scalar_type, ROWS, COLS, matrix_base>& rhs) noexcept
{
    return rhs * lhs;
}

/**
 * Convinience typedef for floating point matrices
 */
template <size_t ROWS, size_t COLS = ROWS>
using matrixf = matrix<float, ROWS, COLS>;

}

#include "details/matrix_impl.hpp"
