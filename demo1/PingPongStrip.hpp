#ifndef PINGPONGSTRIP_HPP
#define PINGPONGSTRIP_HPP

#include "Color.hpp"

class PingPongStrip
{
public:
	PingPongStrip(Color fg, Color bg, Color* data, int size);
	void step();

private:
	Color fg;
	Color bg;
	Color* data;
	int size;
	int osize;
	int pos;
	int direction;
};

#endif
