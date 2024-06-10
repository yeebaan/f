#include <stdio.h>
#include <stdint.h>
int sem_init(uint32_t *__sem, uint32_t count) __THROW
{
  *__sem = count;
  return 0;
}
int sem_wait(uint32_t *__sem) __THROW
{
  uint32_t value, success; // RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     beqz %[value],L%=                   # if zero, try again\n\t\
     addi %[value],%[value],-1           # value --\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
                       : [value] "=r"(value), [success] "=r"(success)
                       : [__sem] "r"(__sem)
                       : "memory");
  return 0;
}
int sem_post(uint32_t *__sem) __THROW
{
  uint32_t value, success; // RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     addi %[value],%[value], 1           # value ++\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
                       : [value] "=r"(value), [success] "=r"(success)
                       : [__sem] "r"(__sem)
                       : "memory");
  return 0;
}
uint32_t lock;
static const uint32_t DMA_OP_MEMCPY = 1;
static volatile uint32_t *const DMA_SRC_ADDR = (uint32_t *)(0x03000000 + 0);
static volatile uint32_t *const DMA_DST_ADDR = (uint32_t *)(0x03000000 + 4);
static volatile uint32_t *const DMA_LEN_ADDR = (uint32_t *)(0x03000000 + 8);
static volatile uint32_t *const DMA_OP_ADDR = (uint32_t *)(0x03000000 + 12);
static uint8_t *const SOBEL_FILTER_R_ADDR = (uint8_t *)(0x04000000 + 0x000);
static uint8_t *const SOBEL_FILTER_RESULT_ADDR = (uint8_t *)(0x04000000 + 0x100);
static uint8_t *const SOBEL_FILTER_R_NUM_ADDR = (uint8_t *)(0x04000000 + 0x200);
static uint8_t *const SOBEL_FILTER_R_ADDR_2 = (uint8_t *)(0x05000000 + 0x000);
static uint8_t *const SOBEL_FILTER_RESULT_ADDR_2 = (uint8_t *)(0x05000000 + 0x100);
static uint8_t *const SOBEL_FILTER_R_NUM_ADDR_2 = (uint8_t *)(0x05000000 + 0x200);
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
static volatile uint8_t tb_to_dut_2[tb_to_dut_size] = {0};
static volatile uint8_t dut_to_tb_2[dut_to_tb_size] = {0};
int main(int hart_id, char **unused)
{
  if (hart_id == 0)
  {
    sem_init(&lock, 1);
  }
  uint8_t *r_addr = (hart_id == 0) ? SOBEL_FILTER_R_ADDR : SOBEL_FILTER_R_ADDR_2;
  uint8_t *result_addr = (hart_id == 0) ? SOBEL_FILTER_RESULT_ADDR : SOBEL_FILTER_RESULT_ADDR_2;
  uint8_t *r_num_addr = (hart_id == 0) ? SOBEL_FILTER_R_NUM_ADDR : SOBEL_FILTER_R_NUM_ADDR_2;
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 010210 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 042121 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 0431 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 0210 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 0b10000000110001100000 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 0b11001000000001100000 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 0b111010100000 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  for (int i = 0; i < tb_to_dut_size; i++)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[i] = 0b110011000000 >> (8 * i);
  }
  write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
  sem_wait(&lock);
  (void)((hart_id == 0) && (printf("                             \n")));
  (void)((hart_id == 0) && (printf("        |          |         \n")));
  (void)((hart_id == 0) && (printf("hart_id |  A B Cin |  S Cout \n")));
  sem_post(&lock);
  for (int i = (hart_id == 0) ? 1 : 0; i < 8; i += 2)
  {
    ((hart_id == 0) ? tb_to_dut : tb_to_dut_2)[0] = i;
    write_data_to_ACC(r_addr, ((hart_id == 0) ? tb_to_dut : tb_to_dut_2), tb_to_dut_size);
    while (1)
    {
      ((hart_id == 0) ? dut_to_tb : dut_to_tb_2)[0] = 0;
      read_data_from_ACC(r_num_addr, ((hart_id == 0) ? dut_to_tb : dut_to_tb_2), dut_to_tb_size);
      uint8_t sample[dut_to_tb_size] = {0};
      while (1)
      {
        for (int i = 0; i < dut_to_tb_size; i++)
        {
          sample[i] = ((hart_id == 0) ? dut_to_tb : dut_to_tb_2)[i];
        }
        if (1 == sample[0])
        {
          break;
        }
      }
      if (0 < sample[1])
      {
        break;
      }
    }
    ((hart_id == 0) ? dut_to_tb : dut_to_tb_2)[0] = 0;
    read_data_from_ACC(result_addr, ((hart_id == 0) ? dut_to_tb : dut_to_tb_2), dut_to_tb_size);
    uint8_t sample[dut_to_tb_size] = {0};
    while (1)
    {
      for (int i = 0; i < dut_to_tb_size; i++)
      {
        sample[i] = ((hart_id == 0) ? dut_to_tb : dut_to_tb_2)[i];
      }
      if (1 == sample[0])
      {
        break;
      }
    }
    sem_wait(&lock);
    printf("    ");
    printf("%d", hart_id);
    printf("   |  ");
    printf("%d", (i >> 2) & 1);
    printf(" ");
    printf("%d", (i >> 1) & 1);
    printf(" ");
    printf("%d", (i >> 0) & 1);
    printf("   |  ");
    printf("%d", (sample[1] >> 1) & 1);
    printf(" ");
    printf("%d", (sample[1] >> 2) & 1);
    printf("\n");
    sem_post(&lock);
  }
  sem_wait(&lock);
  (void)((hart_id == 0) && (printf("        |          |         \n")));
  sem_post(&lock);
}
