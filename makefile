.PHONY: check run clean

default: run

clean:
	@rm boot.o
	@rm jethros.bin
	@rm kernel.o
	@rm kernel_asm.o

run: jethros.iso program.tar
	@rm serial.log -f
	@qemu-system-x86_64 -drive format=raw,file=jethros.iso,media=disk -drive media=disk,file=program.tar,format=raw -serial file:serial.log -m 2G 
#-drive media=disk,file=hdd.bin,format=raw 
#comp command ~/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib

program.tar: tardir makefile tarrdir/program.exe
	@tar --transform 's|^\./||' -cf program.tar -C tardir .

tardir/program.exe: tardir/program.asm
	nasm tardir/program.asm -o tardir/test.exe

boot.o: ./code/boot.s
	@~/opt/cross/bin/i686-elf-as ./code/boot.s -o boot.o

kernel.o: kernel.c headers.h ./c/drivers.c ./c/files.h ./c/gdt.c ./c/headers.h ./c/idt.c ./c/utils.c ./c/graphics.c ./c/interupts.c ./c/keyboard.c ./c/disk.c ./c/timer.c ./c/debug.c ./c/mem.c ./c/font.h ./c/program.c
	@~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

kernel_asm.o: ./code/kernel.asm ./code/isr.asm
	@nasm ./code/kernel.asm -o kernel_asm.o -f elf32

check: jethros.bin
	@grub-file --is-x86-multiboot jethros.bin || exit 1
	@echo VALID MULTIBOOT

jethros.bin: ./code/linker.ld boot.o kernel.o kernel_asm.o
	@~/opt/cross/bin/i686-elf-gcc -T ./code/linker.ld -o jethros.bin -ffreestanding -O2 -nostdlib boot.o kernel.o kernel_asm.o -lgcc

jethros.iso: check jethros.bin
	@cp jethros.bin ./isodir/boot/jethr.os
	@chronic grub-mkrescue -o jethros.iso isodir
	@#grub-mkimage -o jethros.img multiboot sh fat
	@#grub-mkimage -O i386-pc -o jethros.img -p isodir -v
