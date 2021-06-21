CC = gcc 

LDLIBS = -lglut -lGL -lGLU -lX11 -lm

.c:
	$(CC) $@.c $(LDLIBS) -o $@
