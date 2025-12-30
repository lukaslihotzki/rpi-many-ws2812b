#ifndef COLOR_HPP
#define COLOR_HPP

#include <array>
#include <cstdint>

typedef std::array<std::uint8_t, 3> Color;

static_assert(sizeof(Color) == 3);

Color fromHue(unsigned hue);

#endif
