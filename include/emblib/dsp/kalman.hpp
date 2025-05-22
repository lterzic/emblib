#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"
#include <etl/delegate.h>

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
     * Kalman filter update step for a non-linear (general) model
     * @param f State transition - Calculate the expected next state based on the current state
     * @param F State jacobian - Derivative of `f` with respect to the state
     * @param h Calculate the expected observation from a given state
     * @param H State to observation jacobian - Derivative of `h` with respect to state
     * @param Q State transition noise covariance matrix
     * @param R Observation (measurement) noise covariance matrix
     * @param observation Measurement vector
     * @note https://en.wikipedia.org/wiki/Extended_Kalman_filter
     * @todo F and H could return matrix references
     */
    template <size_t OBS_DIM>
    void update(
        etl::delegate<vec_t<STATE_DIM> (const vec_t<STATE_DIM>&)> f,
        etl::delegate<mat_t<STATE_DIM> (const vec_t<STATE_DIM>&)> F,
        etl::delegate<vec_t<OBS_DIM> (const vec_t<STATE_DIM>&)> h,
        etl::delegate<mat_t<OBS_DIM, STATE_DIM> (const vec_t<STATE_DIM>&)> H,
        const mat_t<STATE_DIM>& Q,
        const mat_t<OBS_DIM>& R,
        const vec_t<OBS_DIM>& observation
    ) noexcept;

    /**
     * Update the kalman filter state assuming a linear model
     * @param OBS_DIM Dimension of the observation vector
     * @param F State transition matrix
     * @param u External input to the state
     * @param H State to expected observation mapping matrix
     * @param Q Transition process noise covariance matrix
     * @param R Observation noise covariance matrix
     * @param z Observation (measurement) vector
     * @note https://en.wikipedia.org/wiki/Kalman_filter
     */
    template <size_t OBS_DIM>
    void update(
        const mat_t<STATE_DIM>& F,
        const vec_t<STATE_DIM>& u,
        const mat_t<OBS_DIM, STATE_DIM>& H,
        const mat_t<STATE_DIM>& Q,
        const mat_t<OBS_DIM>& R,
        const vec_t<OBS_DIM>& z
    ) noexcept;

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


template <size_t STATE_DIM, typename scalar_type>
template <size_t OBS_DIM>
inline void kalman<STATE_DIM, scalar_type>::update(
    etl::delegate<vec_t<STATE_DIM>(const vec_t<STATE_DIM> &)> f,
    etl::delegate<mat_t<STATE_DIM>(const vec_t<STATE_DIM> &)> F,
    etl::delegate<vec_t<OBS_DIM>(const vec_t<STATE_DIM> &)> h,
    etl::delegate<mat_t<OBS_DIM, STATE_DIM>(const vec_t<STATE_DIM> &)> H,
    const mat_t<STATE_DIM> &Q,
    const mat_t<OBS_DIM> &R,
    const vec_t<OBS_DIM> &observation
) noexcept
{
    const auto state_predict = f(m_state);
    const auto Fj = F(m_state); // State jacobian
    const mat_t<STATE_DIM> cov_predict = Fj.matmul(m_p).matmul(Fj.transpose()) + Q;

    const auto Hj = H(state_predict); // State to obs jacobian
    const auto HjT = Hj.transpose();
    const vec_t<OBS_DIM> obs_diff = observation - h(state_predict);
    const mat_t<OBS_DIM> obs_cov = Hj.matmul(cov_predict).matmul(HjT) + R;

    const mat_t<STATE_DIM, OBS_DIM> kalman_gain = cov_predict.matmul(HjT).matdivr(obs_cov);

    m_state = state_predict + kalman_gain.matmul(obs_diff);
    m_p = cov_predict - kalman_gain.matmul(Hj).matmul(cov_predict);
}

template <size_t STATE_DIM, typename scalar_type>
template <size_t OBS_DIM>
inline void kalman<STATE_DIM, scalar_type>::update(
    const mat_t<STATE_DIM> &F,
    const vec_t<STATE_DIM> &u,
    const mat_t<OBS_DIM, STATE_DIM>& H,
    const mat_t<STATE_DIM> &Q,
    const mat_t<OBS_DIM> &R,
    const vec_t<OBS_DIM> &z
) noexcept
{
    auto f = [&F, &u](const vec_t<STATE_DIM>& state) {
        return F.matmul(state) + u;
    };

    auto Fj = [&F](const vec_t<STATE_DIM>& state) {
        UNUSED(state);
        return F;
    };

    auto h = [&H](const vec_t<STATE_DIM>& state) {
        return H.matmul(state);
    };

    auto Hj = [&H](const vec_t<STATE_DIM>& state) {
        UNUSED(state);
        return H;
    };

    return update<OBS_DIM>(f, Fj, h, Hj, Q, R, z);
}

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace dsp;
}
#endif