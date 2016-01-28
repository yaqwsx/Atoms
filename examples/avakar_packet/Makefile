
RM = rm -f
ifdef SystemRoot
	EXEEXT = .exe
	RM = del /Q
endif

ARGS = -I../../include -std=c++11 -g3


all: value

value: main.cpp
	g++ $(ARGS) -o avakar_packet main.cpp

clean:
	$(RM) avakar_packet$(EXEEXT)