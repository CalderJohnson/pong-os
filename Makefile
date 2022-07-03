C_SOURCES = $(wildcard kernel/*.c drivers/*.c x86/*.c)
HEADERS   = $(wildcard kernel/*.h drivers/*.h x86/*.h)
OBJ       = ${C_SOURCES:.c=.o x86/interrupt.o}
CC        = /usr/bin/clang
GDB       = /usr/bin/gdb
CFLAGS    = --target=x86_64 -g -ffreestanding -Wall -Wextra -m32

os-image.bin: boot/boot.bin kernel.bin
	cat $^ > os-image.bin

kernel.bin: boot/entry.o ${OBJ}
	ld.lld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/entry.o ${OBJ}
	ld.lld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-x86_64 -fda os-image.bin

debug: os-image.bin kernel.elf
	qemu-system-x86_64 -s -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin boot/*.o x86/*.o drivers/*.o