#!/bin/bash

if [ "$(whoami)" != "root" ] ; then
  echo "=> You must be root to continue"
  exit 1
fi

#
# Define the capes to be loaded
#
capes=( bone_pwm_P8_34 \
        bone_pwm_P8_36 \
        bone_pwm_P8_45 \
        bone_pwm_P8_46 \
        bone_eqep0     \
        bone_eqep1     \
        bone_eqep2)

#
# Insert pwm_test module, needed by the eQEP driver
#
modprobe pwm_test

#
# Load the defined virtual capes
#
for i in ${capes[@]} ; do
	echo $i > /sys/devices/bone_capemgr*/slots 
done
