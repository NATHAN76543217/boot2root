#! /bin/bash

echo "Preparing the environnement..."

# init WIFI
WIFI_SSID="42 Lyon Guests"
WIFI_PASSWORD="Welcome42Lyon"
WIFI_IF="en1"

echo "Enabling WIFI..."
networksetup -setairportpower $WIFI_IF on
echo "Connecting to $WIFI_SSID..."
networksetup -setairportnetwork $WIFI_IF "$WIFI_SSID" "$WIFI_PASSWORD"

echo "Initialisation done."

# TODO enable the two VM and start the initialisation script in kali.
