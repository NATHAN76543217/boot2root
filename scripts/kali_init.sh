#! /bin/bash

IF="eth0"

# init interface
echo "auto eth0\niface $IF inet dhcp" >> /etc/network/interfaces
ifdown $IF
ifup $IF

echo "Init done" > /home/kali/test_success
