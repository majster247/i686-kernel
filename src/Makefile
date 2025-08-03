GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-rtti
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o gdt.o port.o interruptstubs.o interrupts.o kernel.o

%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

kernel.iso: kernel.bin
	@echo "Creating ISO image..."
	@mkdir -p ../iso/boot/grub
	@cp kernel.bin ../iso/boot/kernel.bin
	@echo 'set timeout=0' > ../iso/boot/grub/grub.cfg
	@echo 'set default=0' >> ../iso/boot/grub/grub.cfg
	@echo 'menuentry "Kernel" {' >> ../iso/boot/grub/grub.cfg
	@echo '    multiboot /boot/kernel.bin' >> ../iso/boot/grub/grub.cfg
	@echo '    boot' >> ../iso/boot/grub/grub.cfg
	@echo '}' >> ../iso/boot/grub/grub.cfg
	grub-mkrescue -o ../kernel.iso ../iso

testkernel: kernel.bin
	@echo "Test kernel on QEMU... with PowerShell by command: qemu-system-i386 -kernel kernel.bin"

testiso: kernel.iso
	@echo "Test ISO on QEMU... with PowerShell by command: qemu-system-i386 -cdrom kernel.iso"

run:kernel.iso
	@echo "Running kernel in QEMU..."
	qemu-system-i386 -cdrom kernel.iso

runVirtualBox: kernel.iso
	@echo "Running kernel in VirtualBox..."
	VBoxManage createvm --name "KernelVM" --register
	VBoxManage modifyvm "KernelVM" --memory 512 --acpi on --boot1 dvd
	VBoxManage storagectl "KernelVM" --name "IDE Controller" --add ide
	VBoxManage storageattach "KernelVM" --storagectl "IDE Controller" --port 0 --device 0 --type hdd --medium ../kernel.iso
	VBoxManage startvm "KernelVM"

runkernel: kernel.bin
	@echo "Running kernel in QEMU..."
	qemu-system-i386 -kernel kernel.bin

clean:
	@echo "Cleaning up..."
	@rm -f $(objects) kernel.bin
	@rm -rf ../iso
	@rm -f ../kernel.iso