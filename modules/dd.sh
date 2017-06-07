cd 7segment
make
make install
cd ..
cd btnswitch 
make
make install
cd ..
cd led
make
make install
cd ..
cd piezo
make
make install
cd ..
cd touch
make
make install
cd ..
cd vib
make
make install
cd ..
cd textlcd
make
make install
cd ..
adb push mknod.sh /
