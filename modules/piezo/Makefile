ADB := /usr/bin/adb

obj-m := dd_piezo.o

KDIR := /home/youngjae/edk9/linux

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

install: $(obj-m)
	$(ADB) -d push $(^:.o=.ko) /
	$(ADB) -d shell chmod 644 /$(^:.o=.ko)
	$(ADB) -d shell toolbox rmmod $(^:.o=) > /dev/null
	$(ADB) -d shell insmod /$(^:.o=.ko)
	$(ADB) -d shell lsmod | grep $(^:.o=)

clean:
	rm -f *.symvers
	rm -f *.ko	
	rm -f *.o
	rm -f *.mod.c
	rm -f *.order
	rm -f .*.cmd
	rm -f .tmp_versions

