#pragma once


namespace emblib::dsp {

template <typename scalar_type, typename coeff_type = scalar_type>
class pid {

public:
    pid(coeff_type kp,
        coeff_type ki,
        coeff_type kd
    ) noexcept :
        m_kp(kp),
        m_ki(ki),
        m_kd(kd),
        m_clamp(false)
    {}

    pid(
        coeff_type kp,
        coeff_type ki,
        coeff_type kd,
        scalar_type clamp_low,
        scalar_type clamp_high
    ) noexcept :
        m_kp(kp),
        m_ki(ki),
        m_kd(kd),
        m_clamp(true),
        m_clamp_low(clamp_low),
        m_clamp_high(clamp_high),
        m_saturated(0)
    {}

    /**
     * Update PID state with the next input value
    */
    void update(const scalar_type& input, coeff_type dt = coeff_type(1)) noexcept
    {
        m_output = m_kp * input + (m_ki * dt) * m_integral + (m_kd / dt) * (input - m_prev_input);

        if (m_clamp) {
            // Only contributing to the integral when not saturated
            m_integral += static_cast<scalar_type>(!m_saturated) * input;

            // Check whether there is saturation
            auto sat_high = m_output >= m_clamp_high;
            auto sat_low = m_output <= m_clamp_low;
            m_saturated = sat_high || sat_low;
            
            // Vector (arithmetic) implementation of clamping the output
            m_output = static_cast<scalar_type>(!sat_high) * m_output + static_cast<scalar_type>(sat_high) * m_clamp_high;
            m_output = static_cast<scalar_type>(!sat_low) * m_output + static_cast<scalar_type>(sat_low) * m_clamp_low;
        } else {
            m_integral += input;
        }
        
        m_prev_input = input;
    }

    scalar_type get_output() const noexcept
    {
        return m_output;
    }

private:
    const coeff_type m_kp;
    const coeff_type m_ki;
    const coeff_type m_kd;

    bool m_clamp;
    scalar_type m_clamp_low;
    scalar_type m_clamp_high;
    scalar_type m_saturated;

    scalar_type m_output = 0;
    scalar_type m_integral = 0;
    scalar_type m_prev_input = 0;

};

}
