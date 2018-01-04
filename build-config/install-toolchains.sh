#! /bin/sh

set -e

binutils_repo=https://github.com/qvjp/binutils-inlow-2.26.git
gcc_repo=https://github.com/qvjp/gcc-inlow-6.1.0.git

[ -z "${PREFIX+x}" ] && PREFIX="$HOME/inlow-toolchain"
[ -z "$SRCDIR" ] && SRCDIR="$HOME/src"
[ -z "$BUILDDIR" ] && BUILDDIR="$SRCDIR"
[ -z "$ARCH" ] && ARCH=i686
[ -z "$TARGET" ] && TARGET=$ARCH-inlow

[ -z "$SYSROOT" ] && echo "Error: \$SYSROOT not set" && exit 1

if [ "$CONTINUOUS_INTEGRATION" = true ]
then
	current_binutils=$(cat "$PREFIX/binutils-commit" || echo Not installed)
	current_gcc=$(cat "$PREFIX/gcc-commit" || echo Not installed)
	latest_binutils=$(git ls-remote $binutils_repo HEAD | cut -c 1-40)
	latest_gcc=$(git ls-remote $gcc_repo HEAD | cut -c 1-40)
	[ "$current_binutils" = "$latest_binutils" ] && \
			[ "$current_gcc" = "$latest_gcc"] && \
			echo Cached Toolchain is already up to date. && exit
	rm -rf "$PREFIX"
fi

SYSROOT="$(cd "$SYSROOT" && pwd)"

export PATH="$PREFIX/bin:$PATH"

rm -rf "$SRCDIR/inlow-binutils" "$SRCDIR/inlow-gcc"
rm -rf "$BUILDDIR/build-binutils" "$BUILDDIR/build-gcc"

echo Downloading sources...
mkdir -p "$SRCDIR"
cd "$SRCDIR"
git clone $binutils_repo inlow-binutils
git clone $gcc_repo inlow-gcc

echo Building binutils...
mkdir -p "$BUILDDIR/build-binutils"
cd "$BUILDDIR/build-binutils"
"$SRCDIR/inlow-binutils/configure" --target=$TARGET --prefix="$PREFIX" \
		--with-sysroot="$SYSROOT" --disable-werror --disable-nls
make -j5
make install

echo Building gcc...
mkdir -p "$BUILDDIR/build-gcc"
cd "$BUILDDIR/build-gcc"
"$SRCDIR/inlow-gcc/configure" --target=$TARGET --prefix="$PREFIX" \
		--with-sysroot="$SYSROOT" --enable-languages=c,c++ --disable-nls
make -j5 all-gcc all-target-libgcc
make install-gcc install-target-libgcc

if [ "$CONTINUOUS_INTEGRATION" = true ]
then
	cd "$SRCDIR/inlow-binutils"
	git rev-parse HEAD > "$PREFIX/binutils-commit"
	cd "$SRCDIR/inlow-gcc"
	git rev-parse HEAD > "$PREFIX/gcc-commit"
fi

echo Install completed
echo $PATH
