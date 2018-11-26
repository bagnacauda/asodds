CC=gcc
LIBS = "-lpthread"

all: asodds

asodds:
	$(CC) -o asodds src/*.c `pkg-config --cflags libcurl` lib/libmyhtml_static.a $(LIBS) `pkg-config --libs libcurl`

install: asodds
	mv asodds /usr/bin

clean: 
	rm asodds
