#ifndef COPYBUF_NEON_HPP
#define COPYBUF_NEON_HPP

#include <arm_neon.h>
#include <array>
#include <algorithm>

#define BIT_TRANSPOSE(s) vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 7), mask), vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 6), mask), vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 5), mask), vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 4), mask), vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 3), mask), vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 2), mask), vsri_n_u8( \
	vtst_u8(vdup_lane_u8(s, 1), mask), \
	vtst_u8(vdup_lane_u8(s, 0), mask), \
			1), 1), 1), 1), 1), 1), 1)

#define BIT_TRANSPOSE_2LSB(s) vshr_n_u8(vsri_n_u8(vtst_u8(vdup_lane_u8(s, 1), mask), vtst_u8(vdup_lane_u8(s, 0), mask), 1), 6)

template <int G = 1, int R = 0, int B = 2, int W = 3>
void copybuf(int chainlen, int chaincount, const uint8_t* const datain[8], uint8_t* buf)
{
	std::array<const uint8_t*, 8> data;
	std::copy_n(datain, chaincount, data.data());
	std::fill_n(data.data() + chaincount, data.size() - chaincount, datain[0]);

	for (int pxl = 0; pxl < chainlen; pxl++) {
		uint8x8x3_t val;
		val = vld3_lane_u8(data[0], val, 0);
		val = vld3_lane_u8(data[1], val, 1);
		val = vld3_lane_u8(data[2], val, 2);
		val = vld3_lane_u8(data[3], val, 3);
		val = vld3_lane_u8(data[4], val, 4);
		val = vld3_lane_u8(data[5], val, 5);
		val = vld3_lane_u8(data[6], val, 6);
		val = vld3_lane_u8(data[7], val, 7);

		for (int i = 0; i < 8; i++) {
			data[i] += W;
		}

		uint8x8_t mask{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

		uint8x8x3_t pout0 = { vdup_n_u8(0xff), BIT_TRANSPOSE(val.val[G]), vdup_n_u8(0x00) };
		uint8x8x3_t pout1 = { vdup_n_u8(0xff), BIT_TRANSPOSE(val.val[R]), vdup_n_u8(0x00) };
		uint8x8x3_t pout2 = { vdup_n_u8(0xff), BIT_TRANSPOSE(val.val[B]), vdup_n_u8(0x00) };

		vst3_u8(buf, pout0);
		buf += 8 * 3;
		vst3_u8(buf, pout1);
		buf += 8 * 3;
		vst3_u8(buf, pout2);
		buf += 8 * 3;
	}
}

template <int G = 1, int R = 0, int B = 2, int W = 3>
void copybuf(int chainlen, int chaincount, const uint8_t* const datain[16], uint16_t* buf)
{
	std::array<const uint8_t*, 16> data;
	std::copy_n(datain, chaincount, data.data());
	std::fill_n(data.data() + chaincount, data.size() - chaincount, datain[0]);

	for (int pxl = 0; pxl < chainlen; pxl++) {
		uint8x8x3_t val1, val2;
		val1 = vld3_lane_u8(data[0], val1, 0);
		val1 = vld3_lane_u8(data[1], val1, 1);
		val1 = vld3_lane_u8(data[2], val1, 2);
		val1 = vld3_lane_u8(data[3], val1, 3);
		val1 = vld3_lane_u8(data[4], val1, 4);
		val1 = vld3_lane_u8(data[5], val1, 5);
		val1 = vld3_lane_u8(data[6], val1, 6);
		val1 = vld3_lane_u8(data[7], val1, 7);
		val2 = vld3_lane_u8(data[8], val2, 0);
		val2 = vld3_lane_u8(data[9], val2, 1);
		val2 = vld3_lane_u8(data[10], val2, 2);
		val2 = vld3_lane_u8(data[11], val2, 3);
		val2 = vld3_lane_u8(data[12], val2, 4);
		val2 = vld3_lane_u8(data[13], val2, 5);
		val2 = vld3_lane_u8(data[14], val2, 6);
		val2 = vld3_lane_u8(data[15], val2, 7);

		for (int i = 0; i < 16; i++) {
			data[i] += W;
		}

		uint8x8_t mask{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

		uint8x8x2_t t1 = vzip_u8(BIT_TRANSPOSE(val1.val[G]), BIT_TRANSPOSE(val2.val[G]));
		uint8x8x2_t t2 = vzip_u8(BIT_TRANSPOSE(val1.val[R]), BIT_TRANSPOSE(val2.val[R]));
		uint8x8x2_t t3 = vzip_u8(BIT_TRANSPOSE(val1.val[B]), BIT_TRANSPOSE(val2.val[B]));

		uint16x8_t c1 = vcombine_u16(vreinterpret_u16_u8(t1.val[0]), vreinterpret_u16_u8(t1.val[1]));
		uint16x8_t c2 = vcombine_u16(vreinterpret_u16_u8(t2.val[0]), vreinterpret_u16_u8(t2.val[1]));
		uint16x8_t c3 = vcombine_u16(vreinterpret_u16_u8(t3.val[0]), vreinterpret_u16_u8(t3.val[1]));

		uint16x8x3_t pout0 = { vdupq_n_u16(0xffff), c1, vdupq_n_u16(0x0000) };
		uint16x8x3_t pout1 = { vdupq_n_u16(0xffff), c2, vdupq_n_u16(0x0000) };
		uint16x8x3_t pout2 = { vdupq_n_u16(0xffff), c3, vdupq_n_u16(0x0000) };

		vst3q_u16(buf, pout0);
		buf += 8 * 3;
		vst3q_u16(buf, pout1);
		buf += 8 * 3;
		vst3q_u16(buf, pout2);
		buf += 8 * 3;
	}
}

template <int G = 1, int R = 0, int B = 2, int W = 3>
void copybuf(int chainlen, int chaincount, const uint8_t* const datain[18], uint32_t* buf)
{
	std::array<const uint8_t*, 18> data;
	std::copy_n(datain, chaincount, data.data());
	std::fill_n(data.data() + chaincount, data.size() - chaincount, datain[0]);

	for (int pxl = 0; pxl < chainlen; pxl++) {
		uint8x8x3_t val1, val2, val3;
		val1 = vld3_lane_u8(data[0], val1, 0);
		val1 = vld3_lane_u8(data[1], val1, 1);
		val1 = vld3_lane_u8(data[2], val1, 2);
		val1 = vld3_lane_u8(data[3], val1, 3);
		val1 = vld3_lane_u8(data[4], val1, 4);
		val1 = vld3_lane_u8(data[5], val1, 5);
		val1 = vld3_lane_u8(data[6], val1, 6);
		val1 = vld3_lane_u8(data[7], val1, 7);
		val2 = vld3_lane_u8(data[8], val2, 0);
		val2 = vld3_lane_u8(data[9], val2, 1);
		val2 = vld3_lane_u8(data[10], val2, 2);
		val2 = vld3_lane_u8(data[11], val2, 3);
		val2 = vld3_lane_u8(data[12], val2, 4);
		val2 = vld3_lane_u8(data[13], val2, 5);
		val2 = vld3_lane_u8(data[14], val2, 6);
		val2 = vld3_lane_u8(data[15], val2, 7);
		val3 = vld3_lane_u8(data[16], val3, 0);
		val3 = vld3_lane_u8(data[17], val3, 1);

		for (int i = 0; i < 18; i++) {
			data[i] += W;
		}

		uint8x8_t mask{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

		uint8x8x2_t t1 = vzip_u8(BIT_TRANSPOSE(val1.val[G]), BIT_TRANSPOSE(val2.val[G]));
		uint8x8x2_t t2 = vzip_u8(BIT_TRANSPOSE(val1.val[R]), BIT_TRANSPOSE(val2.val[R]));
		uint8x8x2_t t3 = vzip_u8(BIT_TRANSPOSE(val1.val[B]), BIT_TRANSPOSE(val2.val[B]));

		uint16x8_t c1 = vcombine_u16(vreinterpret_u16_u8(t1.val[0]), vreinterpret_u16_u8(t1.val[1]));
		uint16x8_t c2 = vcombine_u16(vreinterpret_u16_u8(t2.val[0]), vreinterpret_u16_u8(t2.val[1]));
		uint16x8_t c3 = vcombine_u16(vreinterpret_u16_u8(t3.val[0]), vreinterpret_u16_u8(t3.val[1]));

		uint8x8x2_t t4 = vzip_u8(BIT_TRANSPOSE_2LSB(val3.val[G]), vdup_n_u8(0x00));
		uint8x8x2_t t5 = vzip_u8(BIT_TRANSPOSE_2LSB(val3.val[R]), vdup_n_u8(0x00));
		uint8x8x2_t t6 = vzip_u8(BIT_TRANSPOSE_2LSB(val3.val[B]), vdup_n_u8(0x00));

		uint16x8_t c4 = vcombine_u16(vreinterpret_u16_u8(t4.val[0]), vreinterpret_u16_u8(t4.val[1]));
		uint16x8_t c5 = vcombine_u16(vreinterpret_u16_u8(t5.val[0]), vreinterpret_u16_u8(t5.val[1]));
		uint16x8_t c6 = vcombine_u16(vreinterpret_u16_u8(t6.val[0]), vreinterpret_u16_u8(t6.val[1]));

		uint16x8x2_t t7 = vzipq_u16(c1, c4);
		uint16x8x2_t t8 = vzipq_u16(c2, c5);
		uint16x8x2_t t9 = vzipq_u16(c3, c6);

		uint32x4x3_t pout0 = { vdupq_n_u32(0xffffffff), vreinterpretq_u32_u16(t7.val[0]), vdupq_n_u32(0x00000000) };
		uint32x4x3_t pout1 = { vdupq_n_u32(0xffffffff), vreinterpretq_u32_u16(t7.val[1]), vdupq_n_u32(0x00000000) };
		uint32x4x3_t pout2 = { vdupq_n_u32(0xffffffff), vreinterpretq_u32_u16(t8.val[0]), vdupq_n_u32(0x00000000) };
		uint32x4x3_t pout3 = { vdupq_n_u32(0xffffffff), vreinterpretq_u32_u16(t8.val[1]), vdupq_n_u32(0x00000000) };
		uint32x4x3_t pout4 = { vdupq_n_u32(0xffffffff), vreinterpretq_u32_u16(t9.val[0]), vdupq_n_u32(0x00000000) };
		uint32x4x3_t pout5 = { vdupq_n_u32(0xffffffff), vreinterpretq_u32_u16(t9.val[1]), vdupq_n_u32(0x00000000) };

		vst3q_u32(buf, pout0);
		buf += 4 * 3;
		vst3q_u32(buf, pout1);
		buf += 4 * 3;
		vst3q_u32(buf, pout2);
		buf += 4 * 3;
		vst3q_u32(buf, pout3);
		buf += 4 * 3;
		vst3q_u32(buf, pout4);
		buf += 4 * 3;
		vst3q_u32(buf, pout5);
		buf += 4 * 3;
	}
}

#endif
