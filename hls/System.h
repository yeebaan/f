#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "SobelFilter_wrap.h"
#else
#include "SobelFilter.h"
#endif

class System: public sc_core::sc_module
{
	public:
		SC_HAS_PROCESS( System );
		System( sc_core::sc_module_name n );
		~System();
private:
		Testbench tb;
#ifndef NATIVE_SYSTEMC
		SobelFilter_wrapper sobel_filter;
#else
		SobelFilter sobel_filter;
#endif
		sc_clock clk;
		sc_signal<bool> rst;
		c_rgb_t c_rgb;
		c_result_t c_result;
};
#endif
