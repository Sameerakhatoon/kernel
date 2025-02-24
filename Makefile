# Define directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Define files
BOOT_SRC := $(SRC_DIR)/boot/boot.asm
BOOT_BIN := $(BIN_DIR)/boot.bin
KERNEL_SRC := $(SRC_DIR)/kernel.asm
KERNEL_OBJ := $(BUILD_DIR)/kernel.asm.o
KERNEL_FULL_OBJ := $(BUILD_DIR)/kernelfull.o
KERNEL_BIN := $(BIN_DIR)/kernel.bin
OS_BIN := $(BIN_DIR)/os.bin
LINKER_SCRIPT := $(SRC_DIR)/linker.ld

# Define commands
NASM := nasm
LD := i686-elf-ld
GCC := i686-elf-gcc
NDISASM := ndisasm
QEMU := qemu-system-i386
DD := dd
GDB := gdb
RM := rm -rf

# Default target
all: $(OS_BIN)

# Create final OS binary
$(OS_BIN): $(BOOT_BIN) $(KERNEL_BIN)
	$(RM) $(OS_BIN)
	$(DD) if=$(BOOT_BIN) of=$(OS_BIN) conv=notrunc # Copy boot.bin to os.bin
	$(DD) if=$(KERNEL_BIN) of=$(OS_BIN) conv=notrunc oflag=append # Append kernel.bin to os.bin
	$(DD) if=/dev/zero bs=512 count=100 >> $(OS_BIN) # Pad os.bin with zeros

# Build the boot binary
$(BOOT_BIN): $(BOOT_SRC)
	mkdir -p $(BIN_DIR)
	$(NASM) -f bin $< -o $@

# Compile kernel.asm to an object file
$(KERNEL_OBJ): $(KERNEL_SRC)
	mkdir -p $(BUILD_DIR)
	$(NASM) -f elf -g $< -o $@

# Link kernel object to produce kernel binary
$(KERNEL_BIN): $(KERNEL_OBJ)
	mkdir -p $(BIN_DIR)
	$(LD) -g -relocatable $(KERNEL_OBJ) -o $(KERNEL_FULL_OBJ)
	$(GCC) -T $(LINKER_SCRIPT) -o $(KERNEL_BIN) -ffreestanding -O0 -nostdlib $(KERNEL_FULL_OBJ)

# Disassemble boot.bin
disassemble_boot: $(BOOT_BIN)
	$(NDISASM) $(BOOT_BIN)

# Disassemble kernel.bin
disassemble_kernel: $(KERNEL_BIN)
	$(NDISASM) $(KERNEL_BIN)

# Disassemble os.bin
disassemble_os: $(OS_BIN)
	$(NDISASM) $(OS_BIN)

# Run in QEMU
run: $(OS_BIN)
	$(QEMU) -nographic -drive format=raw,file=$(OS_BIN)

# Debugging with QEMU and GDB
debug: $(OS_BIN)
	$(QEMU) -S -gdb tcp::1234 -nographic -drive format=raw,file=$(OS_BIN)

# Launch GDB and connect to QEMU
gdb_debug:
	$(GDB) -ex "set architecture i386" \
	       -ex "add-symbol-file $(KERNEL_FULL_OBJ) 0x100000" \
	       -ex "target remote localhost:1234" \
	       -ex "break _start" \
	    #    -ex "continue"

# Clean up build files
clean:
	$(RM) $(BIN_DIR)/*.bin $(BUILD_DIR)/*.o

# Hexdump os.bin
hexdump: $(OS_BIN)
	hexdump -C $(OS_BIN) | less

# Using xxd
xxd: $(OS_BIN)
	xxd $(OS_BIN) | less
