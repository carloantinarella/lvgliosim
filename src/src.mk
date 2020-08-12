CSRCS += io.c
CSRCS += page1.c

DEPPATH += --dep-path src
VPATH += :src


CFLAGS += "-I$(LVGL_DIR)/src"
