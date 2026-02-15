#include "emblib/lockfree/spmc_queue.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Lock-free SPMC queue test", "[lockfree][spmc_queue]")
{
    struct message {
        size_t value;
    };
    
    constexpr size_t TEST_SIZE = 4;
    emblib::lockfree::spmc_queue<message, TEST_SIZE> queue;
    auto reader = queue.get_reader<true>();

    for (size_t i = 0; i < TEST_SIZE; i++) {
        message msg;
        msg.value = i;
        queue.push(msg);
    }

    for (size_t i = 0; i < TEST_SIZE; i++) {
        message msg;
        REQUIRE((reader.read(msg) && msg.value == i));
    }

    {
        message msg;
        REQUIRE_FALSE(reader.read(msg));
    }

    {
        message msg;
        auto reader_copy = reader;
        REQUIRE_FALSE(reader_copy.read(msg));
    }

    for (size_t i = 0; i < TEST_SIZE + 1; i++) {
        message msg;
        msg.value = i;
        queue.push(msg);
    }
    
    REQUIRE(reader.has_overflowed());
}