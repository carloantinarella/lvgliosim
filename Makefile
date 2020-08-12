#
# Makefile
#
INCLUDE = include
CC = gcc
CFLAGS = -Wall -Wshadow -Wundef -Wmaybe-uninitialized
CFLAGS += -O3 -g3 -I./ -I$(INCLUDE)
LDFLAGS += -lSDL2 -lm
LDFLAGS += -lrt
LDFLAGS += -lpthread
BIN = my_prog
VPATH = 
OBJDIR=objs

LVGL_DIR = ${shell pwd}

MAINSRC = main.c


#LIBRARIES
include ./lvgl/lv_core/lv_core.mk
include ./lvgl/lv_hal/lv_hal.mk
include ./lvgl/lv_objx/lv_objx.mk
include ./lvgl/lv_misc/lv_fonts/lv_fonts.mk
include ./lvgl/lv_misc/lv_misc.mk
include ./lvgl/lv_themes/lv_themes.mk
include ./lvgl/lv_draw/lv_draw.mk

#DRIVERS
include ./lvgl/lv_drivers/display/display.mk
include ./lvgl/lv_drivers/indev/indev.mk

#SRC FILES
include ./src/src.mk

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

all: default

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@
	@echo "CC $<"

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CC) -o $(BIN) $(addprefix $(OBJDIR)/, $(MAINOBJ)) $(AOBJS) $(addprefix $(OBJDIR)/, $(COBJS)) $(LDFLAGS)

clean:
	rm -f $(BIN) $(AOBJS) $(addprefix $(OBJDIR)/, $(COBJS)) $(addprefix $(OBJDIR)/, $(MAINOBJ))
