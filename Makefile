all: background

background: background.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp
	g++ background.cpp akoli.cpp bolayvar.cpp crodriguez4.cpp hchen.cpp -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f background a.out


