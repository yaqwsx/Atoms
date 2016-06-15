
RM = rm -f
ifdef SystemRoot
	EXEEXT = .exe
	RM = del /Q
endif

ARGS = -I../../include -std=c++14 -Wall


all: value

value: main.cpp
	g++ $(ARGS) -o value main.cpp

clean:
	$(RM) value$(EXEEXT)