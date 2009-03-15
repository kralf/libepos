CFLAGS += -I../can -fPIC
LDFLAGS += -L../can -lcan

PREFIX = /usr/local

libepos.so: epos.h epos.c
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o libepos.so epos.c

install: libepos.so
	install -c -m 755 libepos.so $(PREFIX)/lib

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
