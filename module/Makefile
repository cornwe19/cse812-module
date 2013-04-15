obj-m := hello.o

NODE=/dev/keylog
TTYNODE=/dev/ttylog

all:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules

install:
	mknod $(NODE) c 65 0
	mknod $(TTYNODE) c 66 0
	chmod 666 $(NODE)
	chmod 666 $(TTYNODE)
	insmod hello.ko

uninstall:
	rmmod hello
	rm -rf $(NODE)
	rm -rf $(TTYNODE)

clean:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean

rmfiles:
	rm -rf $(NODE)
	rm -rf $(TTYNODE)

i:
	rmmod hello
	rm -rf $(NODE)
	rm -rf $(TTYNODE)
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules
	mknod $(NODE) c 65 0
	mknod $(TTYNODE) c 66 0
	chmod 666 $(NODE)
	chmod 666 $(TTYNODE)
	insmod hello.ko
