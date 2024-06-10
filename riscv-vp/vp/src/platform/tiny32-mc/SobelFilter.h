#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <bitset>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
struct SobelFilter
    : public sc_core::sc_module
{
  tlm_utils::simple_target_socket<SobelFilter> tsock;
  static constexpr auto CLOCK_PERIOD{10};
  static constexpr auto io_width{8};
  using tb_to_dut_t = sc_dt::sc_biguint<(io_width + 1) * 8>;
  using dut_to_tb_t = sc_dt::sc_biguint<(io_width + 1) * 8>;
  sc_core::sc_fifo<tb_to_dut_t> i_tb_to_dut;
  sc_core::sc_fifo<dut_to_tb_t> o_dut_to_tb;
  SC_HAS_PROCESS(SobelFilter);
  SobelFilter(sc_core::sc_module_name n)
      : sc_core::sc_module(n),
        tsock("t_skt")
  {
    tsock.register_b_transport(this, &SobelFilter::blocking_transport);
    SC_THREAD(do_filter);
  }
  ~SobelFilter()
  {
  }
  void do_filter()
  {
    sc_core::wait(CLOCK_PERIOD * 1, sc_core::SC_NS);
    constexpr auto depth{4};
    constexpr auto width{depth * 2};
    constexpr auto n{3};
    std::array<std::array<sc_dt::sc_uint<n>, width>, depth> t{};
    for (auto i{0u}; i < t.size(); i++)
    {
      auto tmp{i_tb_to_dut.read()};
      for (auto j{0u}; j < t[0].size(); j++)
      {
        t[i][j] = tmp.range(n * (j + 1) - 1, n * j);
      }
    }
    std::array<std::array<std::array<std::array<bool, 2>, 2>, width>, depth> u{};
    for (auto i{0u}; i < u.size(); i++)
    {
      auto tmp{i_tb_to_dut.read()};
      for (auto j{0u}; j < u[0].size(); j++)
      {
        for (auto k{0u}; k < u[0][0].size(); k++)
        {
          for (auto l{0u}; l < u[0][0][0].size(); l++)
          {
            u[i][j][k][l] = tmp[u[0][0][0].size() * (u[0][0].size() * j + k) + l];
          }
        }
      }
    }
    while (true)
    {
      std::array<std::array<bool, width>, depth> a{};
      std::array<std::array<bool, width>, depth> b{};
      auto tmp{i_tb_to_dut.read()};
      std::array<bool, width> c{};
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
      dut_to_tb_t output{};
      for (auto i{0u}; i < b[0].size(); i++)
      {
        output[i] = b[b.size() - 1][i];
      }
      sc_core::wait(CLOCK_PERIOD * 1, sc_core::SC_NS);
      o_dut_to_tb.write(output);
    }
  }
  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay)
  {
    delay += sc_core::sc_time(CLOCK_PERIOD * (2 + payload.get_data_length() / 4), sc_core::SC_NS);
    constexpr auto SOBEL_FILTER_R_ADDR{0x000};
    constexpr auto SOBEL_FILTER_RESULT_ADDR{0x100};
    constexpr auto SOBEL_FILTER_R_NUM_ADDR{0x200};
    (io_width + 1 == payload.get_data_length()) || (std::cerr << "payload.get_data_len() == " << payload.get_data_length() << std::endl);
    (i_tb_to_dut.num_free() > 0) || (std::cerr << "i_tb_to_dut.num_free() == " << i_tb_to_dut.num_free() << std::endl);
    (o_dut_to_tb.num_free() > 0) || (std::cerr << "o_dut_to_tb.num_free() == " << o_dut_to_tb.num_free() << std::endl);
    if (tlm::TLM_WRITE_COMMAND == payload.get_command())
    {
      (SOBEL_FILTER_R_ADDR == payload.get_address()) || (std::cerr << "payload.get_address() == " << payload.get_address() << std::endl);
      tb_to_dut_t tb_to_dut{};
      for (auto i{0u}; i < payload.get_data_length(); i++)
      {
        tb_to_dut.range(8 * (i + 1) - 1, 8 * i) = payload.get_data_ptr()[i];
      }
      i_tb_to_dut.write(tb_to_dut);
    }
    else if (tlm::TLM_READ_COMMAND == payload.get_command())
    {
      if (SOBEL_FILTER_RESULT_ADDR == payload.get_address())
      {
        auto dut_to_tb{o_dut_to_tb.read()};
        payload.get_data_ptr()[0] = 1;
        for (auto i{0}; i < io_width; i++)
        {
          payload.get_data_ptr()[i + 1] = sc_dt::sc_uint<8>(dut_to_tb.range(8 * (i + 1) - 1, 8 * i));
        }
      }
      else if (SOBEL_FILTER_R_NUM_ADDR == payload.get_address())
      {
        auto dut_to_tb{o_dut_to_tb.num_available()};
        payload.get_data_ptr()[0] = 1;
        payload.get_data_ptr()[1] = dut_to_tb;
      }
      else
      {
        std::cerr << "payload.get_address() == " << payload.get_address() << std::endl;
      }
    }
    else
    {
      std::cerr << "payload.get_command() == " << payload.get_command() << std::endl;
    }
    payload.set_response_status(tlm::TLM_OK_RESPONSE);
  };
};
#endif
