.PHONY:clean

TARGETS = sombrero.pdf sombrero.ofig render_ofig

all: $(TARGETS)

render_ofig: render_ofig.cpp
	mkoctfile -v --link-stand-alone $^ -o $@ -lOSMesa

sombrero.ofig:
	octave -q -f --eval "figure visible off; sombrero; hgsave $@;"

sombrero.pdf: render_ofig sombrero.ofig
	./render_ofig

clean:
	rm -f $(TARGETS) *.o octave-workspace
