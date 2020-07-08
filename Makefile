all:
	g++ term.cpp std.c -o term -lutil -lX11
clean:
	rm term
