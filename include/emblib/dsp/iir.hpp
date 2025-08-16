#pragma once

#include <etl/array.h>
#include <etl/vector.h>

namespace emblib::dsp {

/**
 * Assuming a transfer function of the form:
 * H(z) = B(z)/A(z)
 * where B(z) is a polynomial of degree at most `ORDER-1` and A(z) is
 * a polynomial of degree `ORDER` with the constant coefficient equal to 1
 * @param num_coeffs Coefficients of B(z) starting from b0
 * @param den_coeffs Coefficients of A(z) starting from a1 (a0 = 1)
 */
template <
    typename scalar_type,
    size_t ORDER,
    typename coeff_type = scalar_type
>
class iir_tf2 {

public:
    explicit iir_tf2(
        etl::vector<coeff_type, ORDER> num_coeffs,
        etl::array<coeff_type, ORDER> den_coeffs
    ) : m_num_coeffs(num_coeffs), m_den_coeffs(den_coeffs)
    {}

    void update(const scalar_type& input) noexcept
    {
        m_output = m_num_coeffs[0] * input + m_delay_line[0];

        for (size_t i = 0; i < m_num_coeffs.size()-1; i++) {
            m_delay_line[i] = m_num_coeffs[i+1] * input - m_den_coeffs[i] * m_output + m_delay_line[i+1];
        }
        for (size_t i = m_num_coeffs.size()-1; i < ORDER-1; i++) {
            m_delay_line[i] = m_delay_line[i+1] - m_den_coeffs[i] * m_output;
        }
        m_delay_line[ORDER-1] = -m_den_coeffs[ORDER-1] * m_output;
    }

    scalar_type get_output() const noexcept
    {
        return m_output;
    }

private:
    const etl::vector<coeff_type, ORDER> m_num_coeffs;
    const etl::array<coeff_type, ORDER> m_den_coeffs;

    scalar_type m_delay_line[ORDER];
    scalar_type m_output;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace dsp;
}
#endif