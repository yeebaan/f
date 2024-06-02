#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_
#include <systemc>

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

constexpr auto width = 64;

using rgb_t = sc_dt::sc_uint<width>;
using c_rgb_t = cynw_p2p<rgb_t>;
using i_rgb_t = cynw_p2p<rgb_t>::in;
using o_rgb_t = cynw_p2p<rgb_t>::base_out;

using result_t = sc_dt::sc_uint<width>;
using c_result_t = cynw_p2p<result_t>;
using i_result_t = cynw_p2p<result_t>::base_in;
using o_result_t = cynw_p2p<result_t>::out;

#endif
