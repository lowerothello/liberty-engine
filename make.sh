#!/bin/sh -ex

warnings='-Wall'
libs="$(pkg-config --libs --cflags sdl2)"
shared="-shared -fPIC"

# $(pkg-config --libs --cflags jack) \
[ "$1" = "pg" ] && {
	time gcc -o build/libliberty.so -O0 \
		-lm $libs $warnings -g -pg $shared \
		src/main.c 2>&1
	time gcc -o libertytest -O0 \
		-lm $libs -Lbuild -lliberty $warnings -g -pg \
		test/main.c 2>&1
	return
}

[ "$1" ] && {
	time ${CC:-gcc} -o build/libliberty.so -O$1 \
		-lm $libs $warnings -g $shared \
		src/main.c 2>&1
	time ${CC:-gcc} -o libertytest -O$1 \
		-lm $libs -Lbuild -lliberty $warnings -g \
		test/main.c 2>&1
	return
}

time ${CC:-gcc} -o build/libliberty.so -O0 \
	-lm $libs $warnings -g $shared \
	src/main.c 2>&1
time ${CC:-gcc} -o libertytest -O0 \
	-lm $libs -Lbuild -lliberty $warnings -g \
	test/main.c 2>&1
return
