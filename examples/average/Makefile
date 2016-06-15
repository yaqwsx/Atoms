
RM = rm -f
ifdef SystemRoot
	EXEEXT = .exe
	RM = del /Q
endif

ARGS = -I../../include -std=c++11 -g3 -Wall


all: value

value: main.cpp
	g++ $(ARGS) -o average main.cpp

clean:
	$(RM) average$(EXEEXT)