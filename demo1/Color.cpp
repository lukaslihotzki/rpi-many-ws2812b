#include "Color.hpp"

Color fromHue(unsigned hue)
{
	uint8_t i = hue / 60;
	uint8_t f1 = (255 * hue) / 60 - 255 * i;
	uint8_t f2 = 255 - f1;

	switch (i) {
		case 0: return {255, f1, 0};
		case 1: return {f2, 255, 0};
		case 2: return {0, 255, f1};
		case 3: return {0, f2, 255};
		case 4: return {f1, 0, 255};
		case 5: return {255, 0, f2};
		default: return {0, 0, 0};
	}
}
