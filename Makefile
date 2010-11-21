.PHONY: clean

circletones: circletones.c
	$(CC) -o circletones circletones.c `sdl-config --cflags --libs` -lSDL_gfx

clean:
	$(RM) circletones
