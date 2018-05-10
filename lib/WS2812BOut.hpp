#ifndef WS2812B_OUT_HPP
#define WS2812B_OUT_HPP

#include "SmiOut.hpp"

class WS2812BOut
{
	public:
		WS2812BOut(int chainLength, int chainCount);
		void operator()(const uint8_t* const datain[]);

	private:
		int chainLength;
		int chainCount;
		int wordSize;
		SmiOut smiout;
};

#endif
