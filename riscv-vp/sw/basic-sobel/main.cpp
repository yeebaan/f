#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

unsigned char header[54] = {
    0x42,        // identity : B
    0x4d,        // identity : M
    0, 0, 0, 0,  // file size
    0, 0,        // reserved1
    0, 0,        // reserved2
    54, 0, 0, 0, // RGB data offset
    40, 0, 0, 0, // struct BITMAPINFOHEADER size
    0, 0, 0, 0,  // bmp width
    0, 0, 0, 0,  // bmp height
    1, 0,        // planes
    24, 0,       // bit per pixel
    0, 0, 0, 0,  // compression
    0, 0, 0, 0,  // data size
    0, 0, 0, 0,  // h resolution
    0, 0, 0, 0,  // v resolution
    0, 0, 0, 0,  // used colors
    0, 0, 0, 0   // important colors
};

union word
{
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};

unsigned int input_rgb_raw_data_offset;
const unsigned int output_rgb_raw_data_offset = 54;
int width;
int height;
unsigned int width_bytes;
unsigned char bits_per_pixel;
unsigned short bytes_per_pixel;
unsigned char *source_bitmap;
unsigned char *target_bitmap;
const int WHITE = 255;
const int BLACK = 0;
const int THRESHOLD = 90;

// Sobel Filter ACC
static char *const SOBELFILTER_START_ADDR = reinterpret_cast<char *>(0x73000000);
static char *const SOBELFILTER_READ_ADDR = reinterpret_cast<char *>(0x73000010);

