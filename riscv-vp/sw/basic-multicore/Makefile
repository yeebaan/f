OBJECTS  = main.o bootstrap.o
CFLAGS   = -march=rv32ima -mabi=ilp32 -Wall -Wextra -pedantic
LDFLAGS  = -nostartfiles -Wl,--no-relax

VP       = ~/ee6470/riscv-vp/vp/build/bin/tiny32-mc
VP_FLAGS = --error-on-zero-traphandler=true --intercept-syscalls

include ../Makefile.common
