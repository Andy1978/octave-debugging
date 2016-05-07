##
## dependencies: OSMesa (libosmesa6-dev in Debian jessie)
##

.PHONY: all check clean

OCT := debug_osmesa.oct

MKOCTFILE  ?= mkoctfile
MKOCTFLAGS ?= -Wall -v -g

all: $(OCT)

check: all
	octave -q -f --no-gui --eval "test debug_osmesa"

%.oct: %.cc
	$(MKOCTFILE) $(MKOCTFLAGS) $^ -o $@ -lOSMesa

clean:
	rm -f *.oct *.o debug_osmesa.png
	rm -f octave-core octave-workspace
