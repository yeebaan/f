#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>

#include <systemc>


#include "filter_def.h"


class Testbench : public sc_core::sc_module {
	public:
		sc_core::sc_in_clk i_clk;
		sc_core::sc_out < bool >  o_rst;
		o_rgb_t o_rgb;
		i_result_t i_result;
		
		SC_HAS_PROCESS(Testbench);

		Testbench(sc_core::sc_module_name n);
		~Testbench();

		sc_core::sc_time total_start_time;

private:

  	void feed_rgb();
	void fetch_result();
};
#endif
