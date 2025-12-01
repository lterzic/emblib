#include "emblib/lockfree/allocator.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("RTOS allocator test", "[lockfree][allocator]")
{
    using message_t = char[32];
    constexpr size_t TEST_SIZE = 4;

    emblib::lockfree::allocator<message_t, TEST_SIZE> allocator;

    message_t* last_ptr;
    for (size_t i = 0; i < TEST_SIZE; i++) {
        last_ptr = allocator.alloc();
        REQUIRE(last_ptr != nullptr);
    }
    REQUIRE(allocator.alloc() == nullptr);

    allocator.dealloc(last_ptr);
    REQUIRE(allocator.alloc() == last_ptr);
}