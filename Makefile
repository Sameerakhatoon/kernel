# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Source Files
BOOT_SRC := $(SRC_DIR)/boot/boot.asm
BOOT_BIN := $(BIN_DIR)/boot.bin
KERNEL_ASM_SRC := $(SRC_DIR)/kernel.asm
KERNEL_C_SRC := $(SRC_DIR)/kernel.c
SERIAL_C_SRC := $(SRC_DIR)/serial.c
IDT_ASM_SRC := $(SRC_DIR)/idt/idt.asm
IDT_C_SRC := $(SRC_DIR)/idt/idt.c
MEMORY_C_SRC := $(SRC_DIR)/memory/memory.c
IO_C_SRC := $(SRC_DIR)/io/io.c
IO_ASM_SRC := $(SRC_DIR)/io/io.asm
HEAP_C_SRC := $(SRC_DIR)/memory/heap/heap.c
KERNEL_HEAP_C_SRC := $(SRC_DIR)/memory/heap/kernel_heap.c
PAGING_C_SRC := $(SRC_DIR)/paging/paging.c
PAGING_ASM_SRC := $(SRC_DIR)/paging/paging.asm
DISK_C_SRC := $(SRC_DIR)/disk/disk.c
DISK_ASM_SRC := $(SRC_DIR)/disk/disk.asm
FS_ASM_SRC := $(SRC_DIR)/fs/fs.asm
FS_C_SRC := $(SRC_DIR)/fs/fs.c
PATH_PARSER_C_SRC := $(SRC_DIR)/fs/path_parser.c
STRING_C_SRC := $(SRC_DIR)/string/string.c
STREAMER_C_SRC := $(SRC_DIR)/disk/streamer.c
FILE_C_SRC := $(SRC_DIR)/fs/file.c
FAT_C_SRC := $(SRC_DIR)/fs/fat/fat16.c

# Object Files
KERNEL_ASM_OBJ := $(BUILD_DIR)/kernel.asm.o
KERNEL_C_OBJ := $(BUILD_DIR)/kernel.o
SERIAL_C_OBJ := $(BUILD_DIR)/serial.o
IDT_ASM_OBJ := $(BUILD_DIR)/idt/idt.asm.o
IDT_C_OBJ := $(BUILD_DIR)/idt/idt.o
MEMORY_C_OBJ := $(BUILD_DIR)/memory/memory.o
IO_ASM_OBJ := $(BUILD_DIR)/io/io.asm.o
HEAP_C_OBJ := $(BUILD_DIR)/memory/heap/heap.o
KERNEL_HEAP_C_OBJ := $(BUILD_DIR)/memory/heap/kernel_heap.o
PAGING_C_OBJ := $(BUILD_DIR)/paging/paging.o
PAGING_ASM_OBJ := $(BUILD_DIR)/paging/paging.asm.o
DISK_C_OBJ := $(BUILD_DIR)/disk/disk.o
DISK_ASM_OBJ := $(BUILD_DIR)/disk/disk.asm.o
FS_ASM_OBJ := $(BUILD_DIR)/fs/fs.asm.o
FS_C_OBJ := $(BUILD_DIR)/fs/fs.o
PATH_PARSER_C_OBJ := $(BUILD_DIR)/fs/path_parser.o
STRING_C_OBJ := $(BUILD_DIR)/string/string.o
STREAMER_C_OBJ := $(BUILD_DIR)/disk/streamer.o
FILE_C_OBJ := $(SRC_DIR)/fs/file.o
FAT_C_OBJ := $(BUILD_DIR)/fs/fat/fat16.o

# Kernel & OS binaries
KERNEL_FULL_OBJ := $(BUILD_DIR)/kernelfull.o
KERNEL_BIN := $(BIN_DIR)/kernel.bin
OS_BIN := $(BIN_DIR)/os.bin

# Linker Script
LINKER_SCRIPT := $(SRC_DIR)/linker.ld

# Compiler & Tools
NASM := nasm
LD := i686-elf-ld
GCC := i686-elf-gcc
QEMU := qemu-system-i386
GDB := gdb
RM := rm -rf

# Compiler Flags
INCLUDES := -I$(SRC_DIR) -I$(SRC_DIR)/idt -I$(SRC_DIR)/memory -I$(SRC_DIR)/io -I$(SRC_DIR)/memory/heap -I$(SRC_DIR)/paging -I$(SRC_DIR)/disk -I$(SRC_DIR)/fs -I$(SRC_DIR)/string
FLAGS := -g -ffreestanding -Wall -O0 -nostdlib -nostartfiles -nodefaultlibs

# Files to compile
FILES := $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(SERIAL_C_OBJ) $(IDT_ASM_OBJ) $(IDT_C_OBJ) $(MEMORY_C_OBJ) $(IO_ASM_OBJ) $(HEAP_C_OBJ) $(KERNEL_HEAP_C_OBJ) $(PAGING_C_OBJ) $(PAGING_ASM_OBJ) $(DISK_C_OBJ) $(DISK_ASM_OBJ) $(FS_ASM_OBJ) $(FS_C_OBJ) $(PATH_PARSER_C_OBJ) $(STRING_C_OBJ) $(STREAMER_C_OBJ) $(FILE_C_OBJ) $(FAT_C_OBJ)

# Default target
all: $(OS_BIN)

