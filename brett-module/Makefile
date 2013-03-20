obj-m := hello.o

all:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules

install:
	modprobe -qr snd_intel8x0 # Yeah, we're stomping on the intel driver code for now...
	insmod hello.ko

uninstall:
	rmmod hello
	# TODO: Reinstantiate the intel drivers? I guess we can't hear them anyway, so who cares? 

clean:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean
