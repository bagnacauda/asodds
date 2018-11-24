CC=gcc
LIBS = "-lpthread"

all: asodds

asodds: main.c libmyhtml_static.a
	$(CC) -o asodds *.c `pkg-config --cflags libcurl` libmyhtml_static.a $(LIBS) `pkg-config --libs libcurl`

install: asodds
	mv asodds /usr/bin

clean: 
	rm asodds
