#include <emblib/rtos/semaphore.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("semaphore successful signal", "[freertos][semaphore]")
{
    emblib::rtos::semaphore semaphore;

    REQUIRE(semaphore.signal());
    REQUIRE(semaphore.wait(emblib::rtos::timeout::min()));
}