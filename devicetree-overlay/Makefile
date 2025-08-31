dtbo = testoverlay
KERNELSRC = ../lyra-sdk/kernel-6.1/

all: dt
	echo Built Device Tree Overlay

dt: testoverlay.dts
	cpp  -I $(KERNELSRC)/include -I $(KERNELSRC)/arch/arm/boot/dts -undef -x assembler-with-cpp $(dtbo).dts $(dtbo).preproccessed
	dtc -@ -I dts -O dtb -i $(KERNELSRC)/arch/arm/boot/dts -o $(dtbo).dtbo $(dtbo).preproccessed

clean:
	rm -rf $(dtbo).dtbo
	rm -rf $(dtbo).preproccessed

install: dt
	mkdir -p /sys/kernel/config/device-tree/overlays/testoverlay
	cat testoverlay.dtbo > /sys/kernel/config/device-tree/overlays/testoverlay/dtbo
	echo 1 > /sys/kernel/config/device-tree/overlays/testoverlay/status
	cat /sys/kernel/config/device-tree/overlays/testoverlay/status

remove:
	rmdir /sys/kernel/config/device-tree/overlays/testoverlay
