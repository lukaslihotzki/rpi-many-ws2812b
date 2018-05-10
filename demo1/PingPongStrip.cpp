#include "PingPongStrip.hpp"

PingPongStrip::PingPongStrip(Color fg, Color bg, Color* data, int size)
	: fg(fg)
	, bg(bg)
	, data(data)
	, size(size)
	, osize(size / 8)
	, pos(osize)
	, direction(1)
{
	std::fill(data, data + osize, fg);
	std::fill(data + osize, data + size, bg);
}

void PingPongStrip::step()
{
	data[pos] = fg;
	data[pos - direction * osize] = bg;

	if (pos == size - 1) {
		direction = -1;
		pos = size - osize;
	}

	if (pos == 0) {
		direction = 1;
		pos = osize - 1;
	}

	pos += direction;
}
