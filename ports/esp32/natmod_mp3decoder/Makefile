# Location of top-level MicroPython directory
MPY_DIR = ../../..

# Name of module (different to built-in btree so it can coexist)
# MOD = mp3decoder_$(ARCH)
MOD = mp3decoder

# Source files (.c or .py)
SRC = mp3decoder.py

# Architecture to build for (x86, x64, armv7m, xtensa, xtensawin)
ARCH = xtensawin

CUSTOM_DEFS = -D__DBINTERFACE_PRIVATE=1 -Dmpool_error="(void)" -Dabort=abort_ "-Dvirt_fd_t=void*" $(DEFS_EXTRA)
CFLAGS += -Wno-old-style-definition -Wno-sign-compare -Wno-unused-parameter -Wunused-function -Wunused-const-variable $(CUSTOM_DEFS)

SRC += \
	mp3decoder.c \
    ../Adafruit_MP3/src/bitstream.c \
    ../Adafruit_MP3/src/buffers.c \
    ../Adafruit_MP3/src/dct32.c \
    ../Adafruit_MP3/src/dequant.c \
    ../Adafruit_MP3/src/dqchan.c \
    ../Adafruit_MP3/src/huffman.c \
    ../Adafruit_MP3/src/hufftabs.c \
    ../Adafruit_MP3/src/imdct.c \
    ../Adafruit_MP3/src/mp3dec.c \
    ../Adafruit_MP3/src/mp3tabs.c \
    ../Adafruit_MP3/src/polyphase.c \
    ../Adafruit_MP3/src/scalfact.c \
    ../Adafruit_MP3/src/stproc.c \
    ../Adafruit_MP3/src/subband.c \
    ../Adafruit_MP3/src/trigtabs.c \

    # shared-bindings/audiomp3/MP3Decoder.c \
    # shared-bindings/audiomp3/__init__.c \
    # shared-module/audiomp3/MP3Decoder.c \
    # shared-module/audiomp3/__init__.c
# 	shared-bindings/

include $(MPY_DIR)/py/dynruntime.mk

# btree needs gnu99 defined
CFLAGS += -std=gnu99
