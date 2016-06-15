
RM = rm -f
ifdef SystemRoot
	EXEEXT = .exe
	RM = del /Q
endif

ARGS = -I../../include -std=c++11 -g3 -Wall


all: value

value: main.cpp
	g++ $(ARGS) -o fixed main.cpp

clean:
	$(RM) fixed$(EXEEXT)