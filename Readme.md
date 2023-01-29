# Boot2Root

By Nlecaill

---------- 

```
PORT    STATE SERVICE  REASON  VERSION
21/tcp  open  ftp      syn-ack vsftpd 2.0.8 or later
22/tcp  open  ssh      syn-ack OpenSSH 5.9p1 Debian 5ubuntu1.7 (Ubuntu Linux; protocol 2.0)
80/tcp  open  http     syn-ack Apache httpd 2.2.22 ((Ubuntu))
143/tcp open  imap     syn-ack Dovecot imapd
443/tcp open  ssl/http syn-ack Apache httpd 2.2.22
993/tcp open  ssl/imap syn-ack Dovecot imapd
Service Info: Host: 127.0.1.1; OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

Username founded:
- admin     : Admin 	  	E-mail
- lmezard 	: User 	  	 
- qudevide 	: User 	  	 
- thor 	    : User 	  	 
- wandre 	: User 	  	 
- zaz 	    : User 	 

## Forum

Forum powered by `my little forum`.

## PHPmyAdmin

Version: 3.4.10.1

# Solutions

- Buffer overflow as zaz on exploit_me
- Kernel exploit: DirtyC0w
- GRUB init replacement
- 2 squashfs / bash history
- apache suExec

- Recon => site web => mail => DB identifiants => phpmyadmin => apache suEXEc => lmzard/ftp => ft_fun => puzzles

READMe
etape 1
scan => scan.md
siteweb

# Recon

## VM
- nmap IP
- nmap PORTS
## Web site
- nikto
- gobuster
- fichier de log ssh lmezard
- liste de user sur le forum
- /forum
- /phpmyadmin
- /webmail/src/index.php






# Penetration

## squirelmail

## PhpMyadmin


## suExec

## php backdoor








# Privileges escalation

## I) lmezard         ft_fun

## challenges chain
laurie          bomb
thor            turtle
zaz             exploit_me
root

## squashfs

scp filesystem.squashfs

mount -t squashfs

cat /rofs/root/.bash_history
zaz             exploit_me
root



## GRUB init

## Dirtycow