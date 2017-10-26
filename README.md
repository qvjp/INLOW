## INLOW

INLOW is a small unix-like operating system. It includes monolithic kernel writen in C++, a standard C library and userspace written in C

-----------

## Build

#### Build Toolchain

`mkdir $HOME/src`

`mkdir $HOME/inlow-toolchain`

`export PATH="$HOME/inlow-toolchain/bin:$PATH"`

In INLOW's root dir: `INLOWROOT=$(pwd)`

In INLOW's root dir: `make install-headers`

##### Build binutils

`cd $HOME/src`

`git clone https://github.com/qvjp/binutils-inlow-2.29.1.git`

`cd binutils-inlow-2.29.1`

`mkdir build && cd build`

`../configure --target=i686-inlow --prefix="$HOME/inlow-toolchain" --with-sysroot="$INLOWROOT/sysroot" --disable-werror --disable-nls`

`make`

`make install`

##### Build GCC
`cd $HOME/src`

`git clone https://github.com/qvjp/gcc-inlow-7.2.0.git`

`cd gcc-inlow-7.2.0`

`mkdir build && cd build`

`../configure --target=i686-inlow --prefix="$HOME/inlow-toolchain" --with-sysroot="$INLOWROOT/sysroot" --enable-languages=c,c++ --disable-nls`

`make all-gcc all-target-libgcc`

`make install-gcc install-target-libgcc`

#### Build Kernel
`cd $INLOWROOT`

`make`

------------

## RUN
`cd $INLOWROOT`

`make qemu`