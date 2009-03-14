CFLAGS = -I../../vanilla/epos
LDFLAGS = -L ../../vanilla/epos -lepos

PREFIX = /usr/local

OBJS = epos_init

all: $(OBJS)

install: $(OBJS)
	install -c -m 755 $(OBJS) $(PREFIX)/bin

clean:
	rm -f *.o $(OBJS)
