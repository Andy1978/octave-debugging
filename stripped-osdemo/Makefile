.PHONY:clean

TARGETS = osdemo cube

all: $(TARGETS)

osdemo: osdemo.c
	gcc -Wall -Wextra $^ -o $@ -lOSMesa 

cube: cube.c
	gcc -Wall -Wextra $^ -o $@ -lGL -lGLU -lglut

clean:
	rm -f $(TARGETS) *.tga
