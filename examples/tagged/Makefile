
RM = rm -f
ifdef SystemRoot
	EXEEXT = .exe
	RM = del /Q
endif

ARGS = -I../../include -std=c++14 -Wall


all: tagged

tagged: main.cpp
	clang++ $(ARGS) -o tagged main.cpp

clean:
	$(RM) tagged$(EXEEXT)