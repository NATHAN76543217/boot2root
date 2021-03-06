#! /bin/bash

OUTPUT_FOLDER="shared/dumps"
IF="eth0"

# init interface
echo "auto eth0\niface $IF inet dhcp" >> /etc/network/interfaces
ifdown $IF
ifup $IF

# First get our IP address

LHOST=$(ip a | grep inet | grep eth0 | cut -f 6 -d ' ' | cut -f 1 -d '/' | tr -d $\"\n\")

echo "\$LHOST = $LHOST on $IF";

# Then locate the IP of the boot2root VM.
echo "# Searching for target...";
NETWORK_SCAN_OUTPUT="target_detection"
nmap $LHOST/24 -p80 -vv -o "$NETWORK_SCAN_OUTPUT" -e eth0 --script=http-title #TODO optimiser la requete et changer la commade ci desosus pour matcher la nouvelle reponse

#RHOST=$(cat network_detection | grep "title" | grep "Hack me" -B 2 | cut -f1 -d $'\n' | rev | cut -f 1 -d ' ' | cut -f 2 -d '/' | rev | tr -d $'\n')
RHOST=$(cat $NETWORK_SCAN_OUTPUT | grep "Nmap scan report" -A 8 | grep "title: Hack" -B 7 | grep "Nmap scan report for" | rev | cut -d ' ' -f 1 | rev | tr -d '\n')

echo "RHOST = $RHOST on $IF"

# First scan the target with nmap
NMAP_OUTPUT_NORMAL="nmap_scan_norm"
NMAP_OUTPUT_XML="nmap_scan_xml"

nmap -p- -A -T 4 -sV -O -oN "$OUTPUT_FOLDER/$NMAP_OUTPUT_NORMAL" -oX "$OUTPUT_FOLDER/$NMAP_OUTPUT_XML" --script=all $RHOST


# Automatic detection of possible CVEs
echo "# Searching for CVEs";
searchsploit --id --nmap $NMAP_OUTPUT_XML -v > "$OUTPUT_FOLDER/cve_detection_result" 
# searchsploit --nmap nmap_scan_xml -v > "cve_detection_result"