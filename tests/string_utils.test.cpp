#include <catch2/catch.hpp>
#include <string_utils.hpp>

using namespace strutil;

TEST_CASE("Upper case a string", "[upper]")
{
    std::string str{ "hello world!" };
    REQUIRE(upper(str) == "HELLO WORLD!");
}

TEST_CASE("Lower case a string", "[upper]")
{
    std::string str{ "HelLO wOrLd!" };
    REQUIRE(lower(str) == "hello world!");
}

TEST_CASE("capitalize a string", "[upper]")
{
    std::string str{ "hello world!" };
    REQUIRE(capitalize(str) == "Hello world!");
    SECTION("capitalizing a string with non printable character yield the same "
            "string")
    {
        std::string str{ " hello world!" };
        REQUIRE(capitalize(str) == " hello world!");
    }
}

TEST_CASE("Strip substring from a string", "[strip]")
{
    std::string s{ "+,$hello world+,$" };

    SECTION("stripping from the left")
    {
        REQUIRE(lstrip(s, "+,$") == "hello world+,$");
    }

    SECTION("stripping from the right")
    {
        REQUIRE(rstrip(s, "+,$") == "+,$hello world");
    }

    SECTION("stripping from both ends")
    {
        REQUIRE(strip(s, "+,$") == "hello world");
    }

    SECTION("stripping a bigger string yield the same string")
    {
        REQUIRE(lstrip(s, "the brown fox jumped over the lazy dof") == s);
    }

    SECTION("stripping a string not contained in the subject string")
    {
        REQUIRE(lstrip(s, "buzzy") == s);
        REQUIRE(rstrip(s, "buzzy") == s);
        REQUIRE(strip(s, "buzzy") == s);
    }
}

TEST_CASE("Stripping any characters for the beginning of a string", "[lstrip]")
{
    std::string s{ "\r\t\n hello world" };
    REQUIRE(lstrip_any_of(s) == "hello world");
}

TEST_CASE("Stripping white characters for the end of a string", "[rstrip]")
{
    std::string s{ "hello world\r\n\t " };
    REQUIRE(rstrip_any_of(s) == "hello world");
}

TEST_CASE("Stripping white characters for both ends of a string", "[strip]")
{
    std::string s{ "\t\n\r   \r\t\t hello world\r\n\t    \r\n" };
    REQUIRE(strip_any_of(s) == "hello world");
}

TEST_CASE("Replace all occurence of substring in a string with other",
          "[replace]")
{
    std::string str{ "One mississippi, Two mississippi, Three mississippi" };
    replace(str, "mississippi", "colorado");
    REQUIRE(str == "One colorado, Two colorado, Three colorado");
}

TEST_CASE("splitting a string", "[split]")
{

    std::string str{ "the brown fox jumped over the lazy dog" };
    split s(str, " ");
    std::vector<std::string> v(s.begin(), s.end());
    REQUIRE(v == std::vector<std::string>{ "the",
                                           "brown",
                                           "fox",
                                           "jumped",
                                           "over",
                                           "the",
                                           "lazy",
                                           "dog" });

    SECTION("splitting a string with using delimiter not contained in that "
            "string yield the same string")
    {
        std::string str{ "hello" };
        split s(str, " ");
        std::vector<std::string> v(s.begin(), s.end());
        REQUIRE(v == std::vector<std::string>{ "hello" });
    }

    SECTION("splitting a string with a string delimiter")
    {
        std::string str{
            "the+^$brown+^$fox+^$jumped+^$over+^$the+^$lazy+^$dog"
        };
        split sp(str, "+^$");
        std::vector<std::string> v(sp.cbegin(), sp.cend());
        REQUIRE(v == std::vector<std::string>{ "the",
                                               "brown",
                                               "fox",
                                               "jumped",
                                               "over",
                                               "the",
                                               "lazy",
                                               "dog" });
    }

    SECTION("splitting a C string")
    {
        const char str[]{
            "the+++brown+++fox+++jumped+++over+++the+++lazy+++dog"
        };
        split sp(str, "+++");
        std::vector<std::string_view> v(sp.begin(), sp.end());
        REQUIRE(v == std::vector<std::string_view>{ "the",
                                                    "brown",
                                                    "fox",
                                                    "jumped",
                                                    "over",
                                                    "the",
                                                    "lazy",
                                                    "dog" });
    }

    SECTION("splitting a string using a delimiter that is not contained in "
            "that string")
    {
        split s("hello world", "+++");
        std::vector<std::string_view> v(s.begin(), s.end());
        REQUIRE(v == std::vector<std::string_view>{ "hello world" });
    }

    SECTION("Splitting an empty string yield an empty string")
    {
        std::string str{ "" };
        split sp{ str, " " };
        std::vector<std::string_view> v(sp.begin(), sp.end());
        REQUIRE(v == std::vector<std::string_view>{ "" });
    }

    SECTION("Splitting a string wich begins with the delimiter string")
    {
        std::string str("+++hello+++world+++");
        split sp(str, "+++");
        std::vector<std::string_view> v(sp.begin(), sp.end());
        REQUIRE(v == std::vector<std::string_view>{ "", "hello", "world", "" });
    }

    SECTION("Splitting a string wich is equal to the delimiter yield two empty "
            "strings")
    {
        split s(" ", " ");
        std::vector<std::string_view> v(s.begin(), s.end());
        REQUIRE(v == std::vector<std::string_view>{ "", "" });
    }
}

