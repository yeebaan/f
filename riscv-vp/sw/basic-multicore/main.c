#include <stdio.h>
#include <stdint.h>
static uint8_t *const SOBEL_FILTER_R_ADDR = (uint8_t *)(0x04000000 + 0x000);
static uint8_t *const SOBEL_FILTER_RESULT_ADDR = (uint8_t *)(0x04000000 + 0x100);
static volatile uint32_t *const DMA_SRC_ADDR = (uint32_t *)(0x03000000 + 0);
static volatile uint32_t *const DMA_DST_ADDR = (uint32_t *)(0x03000000 + 4);
static volatile uint32_t *const DMA_LEN_ADDR = (uint32_t *)(0x03000000 + 8);
static volatile uint32_t *const DMA_OP_ADDR = (uint32_t *)(0x03000000 + 12);
static const uint32_t DMA_OP_MEMCPY = 1;
void write_data_to_ACC(uint8_t *ADDR, volatile uint8_t *buffer, int len)
{
  *DMA_SRC_ADDR = (uint32_t)(buffer);
  *DMA_DST_ADDR = (uint32_t)(ADDR);
  *DMA_LEN_ADDR = len;
  *DMA_OP_ADDR = DMA_OP_MEMCPY;
}
void read_data_from_ACC(uint8_t *ADDR, volatile uint8_t *buffer, int len)
{
  *DMA_SRC_ADDR = (uint32_t)(ADDR);
  *DMA_DST_ADDR = (uint32_t)(buffer);
  *DMA_LEN_ADDR = len;
  *DMA_OP_ADDR = DMA_OP_MEMCPY;
}
#define io_width (8)
#define tb_to_dut_size (io_width + 1)
#define dut_to_tb_size (io_width + 1)
static volatile uint8_t tb_to_dut[tb_to_dut_size] = {0};
static volatile uint8_t dut_to_tb[dut_to_tb_size] = {0};
int main(int hart_id)
{
  int a=0;
	for (unsigned i=0; i<1000*hart_id; ++i){
		a=a+1;
	}
	printf("hard-id=%d a=%d\n", hart_id, a);
  if (hart_id == 0) return 0;
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 010210 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 042121 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0431 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0210 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b10000000110001100000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b11001000000001100000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b111010100000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    tb_to_dut[i] = 0b110011000000 >> (8 * i);
  }
  write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
  if (0 == hart_id) {
    printf("_______core0______\n\n");
    printf(" A B Cin | S Cout \n");
    for (int i = 0; i < 8; i++)
    {
      tb_to_dut[0] = i;
      write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
      dut_to_tb[0] = 0;
      read_data_from_ACC(SOBEL_FILTER_RESULT_ADDR, dut_to_tb, dut_to_tb_size);
      uint8_t sample[dut_to_tb_size] = {0};
      while (1)
      {
        for (int i = 0; i < dut_to_tb_size; i++)
        {
          sample[i] = dut_to_tb[i];
        }
        if (1 == sample[0])
        {
          break;
        }
      }
      printf(" ");
      printf("%d", (i >> 2) & 1);
      printf(" ");
      printf("%d", (i >> 1) & 1);
      printf(" ");
      printf("%d", (i >> 0) & 1);
      printf("   | ");
      printf("%d", (sample[1] >> 1) & 1);
      printf(" ");
      printf("%d", (sample[1] >> 2) & 1);
      printf("\n");
    }
    printf("__________________\n");
  }
  else if (1 == hart_id) {
    printf("_______core0______\n\n");
    printf(" A B Cin | S Cout \n");
    for (int i = 0; i < 8; i++)
    {
      tb_to_dut[0] = i;
      write_data_to_ACC(SOBEL_FILTER_R_ADDR, tb_to_dut, tb_to_dut_size);
      dut_to_tb[0] = 0;
      read_data_from_ACC(SOBEL_FILTER_RESULT_ADDR, dut_to_tb, dut_to_tb_size);
      uint8_t sample[dut_to_tb_size] = {0};
      while (1)
      {
        for (int i = 0; i < dut_to_tb_size; i++)
        {
          sample[i] = dut_to_tb[i];
        }
        if (1 == sample[0])
        {
          break;
        }
      }
      printf(" ");
      printf("%d", (i >> 2) & 1);
      printf(" ");
      printf("%d", (i >> 1) & 1);
      printf(" ");
      printf("%d", (i >> 0) & 1);
      printf("   | ");
      printf("%d", (sample[1] >> 1) & 1);
      printf(" ");
      printf("%d", (sample[1] >> 2) & 1);
      printf("\n");
    }
    printf("__________________\n");
  }
  else {
    printf("error hart_id = %d\n", hart_id);
  }
}