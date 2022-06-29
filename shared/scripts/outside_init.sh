#! /bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo "Preparing the environnement..."

# init WIFI
WIFI_SSID="42 Lyon Guests"
WIFI_PASSWORD="Welcome42Lyon"
WIFI_IF="en1"

echo "Enabling WIFI..."
networksetup -setairportpower $WIFI_IF on
echo "Connecting to $WIFI_SSID..."
networksetup -setairportnetwork $WIFI_IF "$WIFI_SSID" "$WIFI_PASSWORD"

# Starting virtual machines
echo "Starting the Boot2root virtual machine..."
VBoxManage startvm "Boot2root" --type headless


echo "Starting the kali linux virtual machine..."
VBoxManage startvm "kali" --type headless

echo -e "$GREEN Initialisation done.$NC"

KALIIP=$(VBoxManage guestproperty enumerate kali | grep IP | cut -d ':' -f 3 | cut -d ',' -f 1 | tr -d '\n' | tr -d ' ')
echo -e "The$RED IP address$NC of kali is: $KALIIP"

echo "Copy scripts into the shared volume."
cp -r scripts shared/

# Start exploitattion

echo "Run the exploitation script inside the kali VM..."
# ssh -t kali@$KALIIP 'echo \"kali1\" | sudo -S /home/kali/shared/scripts/exploit.sh'
ssh -t "kali@$KALIIP" 'echo kali1 | sudo -S ./shared/scripts/exploit.sh'