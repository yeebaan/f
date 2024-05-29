all : main.cpp
	/opt/riscv-gnu-toolchain/bin/riscv-none-elf-g++ main.cpp -o main -std=c++14 -march=rv32ima -mabi=ilp32 -Wall -Wextra -pedantic -O3

sim: all
	~/ee6470/riscv-vp/vp/build/bin/riscv-vp-acc --intercept-syscalls main

clean:
	rm main