# OS binary linking
$(OS_BIN): $(BOOT_BIN) $(KERNEL_BIN)
	$(RM) $(OS_BIN)
	dd if=$(BOOT_BIN) of=$(OS_BIN) conv=notrunc
	dd if=$(KERNEL_BIN) of=$(OS_BIN) conv=notrunc oflag=append
	# dd if=/dev/zero bs=512 count=100 >> $(OS_BIN)
	dd if=/dev/zero bs=1048576 count=16 >> $(OS_BIN) # 16 mb padding, linux can store the file data in here
	sudo mount -t vfat $(OS_BIN) /mnt/d
	sudo cp ./hello.txt /mnt/d
	sudo umount /mnt/d

# Build Bootloader
$(BOOT_BIN): $(BOOT_SRC)
	mkdir -p $(BIN_DIR)
	$(NASM) -f bin $< -o $@

# Compile kernel.asm
$(KERNEL_ASM_OBJ): $(KERNEL_ASM_SRC)
	mkdir -p $(BUILD_DIR)
	$(NASM) -f elf -g $< -o $@

# Compile kernel.c
$(KERNEL_C_OBJ): $(KERNEL_C_SRC)
	mkdir -p $(BUILD_DIR)
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile serial.c
$(SERIAL_C_OBJ): $(SERIAL_C_SRC)
	mkdir -p $(BUILD_DIR)
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile IDT Assembly
$(IDT_ASM_OBJ): $(IDT_ASM_SRC)
	mkdir -p $(BUILD_DIR)/idt
	$(NASM) -f elf -g $< -o $@

# Compile IDT C
$(IDT_C_OBJ): $(IDT_C_SRC)
	mkdir -p $(BUILD_DIR)/idt
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile Memory C
$(MEMORY_C_OBJ): $(MEMORY_C_SRC)
	mkdir -p $(BUILD_DIR)/memory
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile Heap C
$(HEAP_C_OBJ): $(HEAP_C_SRC)
	mkdir -p $(BUILD_DIR)/memory/heap
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile Kernel Heap C
$(KERNEL_HEAP_C_OBJ): $(KERNEL_HEAP_C_SRC)
	mkdir -p $(BUILD_DIR)/memory/heap
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile Paging C
$(PAGING_C_OBJ): $(PAGING_C_SRC)
	mkdir -p $(BUILD_DIR)/paging
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile Paging Assembly
$(PAGING_ASM_OBJ): $(PAGING_ASM_SRC)
	mkdir -p $(BUILD_DIR)/paging
	$(NASM) -f elf -g $< -o $@

# Compile Disk C
$(DISK_C_OBJ): $(DISK_C_SRC)
	mkdir -p $(BUILD_DIR)/disk
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Compile Disk Assembly
$(DISK_ASM_OBJ): $(DISK_ASM_SRC)
	mkdir -p $(BUILD_DIR)/disk
	$(NASM) -f elf -g $< -o $@

# Compile IO Assembly
$(IO_ASM_OBJ): $(IO_ASM_SRC)
	mkdir -p $(BUILD_DIR)/io
	$(NASM) -f elf -g $< -o $@

# Compile new source files
$(FS_ASM_OBJ): $(FS_ASM_SRC)
	mkdir -p $(BUILD_DIR)/fs
	$(NASM) -f elf -g $< -o $@

$(FS_C_OBJ): $(FS_C_SRC)
	mkdir -p $(BUILD_DIR)/fs
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

$(PATH_PARSER_C_OBJ): $(PATH_PARSER_C_SRC)
	mkdir -p $(BUILD_DIR)/fs
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

$(FILE_C_OBJ): $(FILE_C_SRC)
	mkdir -p $(BUILD_DIR)/fs
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

$(STREAMER_C_OBJ): $(STREAMER_C_SRC)
	mkdir -p $(BUILD_DIR)/disk
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

$(STRING_C_OBJ): $(STRING_C_SRC)
	mkdir -p $(BUILD_DIR)/string
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

$(FAT_C_OBJ): $(FAT_C_SRC)
	mkdir -p $(BUILD_DIR)/fs/fat
	$(GCC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

# Link kernel objects
$(KERNEL_BIN): $(FILES)
	mkdir -p $(BIN_DIR)
	$(LD) -g -relocatable $(FILES) -o $(KERNEL_FULL_OBJ)
	$(GCC) -T $(LINKER_SCRIPT) -o $(KERNEL_BIN) -ffreestanding -O0 -nostdlib $(KERNEL_FULL_OBJ)

# Run in QEMU
run: $(OS_BIN)
	$(QEMU) -nographic -serial mon:stdio -drive format=raw,file=$(OS_BIN)

# Debugging
debug: $(OS_BIN)
	$(QEMU) -S -gdb tcp::1234 -nographic -serial mon:stdio -drive format=raw,file=$(OS_BIN)

gdb_debug:
	$(GDB) -ex "set architecture i386" \
	       -ex "add-symbol-file $(KERNEL_FULL_OBJ) 0x100000" \
	       -ex "target remote localhost:1234" \
	       -ex "break _start"

# Clean
clean:
	$(RM) $(BIN_DIR)/*.bin $(BUILD_DIR)/*.o $(BUILD_DIR)/idt/*.o $(BUILD_DIR)/memory/*.o $(BUILD_DIR)/io/*.o $(BUILD_DIR)/paging/*.o $(BUILD_DIR)/disk/*.o $(KERNEL_FULL_OBJ) $(BUILD_DIR)/memory/heap/*.o

# xxd
xxd:
	xxd -b $(OS_BIN) | less

# hexdump
hexdump:
	hexdump -C $(OS_BIN) | less