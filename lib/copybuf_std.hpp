#ifndef COPYBUF_STD_HPP
#define COPYBUF_STD_HPP

template <int G = 1, int R = 0, int B = 2, int W = 3, typename T>
void copybuf(int chainlen, int chaincount, const uint8_t* const datain[], T* x)
{
	for (int i = 0; i < chainlen; i++) {
		for (int c : {G, R, B}) {
			for (int b = 7; b >= 0; b--) {
				T pbits = 0;
				for (int j = 0; j < chaincount; j++) {
					pbits |= ((datain[j][W * i + c] & (1 << b)) >> b) << j;
				}
				*(x++) = -1;
				*(x++) = pbits;
				*(x++) = 0;
			}
		}
	}
}

#endif
