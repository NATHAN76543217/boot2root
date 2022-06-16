
# init interface
IF="eth0"

echo "auto eth0\niface $IF inet dhcp" >> /etc/network/interfaces
ifdown $IF
ifup $IF

# First get our IP address

LHOST=$(ip a | grep inet | grep eth0 | cut -f 6 -d ' ' | cut -f 1 -d '/' | tr -d $\"\n\")

echo "\$LHOST = $LHOST on $IF";

# Then locate the IP of the boot2root VM.
echo "# Searching for target...";

nmap $LHOST/24 -p80,22 -vv -o network_detection -e eth0 --script=http-title 

#RHOST=$(cat network_detection | grep "title" | grep "Hack me" -B 2 | cut -f1 -d $'\n' | rev | cut -f 1 -d ' ' | cut -f 2 -d '/' | rev | tr -d $'\n')
RHOST=$(cat network_detection | grep "Nmap scan report" -A 8 | grep "title: Hack" -B 7 | grep "Nmap scan report for" | rev | cut -d ' ' -f 1 | rev | tr -d '\n')

echo "RHOST = $RHOST on $IF"

# First scan the target with nmap
nmap -p- -A -T 4 -sV -O -oN nmap_scan_norm -oX nmap_scan_xml --script=all $RHOST



