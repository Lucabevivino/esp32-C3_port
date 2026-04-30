TARGET = freertos_esp32c3

# Toolchain
TOOLCHAIN_BIN = /Users/lucabevivino/xpack_rv_gcc_15.2/bin/
CC      = $(TOOLCHAIN_BIN)riscv-none-elf-gcc
OBJCOPY = $(TOOLCHAIN_BIN)riscv-none-elf-objcopy
SIZE    = $(TOOLCHAIN_BIN)riscv-none-elf-size
GDB     = $(TOOLCHAIN_BIN)riscv-none-elf-gdb

# Esptool
ESPTOOL = esptool.py
PORT    = /dev/cu.usbmodem*
BAUD    = 921600

# --- FIX: DEFINIZIONE VARIABILI OUTPUT ---
TARGET_ELF      = freertos_esp32c3.elf
TARGET_PURE_ELF = freertos_esp32c3.pure.elf
TARGET_BIN      = freertos_esp32c3.bin
# -----------------------------------------

# Paths
FREERTOS_DIR     = ./FreeRTOS-Kernel
FREERTOS_PORT    = ./bare_metal_port
FREERTOS_MEMMANG = $(FREERTOS_DIR)/portable/MemMang
UART_DRIVER      = ./uart

# Headers
INC_DIRS += -I. -I$(FREERTOS_DIR)/include -I$(FREERTOS_PORT) -I$(UART_DRIVER)

# Sources
SRCS += $(FREERTOS_DIR)/tasks.c $(FREERTOS_DIR)/list.c $(FREERTOS_DIR)/queue.c \
        $(FREERTOS_DIR)/timers.c $(FREERTOS_DIR)/event_groups.c \
        $(FREERTOS_DIR)/stream_buffer.c $(FREERTOS_DIR)/croutine.c
SRCS += $(UART_DRIVER)/uart_hal.c
SRCS += $(FREERTOS_PORT)/port.c $(FREERTOS_PORT)/port_systimer.c
SRCS += $(FREERTOS_MEMMANG)/heap_4.c
SRCS += main.c
ASMS += startup.S $(FREERTOS_PORT)/port_asm.S

# Compile options
CFLAGS  = -march=rv32im_zicsr -mabi=ilp32 -g3 -O0
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -fmessage-length=0 $(INC_DIRS)

# Linker options
LDFLAGS = -T linker.ld -Wl,-Map=output.map -Wl,-gc-sections -nostartfiles -lc -lgcc

# Object files
OBJS = $(ASMS:.S=.o) $(SRCS:.c=.o)

.PHONY: all clean flash monitor debug

all: $(TARGET_BIN)

# 1. ELF Completo
$(TARGET_ELF): $(OBJS)
	@echo "Linking $@"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@$(SIZE) $@

# 2. ELF Pulito
$(TARGET_PURE_ELF): $(TARGET_ELF)
	@echo "Creating pure ELF: $@"
	@$(OBJCOPY) --strip-debug \
                --remove-section=.riscv.attributes \
                --remove-section=.comment \
                --remove-section=.note.GNU-stack \
                $< $@

# 3. Immagine BIN - FIX SINTASSI ESPTOOL v5.2
# Cambia la dipendenza: il BIN ora dipende dall'ELF completo
$(TARGET_BIN): $(TARGET_ELF)
	@echo "Generating $(TARGET_BIN) from full ELF"
	@$(ESPTOOL) --chip esp32c3 elf2image --flash_mode dio --flash_freq 40m --flash_size 4MB -o $@ $<
flash: $(TARGET_BIN)
	@echo "Writing to Flash..."
	@$(ESPTOOL) --chip esp32c3 --port $(PORT) --baud $(BAUD) write_flash 0x0 $(TARGET_BIN)

monitor:
	screen $(PORT) 115200

debug: $(TARGET_ELF)
	@echo "Starting GDB..."
	$(GDB) -ex "target extended-remote :3333" $<

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	@echo "Assembling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS) $(TARGET_ELF) $(TARGET_PURE_ELF) $(TARGET_BIN) output.map
