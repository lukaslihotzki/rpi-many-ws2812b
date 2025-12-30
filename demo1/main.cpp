#include "WS2812BOut.hpp"
#include "Color.hpp"
#include "PingPongStrip.hpp"
#include <signal.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

int main(int argc, char** argv)
{
	static volatile bool running = true;

	struct sigaction sa{};
	sa.sa_handler = [] (int) {
		running = false;
	};

	sigaction(SIGTERM, &sa, nullptr);
	sigaction(SIGINT, &sa, nullptr);

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " STRIP1_LENGTH [ STRIP2_LENGTH ... ]" << std::endl;
		return 1;
	}

	std::vector<int> chainLengths;

	for (int i = 1; i < argc; i++) {
		chainLengths.push_back(std::atoi(argv[i]));
	}

	int maxLength = *std::max_element(chainLengths.begin(), chainLengths.end());
	std::vector<Color> data(std::accumulate(chainLengths.begin(), chainLengths.end(), maxLength - chainLengths.back()));
	std::vector<const uint8_t*> row;
	std::vector<PingPongStrip> pps;

	Color* ptr = &data.front();
	for (int i = 0; i < chainLengths.size(); i++) {
		row.push_back(reinterpret_cast<const uint8_t*>(ptr));
		pps.emplace_back(fromHue(i * 360 / chainLengths.size()), std::array<uint8_t,3>{{0,0,0}}, ptr, chainLengths[i]);
		ptr += chainLengths[i];
	}

	WS2812BOut out(maxLength, chainLengths.size());

	while (running) {
		std::for_each(pps.begin(), pps.end(), std::mem_fn(&PingPongStrip::step));
		out(row.data());
	}
}
