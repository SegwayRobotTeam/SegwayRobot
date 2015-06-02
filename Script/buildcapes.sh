#!/bin/bash

capesdir="../capes"
outputdir="$capesdir/dtbo"

#
# Removes the suffix .dts and builds the dtbo in the dtbo folder
#
for i in $(ls -1 $capesdir | grep dts) ; do
	filename=$(basename "$i" ".dts")
	echo "Building $i.dts" 
	dtc -O dtb -o $outputdir/$filename.dtbo -@ $capesdir/$filename.dts
done

echo "Success"
