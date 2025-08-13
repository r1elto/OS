CROSS = /c/msys64/home/r1elto/cross/bin
CC = $(CROSS)/i386-elf-gcc
AS = $(CROSS)/i386-elf-as
LD = $(CROSS)/i386-elf-ld

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32

objects = loader.o gdt.o interrupts_stubs.o interrupts.o port.o kernel.o

all : run

%.o: %.cpp
	$(CC) $(GCCPARAMS) -c -o $@ $<

loader.o: loader.s
	$(AS) $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	$(LD) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	  mkdir -p iso/boot/grub
	  cp $< iso/boot/
	  echo 'set timeout=0' > iso/boot/grub/grub.cfg
	  echo 'set default=0' >> iso/boot/grub/grub.cfg
	  echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	  echo '    multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	  echo '    boot' >> iso/boot/grub/grub.cfg
	  echo '}' >> iso/boot/grub/grub.cfg
	  grub-mkrescue --output=$@ iso
	  rm -rf iso

run: mykernel.bin
	qemu-system-i386  -kernel mykernel.bin

.PHONY: clean
clean: 
		 rm -f $(objects) mykernel.bin mykernel.iso


