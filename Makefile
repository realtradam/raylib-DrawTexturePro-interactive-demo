desktop:
	zig cc -target native main.c -o game -lGL -lm -lpthread -ldl -lrt -lX11 -Iraylib/src -Iraygui/src raylib/src/libraylib.a && ./game
