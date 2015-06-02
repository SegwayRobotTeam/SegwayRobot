#SegwayRobot

## Hardware Configuration
| Component               | Device name and link                                                                     |
|-------------------------|------------------------------------------------------------------------------------------|
| Main Board              | BeagleBone Black revC                                                                    |
| IMU                     | [AltIMU-10 v4 (L3GD20H, LSM303D and LPS25H Carrier)](https://www.pololu.com/product/2470)|
| Motors                  | [Metal Gearmotor 25Dx52L HP with 48CPR Encoder](https://www.pololu.com/product/2273)     |
| Motor Driver            | [DRV8833](https://www.pololu.com/product/2130)                                           |
| Current Sensors         |  [ACS714](https://www.pololu.com/product/1185)                                           |
| Wheels                  | [Pololu Wheel 70x8mm](https://www.pololu.com/product/1425)                               |
| Mounting Hub            | [4mm shaft](https://www.pololu.com/product/1081)                                         |
| Metal Gearmotor Bracket | [25D mm](https://www.pololu.com/product/2676)                                            |
| Protocape               | [By Sparkfun](https://www.sparkfun.com/products/12774)                                   |

##Configure the segway
To set up the project, connect via ssh to the BeagleBone Black and follow the next steps

###1) Prerequisites
We need the following tools / libraries to build the code:

* Linux Kernel 3.8
* gcc (build-essential)
* [Eigen/Dense](http://eigen.tuxfamily.org/index.php?title=Main_Page)

###2) Clone the tree

	git clone --recursive https://github.com/SegwayRobotTeam/SegwayRobot.git

From now we assume to be positioned inside the SegwayRobot/ folder.

###3) Build the code

	cd Code/
	make balance

###4) Setup the overlays
####4a) Install the right device tree compiler version
	
	cd /tmp
	wget -c https://raw.githubusercontent.com/RobertCNelson/tools/master/pkgs/dtc.sh
	chmod +x dtc.sh
	sudo ./dtc.sh

####4b) Build and install the overlays

	cd Script/
	./buildcapes.sh
	cd ../capes/dtbo
	sudo cp *.dtbo /lib/firmware

####4c) Disable the HDMI cape
Edit `/boot/uEnv.txt` and add the following line:

	capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN

Then reboot

####4d) Load the required overlays

	cd Scripts/
	sudo ./loadvirtualcapes.sh

###5) Launch the balance program

	cd Code/
	sudo ./balance
