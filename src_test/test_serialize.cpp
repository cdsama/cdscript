#include "catch2_ext.hpp"
#include "serialize.hpp"
#include <sstream>

using namespace cd::serialize;

class Color
{
  public:
    int8_t r = 0;
    int8_t g = 0;

    Color(int8_t _r, int8_t _g, int8_t _b)
        : r(_r), g(_g), b(_b)
    {
    }

    int8_t get_b()
    {
        return b;
    }

  private:
    int8_t b = 0;

  public:
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, Color &c)
    {
        ar << c.r << c.g << c.b;
        if constexpr (Archive::Loading)
        {
            std::cout << "loading" << std::endl;
        }
        if constexpr (Archive::Saving)
        {
            std::cout << "saving" << std::endl;
        }
        return ar;
    }
};

TEST_CASE("Serialize-Basic", "[core][serialize]")
{
    Color c = {1, 2, 3};
    std::ostringstream os;
    Archive<Writer> ar(os);
    ar << c;
    Color d = {0, 0, 0};
    REQUIRE(os.str().size() == 3);
    std::istringstream is(os.str());
    Archive<Reader> ar2(is);
    ar2 << d;
    CHECK(c.r == d.r);
    CHECK(c.g == d.g);
    CHECK(c.get_b() == d.get_b());
}
