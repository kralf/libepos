CFLAGS = -I../../vanilla/can -I../../vanilla/epos -I../../../posix/timer
LDFLAGS = -L ../../vanilla/epos -L ../../../posix/timer -lepos -ltimer

PREFIX = /usr/local

OBJS = epos_version epos_error epos_baudrate epos_bitrate epos_sensor \
	epos_init epos_control epos_home epos_position epos_velocity epos_current \
	epos_profile epos_nod epos_estimate

all: $(OBJS)

install: $(OBJS)
	install -c -m 755 $(OBJS) $(PREFIX)/bin

clean:
	rm -f *.o $(OBJS)
