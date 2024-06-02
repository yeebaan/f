#include <bitset>
#include "Testbench.h"

Testbench::Testbench(sc_core::sc_module_name n) : sc_core::sc_module(n) {
	SC_THREAD(feed_rgb);
	sensitive << i_clk.pos();
	dont_initialize();
	SC_THREAD(fetch_result);
	sensitive << i_clk.pos();
	dont_initialize();
}

Testbench::~Testbench() {
}

void Testbench::feed_rgb() {
	o_rgb.reset();
	o_rst.write(false);
	wait();
	o_rst.write(true);
	constexpr auto n{3};
	total_start_time = sc_time_stamp();
	{
		rgb_t t{};
		t.range(n * (0 + 1) - 1, n * 0) = 0;
		t.range(n * (1 + 1) - 1, n * 1) = 1;
		t.range(n * (2 + 1) - 1, n * 2) = 2;
		t.range(n * (3 + 1) - 1, n * 3) = 0;
		t.range(n * (4 + 1) - 1, n * 4) = 1;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t.range(n * (0 + 1) - 1, n * 0) = 1;
		t.range(n * (1 + 1) - 1, n * 1) = 2;
		t.range(n * (2 + 1) - 1, n * 2) = 1;
		t.range(n * (3 + 1) - 1, n * 3) = 2;
		t.range(n * (4 + 1) - 1, n * 4) = 4;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t.range(n * (0 + 1) - 1, n * 0) = 1;
		t.range(n * (1 + 1) - 1, n * 1) = 3;
		t.range(n * (2 + 1) - 1, n * 2) = 4;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t.range(n * (1 + 1) - 1, n * 1) = 1;
		t.range(n * (2 + 1) - 1, n * 2) = 2;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t[4 * 1 + 0] = 0;
		t[4 * 1 + 1] = 1;
		t[4 * 1 + 2] = 1;
		t[4 * 1 + 3] = 0;
		t[4 * 2 + 0] = 0;
		t[4 * 2 + 1] = 0;
		t[4 * 2 + 2] = 1;
		t[4 * 2 + 3] = 1;
		t[4 * 4 + 0] = 0;
		t[4 * 4 + 1] = 0;
		t[4 * 4 + 2] = 0;
		t[4 * 4 + 3] = 1;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t[4 * 1 + 0] = 0;
		t[4 * 1 + 1] = 1;
		t[4 * 1 + 2] = 1;
		t[4 * 1 + 3] = 0;
		t[4 * 3 + 0] = 0;
		t[4 * 3 + 1] = 0;
		t[4 * 3 + 2] = 0;
		t[4 * 3 + 3] = 1;
		t[4 * 4 + 0] = 0;
		t[4 * 4 + 1] = 0;
		t[4 * 4 + 2] = 1;
		t[4 * 4 + 3] = 1;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t[4 * 1 + 0] = 0;
		t[4 * 1 + 1] = 1;
		t[4 * 1 + 2] = 0;
		t[4 * 1 + 3] = 1;
		t[4 * 2 + 0] = 0;
		t[4 * 2 + 1] = 1;
		t[4 * 2 + 2] = 1;
		t[4 * 2 + 3] = 1;
		o_rgb.put(t);
	}
	{
		rgb_t t{};
		t[4 * 1 + 0] = 0;
		t[4 * 1 + 1] = 0;
		t[4 * 1 + 2] = 1;
		t[4 * 1 + 3] = 1;
		t[4 * 2 + 0] = 0;
		t[4 * 2 + 1] = 0;
		t[4 * 2 + 2] = 1;
		t[4 * 2 + 3] = 1;
		o_rgb.put(t);
	}
	for (auto i{0}; i < 128; i++) {
		o_rgb.put(i);
	}
}

void Testbench::fetch_result() {
	i_result.reset();
	wait();
	for (auto i{0}; i < 16; i++) {
		auto result{i_result.get()};
		std::cout << std::bitset<2>(result.range(2, 1)) << ((i % 8 == 7) ? "\n" : " ");
	}
	std::cout << "00 01 01 10 01 10 10 11" << std::endl;
	std::cout << sc_time_stamp() - total_start_time << " elapsed.";
	sc_stop();
}
