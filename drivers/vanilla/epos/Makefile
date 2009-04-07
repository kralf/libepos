CFLAGS += -I../can -fPIC
LDFLAGS += -L../can -lcan

PREFIX = /usr/local
HEADERS = error.h device.h sensor.h motor.h gear.h control.h home.h \
	position.h velocity.h current.h epos.h
SOURCES = error.c device.c sensor.c motor.c gear.c control.c home.c \
	position.c velocity.c current.c epos.c

libepos.so: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o libepos.so $(SOURCES)

install: libepos.so
	install -c -m 755 libepos.so $(PREFIX)/lib

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
