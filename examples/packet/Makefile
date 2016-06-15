
RM = rm -f
ifdef SystemRoot
	EXEEXT = .exe
	RM = del /Q
endif

ARGS = -I../../include -std=c++14 -g3 -Wall


all: value

value: main.cpp
	g++ $(ARGS) -o packet main.cpp

clean:
	$(RM) packet$(EXEEXT)