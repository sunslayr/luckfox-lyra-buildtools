SOURCEFILE = dt_i2c
KERNELSRC = ../lyra-sdk/kernel-6.1/

obj-m += $(SOURCEFILE).o

all: module dt
	echo Built kernel module

module:
	make -C $(KERNELSRC) M=$(PWD) modules

clean:
	make -C /root/kernel-build/ M=$(PWD) clean
