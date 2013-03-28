obj-m := hello.o

NODE=/dev/keylog

all:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules

install:
	insmod hello.ko

dev:
	mknod $(NODE) c 60 0

uninstall:
	rmmod hello
	rm -rf $(NODE)

clean:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean
