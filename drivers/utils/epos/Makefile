CFLAGS = -I../../vanilla/epos -I../../../posix/timer
LDFLAGS = -L ../../vanilla/epos -L ../../../posix/timer -lepos -ltimer

PREFIX = /usr/local

OBJS = epos_init epos_version epos_status epos_vel epos_profile

all: $(OBJS)

install: $(OBJS)
	install -c -m 755 $(OBJS) $(PREFIX)/bin

clean:
	rm -f *.o $(OBJS)
