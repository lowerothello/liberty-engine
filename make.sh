#!/bin/sh -ex
# compiles liberty
# usage:
#   compile a debug build:   $ ./make.sh g
#   compile a release build: $ ./make.sh

libs="$(pkg-config --libs --cflags sdl3)"
warnings='-Wall'
shared="-shared -fPIC"
debug="-g"

outputdir="output"
mkdir -p "$outputdir"

case "$1" in
	"g"*)
		time gcc -o "$outputdir"/libliberty.so -O0 \
			-lm $libs $warnings $debug -DLIBERTY_DEBUG $shared \
			src/main.c 2>&1
		time gcc -o "$outputdir"/libertytest -O0 \
			-lm $libs -L"$outputdir" -lliberty $warnings $debug \
			test/main.c 2>&1
		;;
	*)
		time ${CC:-gcc} -o "$outputdir"/libliberty.so -O${1:-0} \
			-lm $libs $warnings $shared \
			src/main.c 2>&1
		time ${CC:-gcc} -o "$outputdir"/libertytest -O${1:-0} \
			-lm $libs -L"$outputdir" -lliberty $warnings \
			test/main.c 2>&1
		;;
esac
