#pragma once

namespace emblib::math {

template <typename L, typename R = L>
struct bin_op_add {
    auto operator()(const L& lhs, const R& rhs) const noexcept { return lhs + rhs; }
};

template <typename L, typename R = L>
struct bin_op_sub {
    auto operator()(const L& lhs, const R& rhs) const noexcept { return lhs - rhs; }
};

template <typename L, typename R = L>
struct bin_op_mul {
    auto operator()(const L& lhs, const R& rhs) const noexcept { return lhs * rhs; }
};

template <typename L, typename R = L>
struct bin_op_div {
    auto operator()(const L& lhs, const R& rhs) const noexcept { return lhs / rhs; }
};

template <typename L, typename R = L>
struct bin_op_eq {
    bool operator()(const L& lhs, const R& rhs) const noexcept { return lhs == rhs; }
};

template <typename L, typename R = L>
struct bin_op_neq {
    bool operator()(const L& lhs, const R& rhs) const noexcept { return lhs != rhs; }
};

template <typename L, typename R = L>
struct bin_op_lt {
    bool operator()(const L& lhs, const R& rhs) const noexcept { return lhs < rhs; }
};

template <typename L, typename R = L>
struct bin_op_gt {
    bool operator()(const L& lhs, const R& rhs) const noexcept { return lhs > rhs; }
};

template <typename L, typename R = L>
struct bin_op_lte {
    bool operator()(const L& lhs, const R& rhs) const noexcept { return lhs <= rhs; }
};

template <typename L, typename R = L>
struct bin_op_gte {
    bool operator()(const L& lhs, const R& rhs) const noexcept { return lhs >= rhs; }
};

}