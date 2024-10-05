Compile with:
```
make
```
	
Pass the **/dev/hidrawX** device path to the binaries. Don't forget to use **sudo** (or any other way to get privileges) if not root user.

Write period is configurable (in msecs). E.g:
```
sudo ./hid_write /dev/hidraw2 100
```
