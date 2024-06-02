#include <bitset>
#include "SobelFilter.h"

SobelFilter::SobelFilter(sc_core::sc_module_name n) : sc_core::sc_module(n)
{
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);

  i_rgb.clk_rst(i_clk, i_rst);
  o_result.clk_rst(i_clk, i_rst);
}

SobelFilter::~SobelFilter() {}

void SobelFilter::do_filter()
{
  {
    HLS_DEFINE_PROTOCOL("reset_0");
    i_rgb.reset();
    o_result.reset();
    wait();
  }
  constexpr auto depth{4};
  constexpr auto width{depth * 2};
  constexpr auto n{3};
  std::array<std::array<sc_uint<n>, width>, depth> t{};
  #ifndef NATIVE_SYSTEMC
    HLS_FLATTEN_ARRAY(t);
  #endif
  for (auto i{0u}; i < t.size(); i++) {
    #ifndef NATIVE_SYSTEMC
      HLS_UNROLL_LOOP(ALL, "loop1");
      auto tmp{i_rgb.get()};
    #else
      auto tmp{i_rgb.read()};
    #endif
    for (auto j{0u}; j < t[0].size(); j++) {
      t[i][j] = tmp.range(n * (j + 1) - 1, n * j);
    }
  }
  std::array<std::array<std::array<std::array<bool, 2>, 2>, width>, depth> u{};
  #ifndef NATIVE_SYSTEMC
    HLS_FLATTEN_ARRAY(u);
  #endif
  for (auto i{0u}; i < u.size(); i++) {
    #ifndef NATIVE_SYSTEMC
      HLS_UNROLL_LOOP(ALL, "loop2");
      auto tmp{i_rgb.get()};
    #else
      auto tmp{i_rgb.read()};
    #endif
    for (auto j{0u}; j < u[0].size(); j++) {
      for (auto k{0u}; k < u[0][0].size(); k++) {
        for (auto l{0u}; l < u[0][0][0].size(); l++) {
          u[i][j][k][l] = tmp[u[0][0][0].size() * (u[0][0].size() * j + k) + l];
        }
      }
    }
  }
  while (true)
  {
    #ifndef NATIVE_SYSTEMC
      HLS_PIPELINE_LOOP(HARD_STALL, 1, "pipe0");
      HLS_CONSTRAIN_LATENCY(0, HLS_ACHIEVABLE, "lat0");
    #endif
    std::array<std::array<bool, width>, depth> a{};
    #ifndef NATIVE_SYSTEMC
      HLS_FLATTEN_ARRAY(a);
    #endif
    std::array<std::array<bool, width>, depth> b{};
    #ifndef NATIVE_SYSTEMC
      HLS_FLATTEN_ARRAY(b);
    #endif
    #ifndef NATIVE_SYSTEMC
      auto tmp{i_rgb.get()};
    #else
      auto tmp{i_rgb.read()};
    #endif
    std::array<bool, width> c{};
    #ifndef NATIVE_SYSTEMC
      HLS_FLATTEN_ARRAY(c);
    #endif
    for (auto i{0u}; i < c.size(); i++)
    {
      c[i] = tmp[i];
    }
    for (auto i{0u}; i < b.size(); i++)
    {
      for (auto j{0u}; j < b[0].size(); j++)
      {
        a[i][j] = ((i == 0) ? c : (b[i - 1]))[t[i][j]];
        b[i][j] = u[i][j][a[i][j]][a[i][(j == 0) ? 0 : (j - 1)]];
      }
    }
    result_t output{};
    for (auto i{0u}; i < b[0].size(); i++) {
      output[i] = b[b.size() - 1][i];
    }
    #ifndef NATIVE_SYSTEMC
      o_result.put(output);
    #else
      o_result.write(output);
    #endif
  }
}
