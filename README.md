## This is OctoPkg, a powerful pkgng front end using Qt libs.

![Main window](https://raw.githubusercontent.com/aarnt/octopkg/master/octopkg-mainwindow.png)

**OctoPkg** is a Qt based GUI front end to the [pkg](https://wiki.freebsd.org/pkg) package manager, derived from [Octopi](http://tintaescura.com/projects/octopi).
It consists of a lxqt-sudo clone called [qt-sudo](https://github.com/aarnt/qt-sudo/) used to gain root privileges and a package browser application used
to search, install, remove and update packages.
The project is compatible with [FreeBSD](https://www.freebsd.org/), [DragonFly BSD](https://www.dragonflybsd.org/), [GhostBSD](https://ghostbsd.org/) and [HardenedBSD](https://hardenedbsd.org/).

### You can use pkg to install the latest OctoPkg version available in your BSD:

```
# pkg install octopkg
```

### Follow the steps bellow to compile the latest source code (you'll need curl, git and qt5 packages):

```
$ git clone https://github.com/aarnt/octopkg
$ cd octopkg
$ /usr/local/lib/qt5/bin/qmake
$ make
# make install
```

### In order to run OctoPkg:

```
$ octopkg
```

Remember: OctoPkg needs doas or sudo tool to work.


Enjoy!
