
CC = gcc
CFLAGS = -m32   -ffreestanding -fno-builtin  -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -Wno-unused-variable -c
LDFLAGS = -T src/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

VPATH = src:drivers

# get all .c files in src directory
C_SOURCES = $(wildcard src/*.c drivers/*.c prov_drivers/*.c)
# get all .s files in src directory
ASM_SOURCES = $(wildcard src/*.s drivers/*.s prov_drivers/*.s)





# add .o files to the list
OBJECTS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

io.o:
	nasm -f elf32 src/io.asm -o drivers/io.o

all: kernel.elf

kernel.elf: $(OBJECTS)
	echo "Linking..."
	echo "Objects: $(OBJECTS)"
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R 	-b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 	-A os 	-input-charset utf8 	-quiet 	-boot-info-table 	-o os.iso 	iso


run: os.iso
	qemu-system-i386 -nographic -boot d -monitor unix:/tmp/qemu-monitor,server,nowait -cdrom os.iso -m 32 -d cpu -D /dev/null #logQ.txt
	reset
run_log: os.iso
	qemu-system-i386 -nographic -boot d -monitor unix:/tmp/qemu-monitor,server,nowait -cdrom os.iso -m 32 -d cpu -D logQ.txt
	reset

telnet: os.iso
	qemu-system-i386 -curses -monitor telnet::42068,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32 -d cpu -D log.txt
	reset

serial: os.iso
	qemu-system-i386 -serial telnet::4321,server,nowait -curses -monitor telnet::42068,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32 -d cpu -D /dev/null
	reset 

tmux:
	./tmux.sh

logcpu: os.iso

	# if args are passed then use those instead

	# # set ports
	# tel_ser=4321
	# tel_qem=42068

	echo "Serial Port: $(tel_ser)"
	echo "Qemu Monitor Port: $(tel_qem)"

	qemu-system-i386 -serial telnet::4343,server,nowait -display curses -monitor telnet::4344,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32 -d cpu -D log.txt
	reset 


%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.cpp types.h
	$(CC) $(CFLAGS) $< -o $@
%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@
clean:
	rm -rf *.o kernel.elf os.iso 
	rm -rf src/*.o drivers/*.o



# all: iso

# build:
# 	nasm -f elf32 src/loader.asm -o src/loader.o
# 	ld -T ./src/link.ld -melf_i386 src/loader.o -o kernel.elf
# 	cp kernel.elf iso/boot/kernel.elf
# 	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

# run: build
# 	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu #-D logQ.txt
