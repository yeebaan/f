#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>



#include "filter_def.h"

class SobelFilter: public sc_core::sc_module
{
	public:
		sc_core::sc_in_clk i_clk;
		sc_core::sc_in < bool >  i_rst;
		i_rgb_t i_rgb;
		o_result_t o_result;

		SC_HAS_PROCESS( SobelFilter );
		SobelFilter( sc_core::sc_module_name n );
		~SobelFilter();
private:
		void do_filter();
};
#endif
