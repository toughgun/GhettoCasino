all: main

main: main.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp
	g++ main.cpp akoli.cpp image.cpp button.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp \
	libggfonts.a -Wall -lX11 -lGL -lGLU -lglut -lm -o main

clean:
		rm -f main
