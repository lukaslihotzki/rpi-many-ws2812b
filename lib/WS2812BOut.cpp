#include "WS2812BOut.hpp"

#ifdef __ARM_NEON
#include "copybuf_neon.hpp"
#else
#include "copybuf_std.hpp"
#endif

#include <stdexcept>

static inline int getWordSize(int chainCount)
{
	if (chainCount <= 0) {
		throw std::range_error("chainCount must be positive");
	} else if (chainCount <= 8) {
		return 1;
	} else if (chainCount <= 16) {
		return 2;
	} else {
		return 4;
	}
}

WS2812BOut::WS2812BOut(int chainLength, int chainCount)
	: chainLength(chainLength)
	, chainCount(chainCount)
	, wordSize(getWordSize(chainCount))
	, smiout(chainLength * 24 * 3, wordSize, chainCount, 20) // 50 MHz / 20 = 2.5 MHz (400 ns cycle length)
{
}

void WS2812BOut::operator()(const uint8_t* const datain[])
{
	switch (wordSize) {
		case 1:
			copybuf(chainLength, chainCount, datain, smiout.data());
			break;
		case 2:
			copybuf(chainLength, chainCount, datain, reinterpret_cast<uint16_t*>(smiout.data()));
			break;
		case 4:
			copybuf(chainLength, chainCount, datain, reinterpret_cast<uint32_t*>(smiout.data()));
			break;
	}
	smiout();
}
