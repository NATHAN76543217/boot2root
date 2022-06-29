# /bin/bash

WIFI_IF="en1"

#Shutdown the two VMs
echo "Shutdown Boot2root..."
VBoxManage controlvm "Boot2root" poweroff

echo "Shutdown kali..."
VBoxManage controlvm "kali" poweroff

# Shutdown WIFI
echo "Disconnecting WIFI"
networksetup -setairportpower $WIFI_IF off

echo "The system is now off."