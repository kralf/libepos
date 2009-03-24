CFLAGS += -I../can -fPIC
LDFLAGS += -L../can -lcan

PREFIX = /usr/local

libepos.so: error.h error.c device.h device.c sensor.h sensor.c \
	motor.h motor.c control.h control.c epos.h epos.c
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o libepos.so \
	error.c device.c sensor.c motor.c control.c epos.c

install: libepos.so
	install -c -m 755 libepos.so $(PREFIX)/lib

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
