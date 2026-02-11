#include "emblib/lockfree/spsc_queue.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Lock-free SPSC queue test", "[lockfree][spsc_queue]")
{
    struct message {
        size_t value;
    };
    
    constexpr size_t TEST_SIZE = 4;
    emblib::lockfree::spsc_queue<message, TEST_SIZE> queue;

    for (size_t i = 0; i < TEST_SIZE; i++) {
        message msg;
        msg.value = i;
        REQUIRE(queue.push(msg));
    }
    REQUIRE(!queue.push(message {0}));

    for (size_t i = 0; i < TEST_SIZE; i++) {
        message msg;
        REQUIRE((queue.pop(msg) && msg.value == i));
    }
    
    message tmp;
    REQUIRE(!queue.pop(tmp));

}