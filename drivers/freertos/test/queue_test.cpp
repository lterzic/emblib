#include <emblib/rtos/queue.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("queue send receive", "[freertos][queue]")
{
    emblib::rtos::queue<int, 2> queue;

    REQUIRE(queue.send(2, emblib::rtos::timeout::min()));
    REQUIRE(queue.send(3, emblib::rtos::timeout::min()));

    int data;
    REQUIRE(queue.receive(data, emblib::rtos::timeout::min()));
    REQUIRE(data == 2);
    REQUIRE(queue.receive(data, emblib::rtos::timeout::min()));
    REQUIRE(data == 3);
}