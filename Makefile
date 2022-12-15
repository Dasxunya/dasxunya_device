obj-m += driver.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	insmod ./driver.ko

unload:
	rmmod ./driver.ko

build1:
	gcc -c user.c

build2:
	gcc user.o -o main