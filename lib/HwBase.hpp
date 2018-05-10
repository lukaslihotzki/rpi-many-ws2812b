#ifndef HW_BASE_HPP
#define HW_BASE_HPP

#include "Mailbox.hpp"
#include "Model.hpp"
#include "MemoryFile.hpp"
#include "Peripheral.hpp"
#include "UserMemory.hpp"
#include "hwdesc.hpp"

class HwBase
{
public:
	HwBase();

	Mailbox mbox;
	Model model;
	MemoryFile mem;
	Peripheral<dma_chan> dma;
	Peripheral<clocks_ctl> clocks;
	Peripheral<gpio_ctl> gpio;
	Peripheral<smi_ctl> smi;
};

#endif
