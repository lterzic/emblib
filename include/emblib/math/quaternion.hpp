#pragma once

#include "vector.hpp"

namespace emblib::math {

template <typename scalar_type>
class quaternion {

public:
    quaternion(scalar_type w, scalar_type x, scalar_type y, scalar_type z) noexcept :
        m_w(w), m_x(x), m_y(y), m_z(z) {}

    quaternion(scalar_type real, const vector<scalar_type, 3>& imag) noexcept :
        m_w(real), m_x(imag(0)), m_y(imag(1)), m_z(imag(2)) {}

    scalar_type get_real() const noexcept
    {
        return m_w;
    }

    vector<scalar_type, 3> get_imag() const noexcept
    {
        return {m_x, m_y, m_z};
    }

    quaternion conjugate() const noexcept
    {
        return {m_w, -m_x, -m_y, -m_z};
    }

    quaternion operator+(const quaternion& rhs) const noexcept
    {
        return {m_w + rhs.m_w, m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z};
    }

    quaternion operator*(const scalar_type& s) const noexcept
    {
        return {m_w * s, m_x * s, m_y * s, m_z * s};
    }

    quaternion operator*(const quaternion& rhs) const noexcept
    {
        return {
            m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z,
            m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y,
            m_w * rhs.m_y - m_x * rhs.m_z + m_y * rhs.m_w + m_z * rhs.m_x,
            m_w * rhs.m_z + m_x * rhs.m_y - m_y * rhs.m_x + m_z * rhs.m_w,
        };
    }

    vector<scalar_type, 3> rotate_vec(const vector<scalar_type, 3>& vec) const noexcept
    {
        return ((*this) * quaternion(0, vec) * conjugate()).get_imag();
    }

    vector<scalar_type, 4> as_vector() const noexcept
    {
        return {m_w, m_x, m_y, m_z};
    }

private:
    scalar_type m_w, m_x, m_y, m_z;
};

template <typename scalar_type>
quaternion<scalar_type> operator*(const scalar_type& lhs, const quaternion<scalar_type>& rhs) noexcept
{
    return rhs * lhs;
}

using quaternionf = quaternion<float>;

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace math;
}
#endif