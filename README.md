spe4k v0.1.0a
=======

spe4k (pronunced speak) is a multiplatform IRC client written with Qt.
The goal is to have a very simple and customizable client, in which you can add python scripts to customize its behaviour just like mIRC (but free).
For now, only the IRC part works:
* Basic IRC commands
* Basic options
* Network list
* Identities

If you want to join this project as a developer, feel free to contact me by e-mail (shaoner@gmail.com)

## Build

### Requirements

* qt >= 4.8
* qmake >= 2.01a

### Submodules

First, you have to fetch all the data from each submodule
<pre>
git submodule init
git submodule update
</pre>

### Compiling

Generate the makefile with qmake, and compile.
Note: you can do it with make -j<X> to compile faster.

#### For linux
<pre>
qmake-qt4 -r -spec linux-g++
make
</pre>

#### For Windows 32bits

Requires:
* mingw32-qmake-qt4
* mingw32-gcc-c++

<pre>
mingw-qmake-qt4 libspe4rc.pro -r
make
</pre>

#### For Windows 64bits

Requires:
* mingw64-qmake-qt4
* mingw64-gcc-c++

<pre>
mingw-qmake-qt4 libspe4rc.pro -r
make
</pre>

## License

GPL

## Icons

Fugue Icons: http://p.yusukekamiyamane.com/

## Authors

shaoner <shaoner@gmail.com>
