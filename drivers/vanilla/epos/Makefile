CFLAGS += -I../can
LDFLAGS += -L../can

libepos.so: epos.c
	$(CC) $(CFLAGS) $(LDFLAGS) -Wall -shared -o libepos.so epos.c -lcan

install: libepos.so
	install -c -o root -g root -m 644 libepos.so /usr/lib

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
