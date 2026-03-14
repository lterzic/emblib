#include <emblib/posix/udp_dev.hpp>

#include <catch2/catch_test_macros.hpp>


TEST_CASE("udp_dev write and read", "[posix][udp]")
{
    emblib::posix::udp_dev dev1{"localhost", 25565, 25566};
    emblib::posix::udp_dev dev2{"localhost", 25566, 25565};

    std::array<uint8_t, 8> test_data = {1, 2, 3, 4, 5, 6, 7, 8};
    
    auto write_res = dev1.write(test_data, emblib::io::timeout{1});
    REQUIRE(write_res);
    REQUIRE(write_res == sizeof(test_data));

    std::array<uint8_t, 10> read_buf;
    auto read_res = dev2.read(read_buf, emblib::io::timeout{1});
    REQUIRE(read_res);
    REQUIRE(read_res == sizeof(test_data));

    REQUIRE(std::equal(test_data.begin(), test_data.end(), read_buf.begin()));
}

TEST_CASE("udp_dev async read", "[posix][udp]")
{
    emblib::posix::udp_dev dev1{"localhost", 25565, 25566};
    emblib::posix::udp_dev dev2{"localhost", 25566, 25565};

    emblib::io::result read_res{etl::unexpected{emblib::io::error::TIMEOUT}};
    auto read_cb = [&read_res](emblib::io::result res) {
        read_res = res;
    };

    std::array<uint8_t, 10> read_buf;
    REQUIRE(dev2.read_async(read_buf, read_cb));
    REQUIRE_FALSE(read_res.has_value());

    std::array<uint8_t, 8> test_data = {2, 3, 4, 5, 1, 6, 7, 8};
    REQUIRE(dev1.write(test_data, emblib::io::timeout{1}));
    
    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    REQUIRE(read_res);
    REQUIRE(read_res == sizeof(test_data));
}

TEST_CASE("udp_dev abort async read", "[posix][udp]")
{
    emblib::posix::udp_dev dev1{"localhost", 25565, 25566};
    emblib::posix::udp_dev dev2{"localhost", 25566, 25565};

    emblib::io::result read_res{etl::unexpected{emblib::io::error::TIMEOUT}};
    auto read_cb = [&read_res](emblib::io::result res) {
        read_res = res;
    };

    std::array<uint8_t, 10> read_buf;
    REQUIRE(dev2.read_async(read_buf, read_cb));
    REQUIRE_FALSE(read_res.has_value());

    REQUIRE(dev2.abort_async_read());
    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    REQUIRE(read_res.error() == emblib::io::error::ABORT);
}