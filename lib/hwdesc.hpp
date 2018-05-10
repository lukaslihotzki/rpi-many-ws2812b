#ifndef HWDESC_HPP
#define HWDESC_HPP

struct dma_cb {
	volatile unsigned info, src, dst, length, stride, next, pad[2];

	enum {
		inten = 1 << 0,
		tdmode_2d = 1 << 1,
		wait_resp = 1 << 3,
		dest_inc = 1 << 4,
		dest_width_128 = 1 << 5,
		dest_dreq = 1 << 6,
		dest_ignore = 1 << 7,
		src_inc = 1 << 8,
		src_width_128 = 1 << 9,
		src_dreq = 1 << 10,
		src_ignore = 1 << 11,
		burst_length_offs = 12,
		permap_offs = 16,
		waits_offs = 21,
		no_wide_bursts = 1 << 26,
	};

	enum {
		dreq_always = 0 << permap_offs,
		dreq_dsi = 1 << permap_offs,
		dreq_pcm_tx = 2 << permap_offs,
		dreq_pcm_rx = 3 << permap_offs,
		dreq_smi = 4 << permap_offs,
		dreq_pwm = 5 << permap_offs,
		dreq_spi_tx = 6 << permap_offs,
		dreq_spi_rx = 7 << permap_offs,
		dreq_bsc_spi_slave_tx = 8 << permap_offs,
		dreq_bsc_spi_slave_rx = 9 << permap_offs,
	};
};

struct dma_chan {
	volatile unsigned cs, conblk_ad, info, src, dst, length, stride, next, debug, pad[55];

	enum {
		cs_active = 0x00000001,
		cs_reset = 0x80000000,
	};

	void reset()
	{
		cs = cs_reset;
	}
};

struct smi_ctl {
	volatile unsigned cs, len, addr, data, dsr0, dsw0, dsr1, dsw1, dsr2, dsw2, dsr3, dsw3, dc, dcs, da, dd, debug;

	enum {
		cs_enable = 1 << 0,
		cs_done = 1 << 1,
		cs_active = 1 << 2,
		cs_start = 1 << 3,
		cs_clearfifo = 1 << 4,
		cs_write = 1 << 5,
		cs_pad0 = 1 << 6,
		cs_pad1 = 1 << 7,
		cs_teen = 1 << 8,
		cs_intd = 1 << 9,
		cs_intt = 1 << 10,
		cs_intr = 1 << 11,
		cs_pvmode = 1 << 12,
		cs_seterr = 1 << 13,
		cs_pxldat = 1 << 14,
		cs_edreq = 1 << 15,
		cs_aferr = 1 << 25,
		cs_txw = 1 << 26,
		cs_rxr = 1 << 27,
		cs_txd = 1 << 28,
		cs_rxd = 1 << 29,
		cs_txe = 1 << 30,
		cs_rxf = 1 << 31,
		addr_addr = 0,
		addr_device = 8,
		dc_reqw = 0,
		dc_reqr = 6,
		dc_panicw = 12,
		dc_panicr = 18,
		dc_dmap = 1 << 24,
		dc_dmaen = 1 << 28,
		strobe = 0,
		pace = 8,
		hold = 16,
		setup = 24,
		width = 30,
		dsw_swap = 1 << 22,
		dsw_format = 1 << 23,
		dcs_enable = 1 << 0,
		dcs_start = 1 << 1,
		dcs_done = 1 << 2,
		dcs_write = 1 << 3,
		da_addr = 0,
		da_device = 8,
		fd_fcnt = 0,
		fd_flvl = 8,
	};

	void reset()
	{
		cs = 0;
		dcs = 0;
	}
};

struct gpio_ctl {
	volatile unsigned fsel[6], _0, set[2], _1, clr[2], _2, lev[2];

	int getfunc(int gpionum)
	{
		int bank = gpionum / 10;
		int offset = (gpionum % 10) * 3;

		return (fsel[bank] >> offset) & 0b111;
	}

	void setfunc(int gpionum, int function)
	{
		int bank = gpionum / 10;
		int offset = (gpionum % 10) * 3;

		fsel[bank] = fsel[bank] & ~(0b111 << offset) | (function << offset);
	}
};

struct clocks_ctl {
	struct clock_ctl {
		volatile unsigned ctl, div;

		enum {
			pwd = 0x5a000000,
			ctl_clocksource_offs = 0,
			ctl_clocksource_mask = 0xf,
			ctl_enab = 1 << 4,
			ctl_kill = 1 << 5,
			ctl_busy = 1 << 7,
			ctl_flip = 1 << 8,
			div_divi_mask = 0xf << 12,
			div_divi_offs = 12,
		};

		void disable()
		{
			ctl = pwd | ctl &~ ctl_enab;
			while (ctl & ctl_busy);
		}

		void setup(int clocksource, int divisor)
		{
			disable();

			div = pwd | (divisor << div_divi_offs) & div_divi_mask;
			ctl = pwd | (clocksource << ctl_clocksource_offs) & ctl_clocksource_mask;
			ctl = pwd | (clocksource << ctl_clocksource_offs) & ctl_clocksource_mask | ctl_enab;
		}
	} _0[19], pwm, pcm, _1[1], smi;
};

#endif
