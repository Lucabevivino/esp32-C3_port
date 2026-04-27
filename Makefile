
TARGET = freertos_esp32c3

# Toolchain
TOOLCHAIN_BIN = /Users/lucabevivino/xpack_rv_gcc_15.2/bin/
CC      = $(TOOLCHAIN_BIN)riscv-none-elf-gcc
OBJCOPY = $(TOOLCHAIN_BIN)riscv-none-elf-objcopy
SIZE = $(TOOLCHAIN_BIN)riscv-none-elf-size
GDB = $(TOOLCHAIN_BIN)riscv-none-elf-gdb

#Esptool
ESPTOOL = esptool.py
PORT    = /dev/cu.usbmodem*
BAUD    = 921600
FLASH_ADDR = 0x0

# Output files
TARGET_ELF = freertos_esp32c3.elf
TARGET_BIN = freertos_esp32c3.bin

# Paths
FREERTOS_DIR = ./FreeRTOS-Kernel
FREERTOS_PORT = ./bare_metal_port
FREERTOS_MEMMANG = $(FREERTOS_DIR)/portable/MemMang
UART_DRIVER = ./uart

# Headers
INC_DIRS += -I.
INC_DIRS += -I$(FREERTOS_DIR)/include
INC_DIRS += -I$(FREERTOS_PORT)
INC_DIRS += -I$(UART_DRIVER)

# Sources of FreeRTOS	
SRCS += $(FREERTOS_DIR)/tasks.c
SRCS += $(FREERTOS_DIR)/list.c
SRCS += $(FREERTOS_DIR)/queue.c
SRCS += $(FREERTOS_DIR)/timers.c
SRCS += $(FREERTOS_DIR)/event_groups.c
SRCS += $(FREERTOS_DIR)/stream_buffer.c
SRCS += $(FREERTOS_DIR)/croutine.c

# Sources for UART driver
SRCS += $(UART_DRIVER)/uart_hal.c

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
LDFLAGS = -T linker1.ld -Wl,-Map=output.map -Wl,-gc-sections -nostartfiles -lc

# Object file
OBJS = $(ASMS:.S=.o) $(SRCS:.c=.o) 

.PHONY: all clean flash

all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
	@echo "Linking $@"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@ -lgcc
	@$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	@echo "Generating $@"
	@esptool --chip esp32c3 elf2image --flash_mode dio --flash_freq 40m --flash_size 4MB -o $@ $<
flash: $(TARGET).bin
	@echo "Flashing to ESP32-C3..."
	esptool.py --chip esp32c3 --port /dev/cu.usbmodem* --baud 921600 write_flash 0x0 $(TARGET).bin 
	
monitor: 
	screen /dev/cu.usbmodem* 115200


%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	@echo "Assembling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

debug:
	$(GDB) -ex "target extended-remote :3333" -ex "load" freertos_esp32c3.elf
# esptool for direct flash on memory
#flash: $(TARGET).bin
#	esptool.py --chip esp32c3 write_flash 0x0 $(TARGET).bin
