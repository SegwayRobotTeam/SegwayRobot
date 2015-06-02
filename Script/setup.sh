#!/bin/bash

if [ "$(whoami)" != "root" ] ; then
  echo "=> You must be root to continue"
  exit 1
fi

printstatus()
{
  if [ $1 -ne 0 ] ; then
    echo "--> Error!"
    exit 1
  else
    echo "--> Done!"
  fi
}

overlay_OK()
{
  OVERLAY_PATH  = "/lib/firmware"
  OVERLAY_ARRAY = [ cape-universal-00A0.dtbo cape-univ-hdmi-00A0.dtbo   ]
  echo "--> Checking if config-pin in '$PATH'"
  if [ $(which config-pin) -ne 0 ] ; then
    printstatus 1
  fi
}

#
# For connection sharing
#
echo "=> Adding the new 192.168.7.1 default route"
if [ $(route -n | awk '{ print($2) }' | grep 192.168.7.1 | wc -l) -lt 1 ] ; then
  route add default gw 192.168.7.1
  printstatus $?
else
  printstatus 0
fi

echo "=> Adding a DNS server to resolv.conf"
if [ $(cat /etc/resolv.conf | grep 8.8.4.4 | wc -l) -lt 1 ] ; then
  echo "nameserver 8.8.4.4" >> /etc/resolv.conf
  printstatus $?
else
  printstatus 0
fi

echo "=> Checking connection"
ping -c3 www.google.com &>/dev/null
printstatus $?

echo "=> Updating time via ntp"
ntpdate -b -s -u pool.ntp.org
printstatus $?

# N.B. The HDMI must be disabled in uEnv.txt
#echo "=> Applying overlay to map the gpio"
#if [ overlay_OK  ] ; then
#  config-pin -f ${PATH_OVERLAY_CONF}/segway.bbio
#  printstatus $?
#fi

echo "=> Success"

echo ""
echo "=> Who are you?"
echo "--> 1) Davide"
echo "--> 2) Diego"
echo "--> 3) Giovanni"
echo "--> 4) Roberto"
echo ""
read -p "User Number: " Nuser
echo ""

if [[ $Nuser -gt 0 && $Nuser -lt 5 ]] ; then
  case $Nuser in
    2) FolderHost="/home/diego/SegwayNFS"
       FolderBBB="/home/ubuntu/SharedNFS/diego"
    ;;
    *) echo "If you read me, something is wrong. Error!"
       exit 1
    ;;
  esac
fi

echo "=> Mounting configured NFS folder"
if [ $(df | grep "${FolderBBB}" | wc -l) -lt 1 ] ; then
  mount -t nfs -o proto=tcp,port=2049 192.168.7.1:$FolderHost $FolderBBB
  printstatus $?
else
  printstatus 0
fi

echo "=> Success. Exiting."
exit 0
