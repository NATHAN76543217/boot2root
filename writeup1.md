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
