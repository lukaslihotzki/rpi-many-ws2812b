#ifndef SMI_OUT_HPP
#define SMI_OUT_HPP

#include "HwBase.hpp"

class SmiOut : private HwBase
{
public:
	// wordCount: number of words
	// wordSize: sizeof word (in bytes): 1, 2 or 4
	// bitCount: number of (least significant) bits in words actually used
	// divisor: divisor of 50 MHz clock: 3 to 253
	SmiOut(int wordCount, int wordSize, int bitCount, int divisor);

	~SmiOut();
	uint8_t* data();
	void operator()(unsigned offset, unsigned len);
	void operator()();

private:
	struct mymem_t {
		dma_cb cb;
		uint8_t data[];
	};

	int wordCount;
	int wordSize;
	int bitCount;
	int divisor;
	uint8_t originalFunctions[18];
	UserMemory<mymem_t> mymem;
	struct dma_chan& dmachan;
};

#endif
