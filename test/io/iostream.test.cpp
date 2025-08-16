#include "emblib/io/iostream.hpp"
#include "catch2/catch_test_macros.hpp"
#include <string>
#include <sstream>

class iostream_buffer : public emblib::io::iostream {
public:
    ssize_t read(char* buffer, size_t size, emblib::io::timeout_t timeout) noexcept override
    {
        (void)timeout;
        return m_data.copy(buffer, size);
    }

    ssize_t write(const char* data, size_t size, emblib::io::timeout_t timeout) noexcept override
    {
        (void)timeout;
        m_data.append(data, size);
        return size;
    }

    bidir_mode_e get_bidir_mode() const noexcept override
    {
        return bidir_mode_e::HALF_DUPLEX;
    }

private:
    std::string m_data;
};

TEST_CASE("IO stream", "[io][iostream]")
{
    #define TEST_STRING ("test string")
    auto timeout = emblib::io::timeout_t(0);
    
    iostream_buffer buffer;
    buffer.write(TEST_STRING, sizeof(TEST_STRING), timeout);
    
    char result[sizeof(TEST_STRING)];
    auto read_res = buffer.read(result, 2 * sizeof(TEST_STRING), timeout);

    REQUIRE(read_res == sizeof(TEST_STRING));
    REQUIRE(strncmp(TEST_STRING, result, sizeof(TEST_STRING)) == 0);
}
