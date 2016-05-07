.PHONY:clean realcean

CXXFLAGS = $(shell fltk-config --use-gl --cxxflags)
LDFLAGS = $(shell fltk-config --use-gl --ldflags)

CubeView: CubeViewUI.cxx CubeView.cxx
	g++ -Wall -Wextra $^ -o $@ $(LDFLAGS) -lGL

CubeViewUI.cxx: CubeViewUI.fl
	fluid -c $<

clean:
	rm -f CubeView

realclean: clean
	rm -f CubeViewUI.cxx CubeViewUI.h
