#Capes
Files taken from the [official kernel source directory](https://github.com/beagleboard/linux/tree/3.8/firmware/capes) .

## Build
###Update the dtc package
In order to build the dts into dtbo files you need a patched version of the device tree compiler (dtc):

	wget -c https://github.com/RobertCNelson/tools/blob/master/pkgs/dtc.sh
	chmod +x dtc.sh
	./dtc.sh

###Build the capes

	./../Script/buildcapes.sh

You can find the capes in the `Capes/dtbo` directory.
###Install the capes
As root:

	cp dtbo/*.dtbo /lib/firmware

###Resources:
http://derekmolloy.ie/gpios-on-the-beaglebone-black-using-device-tree-overlays/
https://learn.adafruit.com/introduction-to-the-beaglebone-black-device-tree?view=all
http://elinux.org/BeagleBone_and_the_3.8_Kernel
http://papermint-designs.com/community/node/391

