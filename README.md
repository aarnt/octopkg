## This is OctoPkg, a powerful pkgng front-end using Qt libs.

![Main window](https://raw.githubusercontent.com/aarnt/octopkg/master/octopkg-mainwindow.png)

You'll need "git", "curl" and "qt5" packages in order to get latest dev version running!

To download and build the source code, type:

```
$ git clone https://github.com/aarnt/octopkg
$ cd octopkg/doas
$ /usr/local/lib/qt5/bin/qmake
$ make
# make install
$ cd ..
$ /usr/local/lib/qt5/bin/qmake
$ make
# make install
```

To run it:

```
$ octopkg
```

Remember: OctoPkg needs doas tool to work.

Enjoy!
