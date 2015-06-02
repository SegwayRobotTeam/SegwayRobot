#!/bin/bash

if [ "$(whoami)" != "root" ] ; then
  echo "=> You must be root to continue"
  exit 1
fi

echo "=> Adding the forwarding rule to iptables"
iptables -A POSTROUTING -t nat -j MASQUERADE

echo "=> Enabling IP forward"
echo 1 > /proc/sys/net/ipv4/ip_forward

echo "=> Accessing BBB via ssh"
ssh ubuntu@192.168.7.2
