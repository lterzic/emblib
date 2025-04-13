#include <iostream>
#include <emblib/utils/chrono.hpp>

int main()
{
    constexpr auto a = emblib::MILLISECONDS_MAX;

    if (a.value() == 3) {

    }
    return 0;
}