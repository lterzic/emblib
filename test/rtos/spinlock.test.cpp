#include "emblib/rtos/spinlock.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Spinlock", "[rtos][spinlock]")
{
    emblib::rtos::spinlock s;

    REQUIRE(s.try_lock());
    REQUIRE_FALSE(s.try_lock());
    
    s.unlock();
    REQUIRE(s.try_lock());
}

TEST_CASE("Spinlock scoped lock", "[rtos][spinlock]")
{
    emblib::rtos::spinlock s1, s2;

    {
        emblib::rtos::scoped_lock lock(s1, s2);

        REQUIRE_FALSE(s1.try_lock());
        REQUIRE_FALSE(s2.try_lock());
    }

    REQUIRE(s1.try_lock());
    REQUIRE(s2.try_lock());
}