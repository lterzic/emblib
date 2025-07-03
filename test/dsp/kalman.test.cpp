#include "emblib/dsp/kalman.hpp"
#include "catch2/catch_test_macros.hpp"


TEST_CASE("Kalman linear update", "[dsp][kalman]")
{
    using emblib::dsp::kalman;
    using emblib::math::matrixf;
    using emblib::math::vectorf;
    
    kalman<3> kalman3({1, 1, 1});
    
    matrixf<3> F = {{1, 2, 3}, {-2, -4, 0}, {2, -1, 1}};
    matrixf<3> Q = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // Identity
    vectorf<3> u = {1, 0, -1};
    
    matrixf<4, 3> H = {{1, 3, 7}, {4, 2, -1}, {-1, 2, 0}, {5, 0, -3}};
    vectorf<4> z = {2, -1, 3, 1};
    matrixf<4> R = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; // Identity

    auto f = [&F, &u](const vectorf<3>& state) {
        return F.matmul(state) + u;
    };
    auto Fj = [&F](const vectorf<3>& state) {
        UNUSED(state);
        return F;
    };
    kalman3.predict(f, Fj, Q);

    auto h = [&H](const vectorf<3>& state) {
        return H.matmul(state);
    };
    auto Hj = [&H](const vectorf<3>& state) {
        UNUSED(state);
        return H;
    };
    kalman3.update<4>(h, Hj, R, z);

    vectorf<3> state = kalman3.get_state();
    vectorf<3> expected = {0.348207, -0.381673, 0.407171};

    REQUIRE(state.get_base().isApprox(expected.get_base()));
}