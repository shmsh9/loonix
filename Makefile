CC := clang
LD := lld
ARCH ?= x86-64
ifeq ($(ARCH),x86-64)
FW_BASE  = OVMF
FW_ARCH  = X64
include x86-64.env
else
ARCH      = aarch64
FW_BASE   = QEMU_EFI
FW_ARCH   = AA64
QEMU_OPTS = -M virt -cpu cortex-a57
include aarch64.env
endif
QEMU      = qemu-system-$(subst -,_,$(ARCH))
FW_ZIP    = $(FW_BASE)-$(FW_ARCH).zip

export

SRCS := bootloader/src/main.c bootloader/src/elf.c bootloader/src/stack.c bootloader/src/std.c #bootloader/src/shell.c

default: all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot.efi: bootloader/src/main.o bootloader/src/elf.o bootloader/src/stack.o bootloader/src/std.o #bootloader/src/shell.o
	$(LD) $(LDFLAGS) $^ -out:$@

kernel.elf: kernel/src/main.c
	$(CC) $(KERNEL_CFLAGS) -c $< -o kernel.o
	$(LD) $(KERNEL_LDFLAGS) kernel.o -o $@

-include $(SRCS:.c=.d)

.PHONY: clean all default

all: boot.efi kernel.elf

qemu: CFLAGS += -D_DEBUG
qemu: all $(FW_BASE)_$(FW_ARCH).fd image/efi/boot/boot$(ARCH).efi
	$(QEMU) $(QEMU_OPTS) -nographic -bios ./$(FW_BASE)_$(FW_ARCH).fd -s -net none -hda fat:rw:image
image/efi/boot/boot$(ARCH).efi: boot.efi
	mkdir -p image/efi/boot
	cp -f $< image/efi/boot/boot$(FW_ARCH).efi
	cp -f kernel.elf image/
$(FW_BASE)_$(FW_ARCH).fd:
	wget https://efi.akeo.ie/$(FW_BASE)/$(FW_ZIP)
	unzip $(FW_ZIP) $(FW_BASE).fd
	mv $(FW_BASE).fd $(FW_BASE)_$(FW_ARCH).fd
	rm $(FW_ZIP)

clean:
	rm -rf *.efi *.elf *.o *.d *.lib *.fd bootloader/src/*.o bootloader/src/*.d kernel/src/*.o kernel/src/*.d
