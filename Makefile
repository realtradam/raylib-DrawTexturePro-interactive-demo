desktop:
	zig cc -target native main.c -o game -lGL -lm -lpthread -ldl -lrt -lX11 -Iraylib/src -Iraygui/src lib/tux/libraylib.a && ./game
web:
	emcc -Os -Wall main.c -o game -Iraylib/src -Iraygui/src lib/web/libraylib.a -o output/index.html -s USE_GLFW=3 -s FULL_ES3 -DPLATFORM_WEB --shell-file raylib/src/minshell.html -s TOTAL_MEMORY=268435456 --preload-file ./assets
