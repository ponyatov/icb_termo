.PHONY: all
all: packages doc

## install required packages

.PHONY: packages gcc libc libcoap
packages: gcc libc libcoap
gcc: /usr/bin/gcc
/usr/bin/gcc:
	sudo apt install gcc
libc: /usr/include/stdlib.h
/usr/include/stdlib.h:
	sudo apt install libc-dev
libcoap: src/libcoap/README
src/libcoap/README:
	git clone -o gh --depth=1 -b master https://github.com/obgm/libcoap.git src/libcoap

## generate documentation

FIG = doc/00.svg doc/01.svg

.PHONY: doc
doc: $(FIG)
doc/%.png: doc/%.dot
	dot -Tpng -o $@ $<
doc/%.svg: doc/%.dot
	dot -Tsvg -o $@ $<
