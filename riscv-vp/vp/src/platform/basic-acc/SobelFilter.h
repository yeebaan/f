#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <bitset>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "filter_def.h"
struct SobelFilter
    : public sc_core::sc_module
{
  static constexpr auto data_latency{CLOCK_PERIOD * 1};

  tlm_utils::simple_target_socket<SobelFilter> tsock;

  using tb_to_dut_t = std::bitset<5 * 3 * 8>;

  sc_core::sc_fifo<tb_to_dut_t> i_tb_to_dut;
  sc_core::sc_fifo<uint8_t> o_dut_to_tb;

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
    std::array<uint8_t, 25> buffer{};
    while (true)
    {
      if (true)
      {
        auto input{i_tb_to_dut.read()};
        for (auto i{0}; i < 20; i++)
        {
          buffer[i] = buffer[i + 5];
        }
        for (auto i{0}; i < 5; i++)
        {
          buffer[20 + i] =
              (((input[24 * i + 8 * 0 + 0] + input[24 * i + 8 * 1 + 0] + input[24 * i + 8 * 2 + 0]) << 0) +
               ((input[24 * i + 8 * 0 + 1] + input[24 * i + 8 * 1 + 1] + input[24 * i + 8 * 2 + 1]) << 1) +
               ((input[24 * i + 8 * 0 + 2] + input[24 * i + 8 * 1 + 2] + input[24 * i + 8 * 2 + 2]) << 2) +
               ((input[24 * i + 8 * 0 + 3] + input[24 * i + 8 * 1 + 3] + input[24 * i + 8 * 2 + 3]) << 3) +
               ((input[24 * i + 8 * 0 + 4] + input[24 * i + 8 * 1 + 4] + input[24 * i + 8 * 2 + 4]) << 4) +
               ((input[24 * i + 8 * 0 + 5] + input[24 * i + 8 * 1 + 5] + input[24 * i + 8 * 2 + 5]) << 5) +
               ((input[24 * i + 8 * 0 + 6] + input[24 * i + 8 * 1 + 6] + input[24 * i + 8 * 2 + 6]) << 6) +
               ((input[24 * i + 8 * 0 + 7] + input[24 * i + 8 * 1 + 7] + input[24 * i + 8 * 2 + 7]) << 7)) /
              3;
        }
        o_dut_to_tb.write(
            (
                buffer[0] * 1 +
                buffer[1] * 4 +
                buffer[2] * 7 +
                buffer[3] * 4 +
                buffer[4] * 1 +
                buffer[5] * 4 +
                buffer[6] * 16 +
                buffer[7] * 26 +
                buffer[8] * 16 +
                buffer[9] * 4 +
                buffer[10] * 7 +
                buffer[11] * 26 +
                buffer[12] * 41 +
                buffer[13] * 26 +
                buffer[14] * 7 +
                buffer[15] * 4 +
                buffer[16] * 16 +
                buffer[17] * 26 +
                buffer[18] * 16 +
                buffer[19] * 4 +
                buffer[20] * 1 +
                buffer[21] * 4 +
                buffer[22] * 7 +
                buffer[23] * 4 +
                buffer[24] * 1) /
            273);
      }
      else
      {
        i_tb_to_dut.read();
        for (auto i{0}; i < 25; i++)
        {
          buffer[i] = 255;
        }
        o_dut_to_tb.write(255);
      }
      sc_core::wait(data_latency, sc_core::SC_NS);
    }
  }

  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay)
  {
    (i_tb_to_dut.num_free() > 0) || (std::cerr << "i_tb_to_dut.num_free() == " << i_tb_to_dut.num_free() << std::endl);
    (o_dut_to_tb.num_free() > 0) || (std::cerr << "o_dut_to_tb.num_free() == " << o_dut_to_tb.num_free() << std::endl);
    (payload.get_data_length() == 16) || (std::cerr << "payload.get_data_len() == " << payload.get_data_length() << std::endl);
    if (tlm::TLM_WRITE_COMMAND == payload.get_command())
    {
      tb_to_dut_t tb_to_dut{};
      for (auto i{0}; i < 15; i++)
      {
        for (auto j{0}; j < 8; j++)
        {
          tb_to_dut[8 * i + j] = (payload.get_data_ptr()[i] >> j) & 1;
        }
      }
      i_tb_to_dut.write(tb_to_dut);
    }
    else if (tlm::TLM_READ_COMMAND == payload.get_command())
    {
      auto dut_to_tb{o_dut_to_tb.read()};
      payload.get_data_ptr()[0] = dut_to_tb | 0x0f;
      payload.get_data_ptr()[1] = dut_to_tb | 0xf0;
      payload.get_data_ptr()[2] = dut_to_tb | 0x0f;
      payload.get_data_ptr()[3] = dut_to_tb | 0xf0;
      delay += sc_core::sc_time(data_latency, sc_core::SC_NS);
    }
    else
    {
      std::cerr << "payload.get_command() == " << payload.get_command() << std::endl;
    }
    payload.set_response_status(tlm::TLM_OK_RESPONSE);
  };
};
#endif
