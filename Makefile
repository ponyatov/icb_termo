.PHONY: all
all: packages coap doc

## install required packages

.PHONY: packages gcc libc autotools
packages: gcc libc autotools

### GNU toolchain

gcc: /usr/bin/gcc
/usr/bin/gcc:
	sudo apt install gcc

libc: /usr/include/stdlib.h
/usr/include/stdlib.h:
	sudo apt install libc-dev
	
### autotools req. for libs build

autotools: /usr/bin/autoreconf
/usr/bin/autoreconf:
	sudo apt install autotools-dev

CPU_NUM = $(shell grep processor /proc/cpuinfo|wc -l)
MAKE = make -j$(CPU_NUM) 

## IOT tools and libs

.PHONY: coap libcoap copper
coap: libcoap copper
	
libcoap: /usr/local/bin/coap-server
/usr/local/bin/coap-server: src/libcoap/configure
	rm -rf tmp/libcoap ; mkdir tmp/libcoap ; cd tmp/libcoap ;\
	../../src/libcoap/configure --prefix=/usr/local --disable-documentation &&\
	$(MAKE) && sudo make install-strip
src/libcoap/configure: src/libcoap/README #autotools
	cd src/libcoap ; ./autogen.sh && touch configure
src/libcoap/README:
	git clone -o gh --depth=1 -b master https://github.com/obgm/libcoap.git src/libcoap
	
copper: src/copper/README.md
src/copper/README.md:
	git clone -o gh --depth=1 -b master https://github.com/mkovatsc/Copper.git src/copper

## generate documentation

FIG = doc/00.svg doc/01.svg

.PHONY: doc
doc: $(FIG)
doc/%.png: doc/%.dot
	dot -Tpng -o $@ $<
doc/%.svg: doc/%.dot
	dot -Tsvg -o $@ $<
