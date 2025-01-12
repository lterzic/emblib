#pragma once

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline matrix<scalar_type, ROWS, COLS, base_type>::matrix(scalar_type scalar) noexcept
    : m_base(matrix_native_t<scalar_type, ROWS, COLS>::Constant(scalar))
{
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline matrix<scalar_type, ROWS, COLS, base_type>::matrix(const std::initializer_list<std::initializer_list<scalar_type>>& elements) noexcept
    : m_base(elements)
{
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <size_t TOP, size_t LEFT, size_t ROW_COUNT, size_t COL_COUNT>
inline auto emblib::math::matrix<scalar_type, ROWS, COLS, base_type>::get_submatrix() noexcept
{
    static_assert((TOP + ROW_COUNT <= ROWS) && (LEFT + COL_COUNT <= COLS));
    auto res = m_base.template block<ROW_COUNT, COL_COUNT>(TOP, LEFT);
    return matrix<scalar_type, ROW_COUNT, COL_COUNT, decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::transpose() const noexcept
{
    auto res = m_base.transpose();
    return matrix<scalar_type, COLS, ROWS, decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator+(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = m_base + rhs.get_base();
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator-(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = m_base - rhs.get_base();
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator*(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() * rhs.get_base().array()).matrix();
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_scalar, typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator*(const matrix<rhs_scalar, ROWS, COLS, rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array().template cast<scalar_type>() * rhs.get_base().array()).matrix();
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator*(const scalar_type &rhs) const noexcept
{
    auto res = m_base * rhs;
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline void matrix<scalar_type, ROWS, COLS, base_type>::operator*=(const scalar_type &rhs) noexcept
{
    m_base *= rhs;
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator/(const scalar_type &rhs) const noexcept
{
    auto res = m_base / rhs;
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline void matrix<scalar_type, ROWS, COLS, base_type>::operator/=(const scalar_type &rhs) noexcept
{
    m_base /= rhs;
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator/(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() / rhs.get_base().array()).matrix();
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator-() const noexcept
{
    auto res = -m_base;
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator<(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() < rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator<=(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() <= rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator>(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() > rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator>=(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() >= rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator==(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() == rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator&&(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() && rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator||(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() || rhs.get_base().array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator!() const noexcept
{
    auto res = (!m_base.array()).matrix();
    return matrix_bool_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline bool matrix<scalar_type, ROWS, COLS, base_type>::all() const noexcept
{
    return m_base.all();
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline bool matrix<scalar_type, ROWS, COLS, base_type>::any() const noexcept
{
    return m_base.any();
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline void matrix<scalar_type, ROWS, COLS, base_type>::fill(scalar_type scalar) noexcept
{
    m_base.fill(scalar);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <size_t COLS_RHS, typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::matmul(const matrix<scalar_type, COLS, COLS_RHS, rhs_base> &rhs) const noexcept
{
    auto res = m_base * rhs.get_base();
    return matrix<scalar_type, ROWS, COLS_RHS, decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename divisor_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::matdivl(const matrix<scalar_type, ROWS, ROWS, divisor_base> &divisor) const noexcept
{
    Eigen::Matrix<scalar_type, ROWS, COLS> res = divisor.get_base().colPivHouseholderQr().solve(m_base);
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename cast_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::cast_base() const noexcept
{
    return m_base.template cast<cast_type>();
}