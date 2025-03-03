all: background

background: background.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp
	g++ background.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp \
	libggfonts.a -Wall -lX11 -lGL -lGLU -lm -o background

clean:
		rm -f background
