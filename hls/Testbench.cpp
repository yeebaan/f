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

	total_start_time = sc_time_stamp();
	{
	  rgb_t t{};
      t.range(4 * 0 + 2, 4 * 0) = 0;
      t.range(4 * 1 + 2, 4 * 1) = 1;
      t.range(4 * 2 + 2, 4 * 2) = 2;
      t.range(4 * 3 + 2, 4 * 3) = 0;
      t.range(4 * 4 + 2, 4 * 4) = 1;
	  o_rgb.put(t);
	}
	{
	  rgb_t t{};
      t.range(4 * 0 + 2, 4 * 0) = 1;
      t.range(4 * 1 + 2, 4 * 1) = 2;
      t.range(4 * 2 + 2, 4 * 2) = 1;
      t.range(4 * 3 + 2, 4 * 3) = 2;
      t.range(4 * 4 + 2, 4 * 4) = 4;
	  o_rgb.put(t);
	}
	{
	  rgb_t t{};
      t.range(4 * 0 + 2, 4 * 0) = 1;
      t.range(4 * 1 + 2, 4 * 1) = 3;
      t.range(4 * 2 + 2, 4 * 2) = 4;
	  o_rgb.put(t);
	}
	{
	  rgb_t t{};
      t.range(4 * 1 + 2, 4 * 1) = 1;
      t.range(4 * 2 + 2, 4 * 2) = 2;
	  o_rgb.put(t);
	}
	{
	  rgb_t t{};
      t.range(4 * 1 + 2, 4 * 1) = 1;
      t.range(4 * 2 + 2, 4 * 2) = 2;
	  o_rgb.put(t);
	}
	{
	  rgb_t t{};
      t.range(4 * 1 + 2, 4 * 1) = 1;
      t.range(4 * 2 + 2, 4 * 2) = 2;
	  o_rgb.put(t);
	}
	{
	  rgb_t t{};
      t.range(4 * 1 + 2, 4 * 1) = 1;
      t.range(4 * 2 + 2, 4 * 2) = 2;
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
	for (auto i{0}; i < 10000; i++) {
		// std::cout << "inpt=" << std::bitset<7>(i) << " ";
        o_rgb.put(i);
    }
}

void Testbench::fetch_result() {
	i_result.reset();
	wait();
	for (auto i{0}; i < 4096 + 1; i++) {
		auto result{i_result.get()};
		std::cout << std::bitset<2>(result.range(3, 2));
		if (i % 16 == 0) {
			std::cout << std::endl;
		} else {
			std::cout << " ";
		};
	}
	std::cout << "00 01 01 01 01 10 10 01 01 10 10 10 10 11 11 00";
	std::cout << std::endl;
	std::cout << sc_time_stamp() - total_start_time << " elapsed." << std::endl;
	sc_stop();
}
