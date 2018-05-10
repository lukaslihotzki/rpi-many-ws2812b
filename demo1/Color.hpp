#ifndef COLOR_HPP
#define COLOR_HPP

#include <array>

typedef std::array<uint8_t, 3> Color;

static_assert(sizeof(Color) == 3);

Color fromHue(unsigned hue);

#endif
