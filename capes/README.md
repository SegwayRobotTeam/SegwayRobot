#Capes
Files taken from the [official kernel source directory](https://github.com/beagleboard/linux/tree/3.8/firmware/capes) .

## Build
###Update the dtc package
In order to build the dts into dtbo files you need a patched version of the device tree compiler (dtc):

	cd /tmp
	wget -c https://raw.githubusercontent.com/RobertCNelson/tools/master/pkgs/dtc.sh
	chmod +x dtc.sh
	sudo ./dtc.sh

###Build the capes

	cd Script/
	./buildcapes.sh

You can find the capes in the `capes/dtbo` directory.
###Install the capes
As root:

	cd ../capes/dtbo
	sudo cp *.dtbo /lib/firmware

###Resources:
1. http://derekmolloy.ie/gpios-on-the-beaglebone-black-using-device-tree-overlays/
2. https://learn.adafruit.com/introduction-to-the-beaglebone-black-device-tree?view=all
3. http://elinux.org/BeagleBone_and_the_3.8_Kernel
4. http://papermint-designs.com/community/node/391