// DMA
static volatile uint32_t *const DMA_SRC_ADDR = (uint32_t *)0x70000000;
static volatile uint32_t *const DMA_DST_ADDR = (uint32_t *)0x70000004;
static volatile uint32_t *const DMA_LEN_ADDR = (uint32_t *)0x70000008;
static volatile uint32_t *const DMA_OP_ADDR = (uint32_t *)0x7000000C;
static volatile uint32_t *const DMA_STAT_ADDR = (uint32_t *)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;
int read_bmp(std::string infile_name)
{
  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(infile_name.c_str(), "rb");
  if (fp_s == NULL)
  {
    printf("fopen %s error\n", infile_name.c_str());
    return -1;
  }
  // move offset to 10 to find rgb raw data offset
  fseek(fp_s, 10, SEEK_SET);
  assert(fread(&input_rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s));

  // move offset to 18 to get width & height;
  fseek(fp_s, 18, SEEK_SET);
  assert(fread(&width, sizeof(unsigned int), 1, fp_s));
  assert(fread(&height, sizeof(unsigned int), 1, fp_s));

  // get bit per pixel
  fseek(fp_s, 28, SEEK_SET);
  assert(fread(&bits_per_pixel, sizeof(unsigned short), 1, fp_s));
  bytes_per_pixel = bits_per_pixel / 8;

  // move offset to input_rgb_raw_data_offset to get RGB raw data
  fseek(fp_s, input_rgb_raw_data_offset, SEEK_SET);

  source_bitmap =
      (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
  if (source_bitmap == NULL)
  {
    printf("malloc images_s error\n");
    return -1;
  }

  target_bitmap =
      (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
  if (target_bitmap == NULL)
  {
    printf("malloc target_bitmap error\n");
    return -1;
  }

  assert(fread(source_bitmap, sizeof(unsigned char),
               (size_t)(long)width * height * bytes_per_pixel, fp_s));
  fclose(fp_s);

  unsigned int file_size; // file size
  // file size
  file_size = width * height * bytes_per_pixel + output_rgb_raw_data_offset;
  header[2] = (unsigned char)(file_size & 0x000000ff);
  header[3] = (file_size >> 8) & 0x000000ff;
  header[4] = (file_size >> 16) & 0x000000ff;
  header[5] = (file_size >> 24) & 0x000000ff;

  // width
  header[18] = width & 0x000000ff;
  header[19] = (width >> 8) & 0x000000ff;
  header[20] = (width >> 16) & 0x000000ff;
  header[21] = (width >> 24) & 0x000000ff;

  // height
  header[22] = height & 0x000000ff;
  header[23] = (height >> 8) & 0x000000ff;
  header[24] = (height >> 16) & 0x000000ff;
  header[25] = (height >> 24) & 0x000000ff;

  // bit per pixel
  header[28] = bits_per_pixel;

  return 0;
}

int write_bmp(std::string outfile_name)
{
  FILE *fp_t = NULL; // target file handler

  fp_t = fopen(outfile_name.c_str(), "wb");
  if (fp_t == NULL)
  {
    printf("fopen %s error\n", outfile_name.c_str());
    return -1;
  }

  // write header
  fwrite(header, sizeof(unsigned char), output_rgb_raw_data_offset, fp_t);

  // write image
  fwrite(target_bitmap, sizeof(unsigned char),
         (size_t)(long)width * height * bytes_per_pixel, fp_t);

  fclose(fp_t);
  return 0;
}

void write_data_to_ACC(char *ADDR, volatile unsigned char *buffer, int len)
{
  if (_is_using_dma)
  {
    // Using DMA
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR = DMA_OP_MEMCPY;
  }
  else
  {
    // Directly Send
    memcpy(ADDR, (void*)buffer, sizeof(unsigned char) * len);
  }
}
void read_data_from_ACC(char *ADDR, volatile unsigned char *buffer, int len)
{
  if (_is_using_dma)
  {
    // Using DMA
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR = DMA_OP_MEMCPY;
  }
  else
  {
    // Directly Read
    memcpy((void*)buffer, ADDR, sizeof(unsigned char) * len);
  }
}

#include <iostream>
#include <array>

template <
    typename X,
    typename Y,
    typename Z>
auto get(
    X row,
    Y col,
    Z channel)
{
  if ((0 <= row && row < height) &&
      (0 <= col && col < width) &&
      (0 <= channel && channel < bytes_per_pixel))
  {
    return source_bitmap[(row * width + col) * bytes_per_pixel + channel];
  }
  else
  {
  }
  return uint8_t{};
}

template <
    typename X,
    typename Y,
    typename Z,
    typename A>
auto set(
    X row,
    Y col,
    Z channel,
    A data)
{
  if ((0 <= row && row < height) &&
      (0 <= col && col < width) &&
      (0 <= channel && channel < bytes_per_pixel))
  {
    target_bitmap[(row * width + col) * bytes_per_pixel + channel] = data;
  }
  else
  {
  }
}

constexpr auto tb_to_dut_size{16};
constexpr auto dut_to_tb_size{16};
static volatile uint8_t tb_to_dut[tb_to_dut_size] = {0};
static volatile uint8_t dut_to_tb[dut_to_tb_size] = {0};

int main(int argc, char *argv[])
{
  read_bmp("lena_std_short.bmp");
  for (auto row{0}; row < height; row++)
  {
    (row % 2 == 0) && (std::cerr << ".");
    for (auto col{0}; col < width + 4; col++)
    {
      tb_to_dut[ 0] = get(row - 2, col - 2, 0);
      tb_to_dut[ 1] = get(row - 2, col - 2, 1);
      tb_to_dut[ 2] = get(row - 2, col - 2, 2);
      tb_to_dut[ 3] = get(row - 1, col - 2, 0);
      tb_to_dut[ 4] = get(row - 1, col - 2, 1);
      tb_to_dut[ 5] = get(row - 1, col - 2, 2);
      tb_to_dut[ 6] = get(row + 0, col - 2, 0);
      tb_to_dut[ 7] = get(row + 0, col - 2, 1);
      tb_to_dut[ 8] = get(row + 0, col - 2, 2);
      tb_to_dut[ 9] = get(row + 1, col - 2, 0);
      tb_to_dut[10] = get(row + 1, col - 2, 1);
      tb_to_dut[11] = get(row + 1, col - 2, 2);
      tb_to_dut[12] = get(row + 2, col - 2, 0);
      tb_to_dut[13] = get(row + 2, col - 2, 1);
      tb_to_dut[14] = get(row + 2, col - 2, 2);
      write_data_to_ACC(SOBELFILTER_START_ADDR, tb_to_dut, tb_to_dut_size);
      for (auto i{0}; i < dut_to_tb_size; i++) {
        dut_to_tb[i] = 0;
      }
      read_data_from_ACC(SOBELFILTER_READ_ADDR, dut_to_tb, dut_to_tb_size);
      while (
          (dut_to_tb[0] != dut_to_tb[2]) ||
          (dut_to_tb[1] != dut_to_tb[3]) ||
          (0x0f != (0x0f & (dut_to_tb[0]))) ||
          (0xf0 != (0xf0 & (dut_to_tb[1]))) ||
          (0x0f != (0x0f & (dut_to_tb[2]))) ||
          (0xf0 != (0xf0 & (dut_to_tb[3]))))
      {
      }
      auto dut_to_tb_0{dut_to_tb[0] & dut_to_tb[1]};
      set(row, col - 4, 0, dut_to_tb_0);
      set(row, col - 4, 1, dut_to_tb_0);
      set(row, col - 4, 2, dut_to_tb_0);
    }
  }
  std::cerr << std::endl;
  write_bmp("lena_std_out.bmp");
}