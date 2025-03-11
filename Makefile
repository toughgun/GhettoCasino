all: main

main: main.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp
	g++ main.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp \
	libggfonts.a -Wall -lX11 -lGL -lGLU -lm -o main

clean:
		rm -f main
