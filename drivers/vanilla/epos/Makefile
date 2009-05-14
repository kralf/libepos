CFLAGS += -I../can -I../../../posix/config -I../../../posix/timer -fPIC
LDFLAGS += -L../can -lcan -L../../../posix/timer -ltimer -lm

PREFIX = /usr/local
OBJECTS = error.o device.o sensor.o motor.o gear.o input.o control.o \
	home.o position.o velocity.o current.o profile.o position_profile.o \
	velocity_profile.o epos.o

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

libepos.so: $(OBJECTS)
	$(CC) $(LDFLAGS) -shared -o libepos.so $(OBJECTS)

install: libepos.so
	install -c -m 755 libepos.so $(PREFIX)/lib

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
