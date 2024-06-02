#include <iostream>
#include <array>

static char *const SOBELFILTER_START_ADDR = reinterpret_cast<char *>(0x73000000);
static char *const SOBELFILTER_READ_ADDR = reinterpret_cast<char *>(0x73000100);

static volatile uint32_t *const DMA_SRC_ADDR = (uint32_t *)0x70000000;
static volatile uint32_t *const DMA_DST_ADDR = (uint32_t *)0x70000004;
static volatile uint32_t *const DMA_LEN_ADDR = (uint32_t *)0x70000008;
static volatile uint32_t *const DMA_OP_ADDR = (uint32_t *)0x7000000C;
static volatile uint32_t *const DMA_STAT_ADDR = (uint32_t *)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;

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
    memcpy(ADDR, (void*)buffer, sizeof(unsigned char) * len);
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
    memcpy((void*)buffer, ADDR, sizeof(unsigned char) * len);
  }
}

constexpr auto tb_to_dut_size{16};
constexpr auto dut_to_tb_size{16};
static volatile uint8_t tb_to_dut[tb_to_dut_size] = {0};
static volatile uint8_t dut_to_tb[dut_to_tb_size] = {0};

int main()
{
      tb_to_dut[14] = get(row + 2, col - 2, 2);
      write_data_to_ACC(SOBELFILTER_START_ADDR, tb_to_dut, tb_to_dut_size);
      for (auto i{0}; i < dut_to_tb_size; i++) {
        dut_to_tb[i] = 0;
      }
      read_data_from_ACC(SOBELFILTER_READ_ADDR, dut_to_tb, dut_to_tb_size);
}
