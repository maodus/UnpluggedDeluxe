TARGET = build/UnpluggedDeluxe

INCDIR = include

OBJS = src/imports.o \
	src/stalloc.o \
	src/utils.o \
	src/custom_man.o \
	src/custom_loader.o \
	src/pspmalloc.o \
	src/main.o \
	src/exports.o 

CFLAGS = -Os -ffunction-sections -fdata-sections -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division -std=c99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PSP_FW_VERSION=660

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

LIBS = -lm

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
