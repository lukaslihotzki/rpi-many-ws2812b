#include "HwBase.hpp"

HwBase::HwBase()
	: model(mbox)
	, dma(mem, model, 0x007000, 14 * sizeof(dma_chan))
	, clocks(mem, model, 0x101000)
	, gpio(mem, model, 0x200000)
	, smi(mem, model, 0x600000)
{
}
