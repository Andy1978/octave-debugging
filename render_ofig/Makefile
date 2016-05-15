.PHONY:clean

TARGETS = render_ofig

all: $(TARGETS)

#render_ofig: render_ofig.cpp
	#mkoctfile -v --link-stand-alone $^ -o $@ -lOSMesa
#	mkoctfile -v --link-stand-alone $^ -o $@

render_ofig: render_ofig.cpp
	g++ -c  -fPIC -I/usr/local/include/octave-4.0.2/octave/.. -I/usr/local/include/octave-4.0.2/octave -I/usr/local/include -g -O2 render_ofig.cpp -o render_ofig.o
	g++  -I/usr/local/include/octave-4.0.2/octave/.. -I/usr/local/include/octave-4.0.2/octave -I/usr/local/include -g -O2 -fPIC -o render_ofig  render_ofig.o -L/usr/local/lib/octave/4.0.2 -L/usr/local/lib -loctinterp -loctave -lOSMesa

run:
	./render_ofig out.tga

clean:
	rm -f $(TARGETS) *.tga *.o
