
TARGET = freertos_esp32c3

# Toolchain
TOOLCHAIN_BIN = /Users/lucabevivino/xpack_rv_gcc_15.2/bin/
CC      = $(TOOLCHAIN_BIN)riscv-none-elf-gcc
OBJCOPY = $(TOOLCHAIN_BIN)riscv-none-elf-objcopy
SIZE = $(TOOLCHAIN_BIN)riscv-none-elf-size

# Paths
FREERTOS_DIR = ./FreeRTOS-Kernel
FREERTOS_PORT = ./bare_metal_port
FREERTOS_MEMMANG = $(FREERTOS_DIR)/portable/MemMang

# Headers
INC_DIRS += -I.
INC_DIRS += -I$(FREERTOS_DIR)/include
INC_DIRS += -I$(FREERTOS_PORT)

# Sources of FreeRTOS	
SRCS += $(FREERTOS_DIR)/tasks.c
SRCS += $(FREERTOS_DIR)/list.c
SRCS += $(FREERTOS_DIR)/queue.c
SRCS += $(FREERTOS_DIR)/timers.c
SRCS += $(FREERTOS_DIR)/event_groups.c
SRCS += $(FREERTOS_DIR)/stream_buffer.c
SRCS += $(FREERTOS_DIR)/croutine.c

# Porting and Memory Management
SRCS += $(FREERTOS_PORT)/port.c
SRCS += $(FREERTOS_PORT)/port_systimer.c
SRCS += $(FREERTOS_MEMMANG)/heap_4.c

# Starters
SRCS += main.c
ASMS += startup.S
ASMS += $(FREERTOS_PORT)/port_asm.S

# Compile options (ESP32-C3 RV32IMC)
CFLAGS  = -march=rv32im_zicsr -mabi=ilp32 -O2 
CFLAGS += -ffunction-sections -fdata-sections 
CFLAGS += -Wall -fmessage-length=0 $(INC_DIRS)

# Linker options
LDFLAGS = -T linker.ld -Wl,--gc-sections -nostartfiles -lc

# Object file
OBJS = $(SRCS:.c=.o) $(ASMS:.S=.o)

.PHONY: all clean flash

all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
	@echo "Linking $@"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@ -lgcc
	@$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	@echo "Generating $@"
	@$(OBJCOPY) -O binary $< $@

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	@echo "Assembling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

# esptool for direct flash on memory
#flash: $(TARGET).bin
#	esptool.py --chip esp32c3 write_flash 0x0 $(TARGET).bin
