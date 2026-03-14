#pragma once

#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::dsp {

/**
 * Kalman filter
 */
template <size_t STATE_DIM, typename scalar_type = float>
class kalman {
    template <size_t DIM>
    using vec_t = math::vector<scalar_type, DIM>;
    template <size_t ROWS, size_t COLS = ROWS>
    using mat_t = math::matrix<scalar_type, ROWS, COLS>;

public:
    explicit kalman() noexcept :
        m_state(0),
        m_p(0)
    {}

    explicit kalman(vec_t<STATE_DIM> initial_state) noexcept :
        m_state(initial_state),
        m_p(0)
    {}

    /**
     * Kalman filter prediction step for a non-linear (generic) model
     * @param f State transition - Calculate the expected next state based on the current state
     * @param F State jacobian - Derivative of `f` with respect to the state
     * @param Q State transition noise covariance matrix
     * @see https://en.wikipedia.org/wiki/Extended_Kalman_filter
     * @todo `F` could return const matrix reference
     */
    template <typename f_type, typename F_type>
    void predict(f_type&& f, F_type&& F, const mat_t<STATE_DIM>& Q) noexcept
    {
        static_assert(std::is_invocable_r_v<vec_t<STATE_DIM>, f_type, const vec_t<STATE_DIM>&>);
        static_assert(std::is_invocable_r_v<mat_t<STATE_DIM>, F_type, const vec_t<STATE_DIM>&>);

        // State jacobian
        const auto Fj = F(m_state);

        // Compute the prediction
        m_state = f(m_state);
        m_p = Fj.matmul(m_p).matmul(Fj.transpose()) + Q;
    }

    /**
     * Kalman filter update step using an observation based on
     * previously predicted state
     * @param h Calculate the expected observation from a given state
     * @param H State to observation jacobian - Derivative of `h` with respect to state
     * @param R Observation (measurement) noise covariance matrix
     * @param observation Measurement vector
     * @see https://en.wikipedia.org/wiki/Extended_Kalman_filter
     * @note The calculations are based on the last called `predict` method
     * @todo `H` could return const matrix reference
     */
    template <size_t OBS_DIM, typename h_type, typename H_type>
    void update(
        h_type&& h,
        H_type&& H,
        const mat_t<OBS_DIM>& R,
        const vec_t<OBS_DIM>& observation
    ) noexcept
    {
        static_assert(std::is_invocable_r_v<vec_t<OBS_DIM>, h_type, const vec_t<STATE_DIM>&>);
        static_assert(std::is_invocable_r_v<mat_t<OBS_DIM, STATE_DIM>, H_type, const vec_t<STATE_DIM>&>);
        
        const auto Hj = H(m_state);
        const auto HjT = Hj.transpose();

        const vec_t<OBS_DIM> obs_diff = observation - h(m_state);
        const mat_t<OBS_DIM> obs_cov = Hj.matmul(m_p).matmul(HjT) + R;
        const mat_t<STATE_DIM, OBS_DIM> kalman_gain = m_p.matmul(HjT).matdivr(obs_cov);

        m_state += kalman_gain.matmul(obs_diff);
        m_p -= kalman_gain.matmul(Hj).matmul(m_p);
    }

    /**
     * Get the current state of the kalman filter
     */
    const vec_t<STATE_DIM>& get_state() const noexcept
    {
        return m_state;
    }

private:
    // Current state
    vec_t<STATE_DIM> m_state;
    // Estimated covariance matrix (P)
    mat_t<STATE_DIM> m_p;
};

}
