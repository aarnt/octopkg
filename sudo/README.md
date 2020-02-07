## OCTOPKG-SUDO

This is a clone of LXQt sudo tool (without LXQt libs). It is the *ONLY* priviledge escalation tool supported by OctoPkg.
It needs sudo to work, so your user or group must be part of sudoers.

Steps to build:

```
$ /usr/local/lib/qt5/bin/qmake
$ make
# make install
```
