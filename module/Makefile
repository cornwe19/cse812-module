obj-m := hello.o

NODE=/dev/keylog

all:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules

install:
	mknod $(NODE) c 60 0
	chmod 666 $(NODE)
	insmod hello.ko

uninstall:
	rmmod hello
	rm -rf $(NODE)

clean:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean

i:
	rmmod hello
	rm -rf $(NODE)
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules
	mknod $(NODE) c 60 0
	insmod hello.ko
