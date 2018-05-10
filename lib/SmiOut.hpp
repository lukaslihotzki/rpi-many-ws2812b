#ifndef SMI_OUT_HPP
#define SMI_OUT_HPP

#include "HwBase.hpp"

class SmiOut : private HwBase
{
public:
	SmiOut(int wordCount, int wordSize, int bitCount);
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
	uint8_t originalFunctions[18];
	UserMemory<mymem_t> mymem;
	struct dma_chan& dmachan;
};

#endif
