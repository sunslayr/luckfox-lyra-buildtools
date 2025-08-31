dtb = rk3506g-luckfox-lyra-picocalc
KERNELSRC = ../lyra-sdk/kernel-6.1/

all: dt
	echo Built Device Tree

dt: $(dtb).dts
	cpp  -I $(KERNELSRC)/include -I $(KERNELSRC)/arch/arm/boot/dts -undef -x assembler-with-cpp $(dtb).dts $(dtb).preproccessed
	dtc -@ -I dts -O dtb -i $(KERNELSRC)/arch/arm/boot/dts -o $(dtb).dtb $(dtb).preproccessed

clean:
	rm -rf $(dtb).dtbo
	rm -rf $(dtb).preproccessed
