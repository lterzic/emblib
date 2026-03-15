#include "emblib/io/idev.hpp"
#include "emblib/io/odev.hpp"
#include "catch2/catch_test_macros.hpp"
#include <string>

class buffer_dev : public emblib::io::idev_base<char>, public emblib::io::odev_base<char> {
public:
    emblib::io::result read(etl::span<char> buffer, emblib::io::timeout timeout) noexcept override
    {
        (void)timeout;
        return m_data.copy(buffer.data(), buffer.size());
    }

    emblib::io::result write(etl::span<const char> data, emblib::io::timeout timeout) noexcept override
    {
        (void)timeout;
        m_data.append(data.data(), data.size());
        return data.size();
    }

private:
    std::string m_data;
};

TEST_CASE("IO dev through buffer", "[io][idev][odev]")
{
    constexpr std::string_view sv = "test string";

    buffer_dev dev;
    dev.write(sv, emblib::io::timeout::min());
    
    char buffer[2 * sv.size()];
    auto result = dev.read(buffer, emblib::io::timeout::min());

    REQUIRE(result);
    REQUIRE(*result == sv.size());
    REQUIRE(sv == buffer);
}
