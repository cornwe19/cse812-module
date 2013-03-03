obj-m := hello.o

all:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) modules

clean:
	make -C /usr/src/linux-2.6.32.5/ M=$(PWD) clean
