CFLAGS += -I../can
LDFLAGS += -L../can

PREFIX = /usr/local

libepos.so: epos.c
	$(CC) $(CFLAGS) $(LDFLAGS) -Wall -shared -o libepos.so epos.c -lcan

install: libepos.so
	install -c -m 644 libepos.so $(PREFIX)/lib

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
