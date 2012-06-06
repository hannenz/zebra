CC=gcc
CFLAGS=-Wall
LIBS=gtk+-3.0 clutter-1.0 clutter-gtk-1.0 webkitgtk-3.0
OBJ=main.o zebra_timeline.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) `pkg-config --cflags $(LIBS)`

zebra: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) `pkg-config --libs $(LIBS)`
