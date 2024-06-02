#include <iostream>
#include <array>
#include <bitset>
#include <cstring>
#include <cstdint>
static char *const SOBEL_FILTER_R_ADDR = reinterpret_cast<char *>(0x73000000 + 0x00000000);
static char *const SOBEL_FILTER_RESULT_ADDR = reinterpret_cast<char *>(0x73000000 + 0x00000100);
static char *const SOBEL_FILTER_R_NUM_ADDR = reinterpret_cast<char *>(0x73000000 + 0x00000200);
static volatile uint32_t *const DMA_SRC_ADDR = (uint32_t *)0x70000000;
static volatile uint32_t *const DMA_DST_ADDR = (uint32_t *)0x70000004;
static volatile uint32_t *const DMA_LEN_ADDR = (uint32_t *)0x70000008;
static volatile uint32_t *const DMA_OP_ADDR = (uint32_t *)0x7000000C;
static volatile uint32_t *const DMA_STAT_ADDR = (uint32_t *)0x70000010;
static constexpr uint32_t DMA_OP_MEMCPY = 1;
constexpr bool _is_using_dma = true;
void write_data_to_ACC(char *ADDR, volatile unsigned char *buffer, int len)
{
  if (_is_using_dma)
  {
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR = DMA_OP_MEMCPY;
  }
  else
  {
    std::memcpy(ADDR, (void *)buffer, sizeof(unsigned char) * len);
  }
}
void read_data_from_ACC(char *ADDR, volatile unsigned char *buffer, int len)
{
  if (_is_using_dma)
  {
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR = DMA_OP_MEMCPY;
  }
  else
  {
    std::memcpy((void *)buffer, ADDR, sizeof(unsigned char) * len);
  }
}
constexpr auto io_width{8};
constexpr auto tb_to_dut_size{io_width + 1};
constexpr auto dut_to_tb_size{io_width + 1};
static volatile uint8_t tb_to_dut[tb_to_dut_size] = {0};
static volatile uint8_t dut_to_tb[dut_to_tb_size] = {0};
int main()
{
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 010210 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 042121 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0431 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0210 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b10000000110001100000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b11001000000001100000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b111010100000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (auto i{0}; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b110011000000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);

  std::cout << "__________________\n\n";
  std::cout << " A B Cin | S Cout \n";
  for (auto i{0}; i < 8; i++)
  {
    tb_to_dut[0] = i;
    write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
    dut_to_tb[0] = 0;
    read_data_from_ACC(SOBEL_FILTER_RESULT_ADDR, dut_to_tb, dut_to_tb_size);
    std::array<uint8_t, dut_to_tb_size> sample{};
    while (true)
    {
      for (auto i{0}; i < dut_to_tb_size; i++)
      {
        sample[i] = dut_to_tb[i];
      }
      if (1 == sample[0])
      {
        break;
      }
    }
    std::cout << " "
              << ((i >> 2) & 1) << " "
              << ((i >> 1) & 1) << " "
              << ((i >> 0) & 1) << "   | "
              << ((sample[1] >> 1) & 1) << " "
              << ((sample[1] >> 2) & 1) << "\n";
  }
  std::cout << "__________________\n";
  for (auto i{0}; i < (1 << 8); i++)
  {
    tb_to_dut[0] = i;
    write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
    dut_to_tb[0] = 0;
    read_data_from_ACC(SOBEL_FILTER_RESULT_ADDR, dut_to_tb, dut_to_tb_size);
    std::array<uint8_t, dut_to_tb_size> sample{};
    while (true)
    {
      for (auto i{0}; i < dut_to_tb_size; i++)
      {
        sample[i] = dut_to_tb[i];
      }
      if (1 == sample[0])
      {
        break;
      }
    }
  }
}
