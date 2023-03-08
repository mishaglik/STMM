#-----------------------
# Compiler/linker flags
#-----------------------

CC = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb

CFLAGS = \
	-std=c18 \
	-I include \
	-Wall \
	-Wextra \
	-march=armv6-m \
	-mcpu=cortex-m0

LDFLAGS = \
	-Wall \
	-Wextra \
	-march=armv6-m \
	-mcpu=cortex-m0 \
	-Wl,--start-group -lgcc -lc -lg -Wl,--end-group -Wl,--gc-sections \
	-Wl,--warn-common \
	-Wl,--fatal-warnings \
	-Wl,-z,max-page-size=8 \
	-Wl,-T,entry.lds

ifeq ($(DEBUG),1)
	CFLAGS += -g
endif

ifeq ($(RELEASE),1)
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

#-------
# Files
#-------

SOURCES = \
	entry.S \
	lib/centry.c \
	lib/button.c \
	lib/seg7.c \
	lib/time.c \
	labs/game.c \
	labs/reaction.c

HEADERS = \
	include/common.h \
	include/button.h \
	include/gpio.h \
	include/rcc.h \

OBJECTS_THIRDWAY_DONE = $(SOURCES:labs/%.c=build/%.o)
OBJECTS_HALFWAY_DONE  = $(OBJECTS_THIRDWAY_DONE:lib/%.c=build/%.o)
OBJECTS               = $(OBJECTS_HALFWAY_DONE:%.S=build/%.o)

EXECUTABLE_FLASH = build/reaction.elf
BINARY_FLASH     = build/reaction.bin

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE_FLASH) $(BINARY_FLASH) $(SOURCES)

$(EXECUTABLE_FLASH): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BINARY_FLASH): $(EXECUTABLE_FLASH)
	arm-none-eabi-objcopy -O binary $< $@

build/%.o: labs/%.c $(HEADERS)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<
	$(CC) $(CFLAGS) -S -o $(@:%.o=%.s) -c $<

build/%.o: lib/%.c $(HEADERS)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<
	$(CC) $(CFLAGS) -S -o $(@:%.o=%.s) -c $<

build/%.o: %.S
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build

#----------------------
# Hardware interaction
#----------------------

flash: $(BINARY_FLASH)
	st-flash write $(BINARY_FLASH) 0x08000000

flash2: $(BINARY_FLASH)
	st-flash --connect-under-reset write $(BINARY_FLASH) 0x08000000

GDB_FLAGS = \
	--eval-command="set architecture arm" \
	--eval-command="set gnutarget elf32-littlearm" \
	--eval-command="target remote localhost:1234" \
	--eval-command="file $(EXECUTABLE_FLASH)"

hardware: $(EXECUTABLE_FLASH)
	st-util -p 1234

gdb: $(BINARY_FLASH)
	$(GDB) $(GDB_FLAGS)

.PHONY: all clean flash flash2 hardware gdb
