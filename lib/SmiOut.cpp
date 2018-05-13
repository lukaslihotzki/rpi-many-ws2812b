#include "WS2812BOut.hpp"

#include <iostream>
#include <unistd.h>

template <typename T>
static inline T clamp(const T& v, const T& lo, const T& hi)
{
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

SmiOut::SmiOut(int wordCount, int wordSize, int bitCount, int divisor)
	: wordCount(wordCount)
	, wordSize(wordSize)
	, bitCount(bitCount)
	, divisor(divisor)
	, mymem(mbox, mem, model, (sizeof(mymem_t) + wordCount * wordSize + 0xfff) &~ 0xfff, 0x1000)
	, dmachan(dma[5])
{
	for (int i = 0; i < bitCount; i++) {
		originalFunctions[i] = gpio->getfunc(8 + i);
		gpio->setfunc(8 + i, 0b101);
	}

	smi->reset();

	// 6 = PLLD 500 MHz clocksource;  500 MHz / 10 = 50 MHz
	clocks->smi.setup(6, 10);

	unsigned timing = (1 << smi_ctl::pace)
		| (clamp(divisor - 190, 1, 63) << smi_ctl::hold)
		| (clamp(divisor - 128, 1, 63) << smi_ctl::setup)
		| (clamp(divisor - 2, 1, 127) << smi_ctl::strobe);

	switch (wordSize) {
		case 1:
			smi->cs = smi_ctl::cs_pxldat;
			smi->dsw0 = smi_ctl::dsw_swap | (0 << smi_ctl::width) | timing;
			break;
		case 2:
			smi->cs = smi_ctl::cs_pxldat;
			smi->dsw0 = (1 << smi_ctl::width) | timing;
			break;
		case 4:
			smi->cs = 0;
			smi->dsw0 = (2 << smi_ctl::width) | timing;
			break;
	}

	smi->dc = (0x3F << smi_ctl::dc_reqr) | (0x3F << smi_ctl::dc_reqw) |
	          (0x3F << smi_ctl::dc_panicr) | (0x3F << smi_ctl::dc_panicw) |
	          smi_ctl::dc_dmaen;
	smi->addr = 0;
}

SmiOut::~SmiOut()
{
	for (int i = 0; i < bitCount; i++) {
		if (gpio->getfunc(8 + i) == 0b101) {
			gpio->setfunc(8 + i, originalFunctions[i]);
		}
	}
}

uint8_t* SmiOut::data()
{
	return mymem->data;
}

void SmiOut::operator()(unsigned offset, unsigned len)
{
	unsigned cstmp = smi->cs;

	smi->cs = cstmp &~ (smi_ctl::cs_enable | smi_ctl::cs_done);
	while (smi->cs & smi_ctl::cs_enable);

	smi->len = len;

	dmachan.cs = 0;

	mymem->cb.info = dma_cb::wait_resp | dma_cb::dest_dreq | dma_cb::dreq_smi | dma_cb::src_inc;
	mymem->cb.src = mymem.to_physical(mymem->data + wordSize * offset);
	mymem->cb.dst = smi.to_physical(&smi->data);
	mymem->cb.length = wordSize * len;
	mymem->cb.stride = 0;
	mymem->cb.next = 0;

	dmachan.conblk_ad = mymem.to_physical(&mymem->cb);

	cstmp |= smi_ctl::cs_write | smi_ctl::cs_enable;
	smi->cs = cstmp;
	cstmp |= smi_ctl::cs_start;
	smi->cs = cstmp;

	dmachan.cs = dma_chan::cs_active;

#ifdef DBG_WAIT
	usleep(10);
#else
	int sleepTime = len * divisor / 50;
	usleep(sleepTime < 10 ? 10 : sleepTime);
#endif

	while (!(smi->cs & smi_ctl::cs_done) && (dmachan.cs & dma_chan::cs_active)) {
		sched_yield();
	}

	usleep(divisor * 2);

	if (!(smi->cs & smi_ctl::cs_done)) {
		std::cerr << "SMI not finished." << std::endl;
	}
	if (dmachan.cs & dma_chan::cs_active) {
		std::cerr << "DMA not finished." << std::endl;
	}

	smi->cs = cstmp &~ (smi_ctl::cs_enable | smi_ctl::cs_done);

	dmachan.reset();
	usleep(10);
}

void SmiOut::operator()()
{
	operator()(0, wordCount);
}
